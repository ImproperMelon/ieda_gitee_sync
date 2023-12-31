// ***************************************************************************************
// Copyright (c) 2023-2025 Peng Cheng Laboratory
// Copyright (c) 2023-2025 Institute of Computing Technology, Chinese Academy of Sciences
// Copyright (c) 2023-2025 Beijing Institute of Open Source Chip
//
// iEDA is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan PSL v2.
// You may obtain a copy of Mulan PSL v2 at:
// http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
// MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.
// ***************************************************************************************
#include "BStarTree.hh"

namespace ipl::imp {

BStarTree::BStarTree(std::vector<FPInst*> macro_list, Setting* set) : MPSolution(macro_list)
{
  // set
  _tolerance = 0;
  _new_block_x_shift = 0;
  _new_block_y_shift = 0;
  _total_contour_area = 0;
  _swap_pro = 0.3;
  _move_pro = 0.3;
  _rotate = false;
  _rotate_macro_index = 0;
  _old_orient = Orient::kN;

  for (int i = 0; i < _num_macro + 2; ++i) {
    BStarTreeNode* tree_node = new BStarTreeNode();
    BStarTreeNode* old_tree_node = new BStarTreeNode();
    tree_node->_parent = UNDEFINED;
    tree_node->_left = UNDEFINED;
    tree_node->_right = UNDEFINED;
    old_tree_node->_parent = UNDEFINED;
    old_tree_node->_left = UNDEFINED;
    old_tree_node->_right = UNDEFINED;
    _tree.emplace_back(tree_node);
    _old_tree.emplace_back(old_tree_node);

    ContourNode* contour_node = new ContourNode();
    contour_node->_next = UNDEFINED;
    contour_node->_prev = UNDEFINED;
    contour_node->_begin = UNDEFINED;
    contour_node->_ctl = UNDEFINED;
    _contour.emplace_back(contour_node);
  }
  _contour[_num_macro]->_next = _num_macro + 1;
  _contour[_num_macro]->_prev = UNDEFINED;
  _contour[_num_macro]->_begin = 0;
  _contour[_num_macro]->_end = 0;
  _contour[_num_macro]->_ctl = UINT32_MAX;
  _contour[_num_macro + 1]->_next = UNDEFINED;
  _contour[_num_macro + 1]->_prev = _num_macro;
  _contour[_num_macro + 1]->_begin = 0;
  _contour[_num_macro + 1]->_end = INT32_MAX;
  _contour[_num_macro + 1]->_ctl = 0;

  // inst_1 represents the left edge and inst_2 represents the right edge
  FPInst* inst_1 = new FPInst();
  FPInst* inst_2 = new FPInst();
  inst_1->set_name("left_edge");
  inst_2->set_name("right_edge");
  inst_1->set_height(UINT32_MAX);
  inst_2->set_width(UINT32_MAX);
  _macro_list.emplace_back(inst_1);
  _macro_list.emplace_back(inst_2);

  _swap_pro = set->get_swap_pro();
  _move_pro = set->get_move_pro();
  inittree();
}

BStarTree::~BStarTree()
{
  for (BStarTreeNode* node : _tree) {
    if (node != nullptr) {
      delete node;
      node = nullptr;
    }
  }
  _tree.clear();

  for (BStarTreeNode* node : _old_tree) {
    if (node != nullptr) {
      delete node;
      node = nullptr;
    }
  }
  _old_tree.clear();

  for (ContourNode* node : _contour) {
    if (node != nullptr) {
      delete node;
      node = nullptr;
    }
  }
  _contour.clear();

  // delete right_edge and left_edge
  FPInst* right_edge = _macro_list.back();
  if (right_edge != nullptr && right_edge->get_name() == "right_edge") {
    delete right_edge;
    _macro_list[-1] = nullptr;
    _macro_list.pop_back();
  }
  FPInst* left_edge = _macro_list.back();
  if (left_edge != nullptr && left_edge->get_name() == "right_edge") {
    delete left_edge;
    _macro_list[-1] = nullptr;
    _macro_list.pop_back();
  }
}

void BStarTree::inittree()
{
  _tree[0]->_parent = _num_macro;
  _tree[_num_macro]->_left = 0;
  int index = 1;
  for (int i = 0; i < _num_macro; ++i) {
    if (index == _num_macro) {
      continue;
    }
    _tree[i]->_left = index;
    _tree[index]->_parent = i;
    ++index;
    if (index == _num_macro) {
      continue;
    }
    _tree[i]->_right = index;
    _tree[index]->_parent = i;
    ++index;
  }
  for (size_t i = 0; i < _tree.size(); ++i) {
    _old_tree[i]->_parent = _tree[i]->_parent;
    _old_tree[i]->_left = _tree[i]->_left;
    _old_tree[i]->_right = _tree[i]->_right;
  }
  pack();
}

void BStarTree::rollback()
{
  if (!_rotate) {
    for (size_t i = 0; i < _tree.size(); ++i) {
      _tree[i]->_parent = _old_tree[i]->_parent;
      _tree[i]->_left = _old_tree[i]->_left;
      _tree[i]->_right = _old_tree[i]->_right;
    }
  } else {
    _macro_list[_rotate_macro_index]->set_orient(_old_orient);
  }
}

void BStarTree::update()
{
  if (!_rotate) {
    for (size_t i = 0; i < _tree.size(); ++i) {
      _old_tree[i]->_parent = _tree[i]->_parent;
      _old_tree[i]->_left = _tree[i]->_left;
      _old_tree[i]->_right = _tree[i]->_right;
    }
  }
}

void BStarTree::perturb()
{
  float rand_num = rand() / (RAND_MAX + 1.0);
  // swap node
  if (rand_num < _swap_pro) {
    _rotate = false;
    int block_a = int(_num_macro * (rand() / (RAND_MAX + 1.0)));
    int block_b = int((_num_macro - 1) * (rand() / (RAND_MAX + 1.0)));
    block_b = (block_b >= block_a) ? block_b + 1 : block_b;
    swap(block_a, block_b);
    // rotate node
  } else if (rand_num < 1 - _move_pro) {
    _rotate = true;
    _rotate_macro_index = int(_num_macro * (rand() / (RAND_MAX + 1.0)));
    _old_orient = _macro_list[_rotate_macro_index]->get_orient();
    int new_orient = (_rotate_macro_index + 1) % 8;  // the total of orient is 8
    _macro_list[_rotate_macro_index]->set_orient(Orient(new_orient));
    // move node
  } else {
    _rotate = false;
    int block = int(_num_macro * (rand() / (RAND_MAX + 1.0)));
    int target_rand_num = int((2 * _num_macro - 1) * (rand() / (RAND_MAX + 1.0)));
    int target = target_rand_num / 2;
    target = (target == block) ? target - 1 : target;
    target = (target < 0) ? 1 : target;
    int left_child = target_rand_num % 2;
    move(block, target, (left_child != 0));
  }
  pack();
}

void BStarTree::swap(int index_one, int index_two)
{
  int index_one_left = _tree[index_one]->_left;
  int index_one_right = _tree[index_one]->_right;
  int index_one_parent = _tree[index_one]->_parent;

  int index_two_left = _tree[index_two]->_left;
  int index_two_right = _tree[index_two]->_right;
  int index_two_parent = _tree[index_two]->_parent;

  if (index_one == index_two_parent)
    swapParentChild(index_one, (index_two == _tree[index_one]->_left));
  else if (index_two == index_one_parent)
    swapParentChild(index_two, (index_one == _tree[index_two]->_left));
  else {
    // update around index_one
    _tree[index_one]->_parent = index_two_parent;
    _tree[index_one]->_left = index_two_left;
    _tree[index_one]->_right = index_two_right;

    if (index_one == _tree[index_one_parent]->_left)
      _tree[index_one_parent]->_left = index_two;
    else
      _tree[index_one_parent]->_right = index_two;

    if (index_one_left != UNDEFINED)
      _tree[index_one_left]->_parent = index_two;

    if (index_one_right != UNDEFINED)
      _tree[index_one_right]->_parent = index_two;

    // update around index_two
    _tree[index_two]->_parent = index_one_parent;
    _tree[index_two]->_left = index_one_left;
    _tree[index_two]->_right = index_one_right;

    if (index_two == _tree[index_two_parent]->_right)  // prevent from that two indexs have togeter parent->
      _tree[index_two_parent]->_right = index_one;     // that will resulting in their parent's child don't change->
    else
      _tree[index_two_parent]->_left = index_one;

    if (index_two_left != UNDEFINED)
      _tree[index_two_left]->_parent = index_one;

    if (index_two_right != UNDEFINED)
      _tree[index_two_right]->_parent = index_one;
  }
}
void BStarTree::swapParentChild(int _parent, bool is_left)
{
  int _parent_parent = _tree[_parent]->_parent;
  int _parent_left = _tree[_parent]->_left;
  int _parent_right = _tree[_parent]->_right;

  int child = (is_left) ? _tree[_parent]->_left : _tree[_parent]->_right;
  int child_left = _tree[child]->_left;
  int child_right = _tree[child]->_right;

  if (is_left) {
    _tree[_parent]->_parent = child;
    _tree[_parent]->_left = child_left;
    _tree[_parent]->_right = child_right;

    if (_parent == _tree[_parent_parent]->_left)
      _tree[_parent_parent]->_left = child;
    else
      _tree[_parent_parent]->_right = child;

    if (_parent_right != UNDEFINED)
      _tree[_parent_right]->_parent = child;

    _tree[child]->_parent = _parent_parent;
    _tree[child]->_left = _parent;
    _tree[child]->_right = _parent_right;

    if (child_left != UNDEFINED)
      _tree[child_left]->_parent = _parent;

    if (child_right != UNDEFINED)
      _tree[child_right]->_parent = _parent;
  } else {
    _tree[_parent]->_parent = child;
    _tree[_parent]->_left = child_left;
    _tree[_parent]->_right = child_right;

    if (_parent == _tree[_parent_parent]->_left)
      _tree[_parent_parent]->_left = child;
    else
      _tree[_parent_parent]->_right = child;

    if (_parent_left != UNDEFINED)
      _tree[_parent_left]->_parent = child;

    _tree[child]->_parent = _parent_parent;
    _tree[child]->_left = _parent_left;
    _tree[child]->_right = _parent;

    if (child_left != UNDEFINED)
      _tree[child_left]->_parent = _parent;

    if (child_right != UNDEFINED)
      _tree[child_right]->_parent = _parent;
  }
}

void BStarTree::move(int index, int target, bool _left_child)
{
  int index_parent = _tree[index]->_parent;
  int index_left = _tree[index]->_left;
  int index_right = _tree[index]->_right;

  // remove "index" from the tree
  if ((index_left != UNDEFINED) && (index_right != UNDEFINED))
    removeUpChild(index);
  else if (index_left != UNDEFINED) {
    _tree[index_left]->_parent = index_parent;
    if (index == _tree[index_parent]->_left)
      _tree[index_parent]->_left = index_left;
    else
      _tree[index_parent]->_right = index_left;
  } else if (index_right != UNDEFINED) {
    _tree[index_right]->_parent = index_parent;
    if (index == _tree[index_parent]->_left)
      _tree[index_parent]->_left = index_right;
    else
      _tree[index_parent]->_right = index_right;
  } else {
    if (index == _tree[index_parent]->_left)
      _tree[index_parent]->_left = UNDEFINED;
    else
      _tree[index_parent]->_right = UNDEFINED;
  }

  int target_left = _tree[target]->_left;
  int target_right = _tree[target]->_right;

  // add "index" to the required coordinate
  if (_left_child) {
    _tree[target]->_left = index;
    if (target_left != UNDEFINED)
      _tree[target_left]->_parent = index;

    _tree[index]->_parent = target;
    _tree[index]->_left = target_left;
    _tree[index]->_right = UNDEFINED;
  } else {
    _tree[target]->_right = index;
    if (target_right != UNDEFINED)
      _tree[target_right]->_parent = index;

    _tree[index]->_parent = target;
    _tree[index]->_left = UNDEFINED;
    _tree[index]->_right = target_right;
  }
}
void BStarTree::removeUpChild(int index)
{
  int index_parent = _tree[index]->_parent;
  int index_left = _tree[index]->_left;
  int index_right = _tree[index]->_right;

  _tree[index_left]->_parent = index_parent;
  if (index == _tree[index_parent]->_left)
    _tree[index_parent]->_left = index_left;
  else
    _tree[index_parent]->_right = index_left;

  int ptr = index_left;
  while (_tree[ptr]->_right != UNDEFINED)
    ptr = _tree[ptr]->_right;

  _tree[ptr]->_right = index_right;
  _tree[index_right]->_parent = ptr;
}

void BStarTree::pack()
{
  clean_contour();
  // x- and y- shifts for new block to avoid obstacles
  _new_block_x_shift = 0;
  _new_block_y_shift = 0;

  int tree_prev = _num_macro;
  int tree_curr = _tree[_num_macro]->_left;  // start with first block

  while (tree_curr != _num_macro)  // until reach the root again
  {
    if (tree_prev == _tree[tree_curr]->_parent) {
      unsigned obstacle_id = UINT_MAX;
      int32_t obstacle_x_min, obstacle_x_max, obstacle_y_min, obstacle_y_max;
      int32_t new_x_min, new_x_max, new_y_min, new_y_max;

      if (isIntersectsObstacle(tree_curr, obstacle_id, new_x_min, new_x_max, new_y_min, new_y_max, obstacle_x_min, obstacle_x_max,
                               obstacle_y_min, obstacle_y_max)) {
        // 'add obstacle' and then resume building the tree from here
        int tree_parent = _tree[tree_curr]->_parent;
        int32_t block_height = new_y_max - new_y_min;
        int32_t block_width = new_x_max - new_x_min;

        if ((tree_curr == _tree[tree_parent]->_left && obstacle_y_max + block_height > _contour[tree_parent]->_ctl + (block_height * 0.5)
             && (obstacle_x_max + block_width) < _obstacleframe[0])
            || (tree_curr == _tree[tree_parent]->_right && (obstacle_y_max + block_height) > _obstacleframe[1])) {
          // _left child & shifting up makes it too high
          // or _right child & shifting up makes it too high;
          // shift the starting location of the block _right in x
          _new_block_x_shift += obstacle_x_max - new_x_min;
          _new_block_y_shift = 0;
        } else {
          // shift the block in y
          _new_block_y_shift += obstacle_y_max - new_y_min;
        }
      } else {
        addContourBlock(tree_curr);

        // reset x- y- obstacle shift
        _new_block_x_shift = 0;
        _new_block_y_shift = 0;

        tree_prev = tree_curr;
        if (_tree[tree_curr]->_left != UNDEFINED)
          tree_curr = _tree[tree_curr]->_left;
        else if (_tree[tree_curr]->_right != UNDEFINED)
          tree_curr = _tree[tree_curr]->_right;
        else
          tree_curr = _tree[tree_curr]->_parent;
      }
    } else if (tree_prev == _tree[tree_curr]->_left) {
      tree_prev = tree_curr;
      if (_tree[tree_curr]->_right != UNDEFINED)
        tree_curr = _tree[tree_curr]->_right;
      else
        tree_curr = _tree[tree_curr]->_parent;
    } else {
      tree_prev = tree_curr;
      tree_curr = _tree[tree_curr]->_parent;
    }
  }
  _total_width = _contour[_num_macro + 1]->_begin;

  int contour_ptr = _contour[_num_macro]->_next;
  _total_height = 0;

  _total_contour_area = 0;
  while (contour_ptr != _num_macro + 1) {
    _total_height = std::max(_total_height, _contour[contour_ptr]->_ctl);
    // Calculate contour area
    _total_contour_area += (_contour[contour_ptr]->_end - _contour[contour_ptr]->_begin) * _contour[contour_ptr]->_ctl;
    // go to _next pointer
    contour_ptr = _contour[contour_ptr]->_next;
  }
  _total_area = float(_total_width) * float(_total_height);
}

void BStarTree::clean_contour()
{
  int vec_size = _contour.size();
  int ledge = vec_size - 2;
  int bedge = vec_size - 1;

  _contour[ledge]->_next = bedge;
  _contour[ledge]->_prev = UNDEFINED;
  _contour[ledge]->_begin = 0;
  _contour[ledge]->_end = 0;
  _contour[ledge]->_ctl = UINT32_MAX;

  _contour[bedge]->_next = UNDEFINED;
  _contour[bedge]->_prev = ledge;
  _contour[bedge]->_begin = 0;
  _contour[bedge]->_end = INT32_MAX;
  _contour[bedge]->_ctl = 0;

  // reset obstacles (so we consider all of them again)
  if (_seen_obstacles.size() != get_num_obstacles())
    _seen_obstacles.resize(get_num_obstacles());
  fill(_seen_obstacles.begin(), _seen_obstacles.end(), false);
}

bool BStarTree::isIntersectsObstacle(const int tree_ptr, unsigned& obstacle_id, int32_t& new_x_min, int32_t& new_x_max, int32_t& new_y_min,
                                     int32_t& new_y_max, int32_t& obstacle_x_min, int32_t& obstacle_x_max, int32_t& obstacle_y_min,
                                     int32_t& obstacle_y_max)
// check if adding this new block intersects an obstacle,return obstacle_id if it does
// TODO: smarter way of storing/searching obstacles->
// Currently complexity is O(foreach add_block * foreach unseen_obstacle)
{
  if (get_num_obstacles() == 0)
    return false;  // don't even bother

  obstacle_id = UNDEFINED;
  int contour_ptr = UNDEFINED;
  int contour_prev = UNDEFINED;

  findBlockLocation(tree_ptr, new_x_min, new_y_min, contour_prev, contour_ptr);

  // int block = _tree[tree_ptr]->_block_index;
  // int theta = _tree[tree_ptr]->_orient;

  // get the rest of the bbox of new block, if the new block were added
  new_x_max = new_x_min + _macro_list[tree_ptr]->get_width();
  new_y_max = new_y_min + _macro_list[tree_ptr]->get_height();

  // check if adding this new block will create a contour that
  // intersects with an obstacle
  for (unsigned i = 0; i < get_num_obstacles(); i++) {
    obstacle_x_min = _obstacles[i]->get_x();
    obstacle_y_min = _obstacles[i]->get_y();
    obstacle_x_max = obstacle_x_min + _obstacles[i]->get_width();
    obstacle_y_max = obstacle_y_min + _obstacles[i]->get_height();

    if ((new_x_max <= obstacle_x_min) || (new_x_min >= obstacle_x_max) || (new_y_max <= obstacle_y_min) || (new_y_min >= obstacle_y_max))
      continue;
    obstacle_id = i;
    return true;
  }

  return false;
}

void BStarTree::addContourBlock(const int tree_ptr)
{
  int contour_ptr = UNDEFINED;
  int contour_prev = UNDEFINED;
  int32_t new_xloc, new_yloc;

  findBlockLocation(tree_ptr, new_xloc, new_yloc, contour_prev, contour_ptr);

  _macro_list[tree_ptr]->set_x(new_xloc);
  _macro_list[tree_ptr]->set_y(new_yloc);

  // int tree_parent = _tree[tree_ptr]->_parent;
  _contour[tree_ptr]->_begin = _macro_list[tree_ptr]->get_x();

  _contour[tree_ptr]->_end = _macro_list[tree_ptr]->get_x() + _macro_list[tree_ptr]->get_width();
  _contour[tree_ptr]->_ctl = _macro_list[tree_ptr]->get_y() + _macro_list[tree_ptr]->get_height();
  _contour[tree_ptr]->_next = contour_ptr;
  _contour[tree_ptr]->_prev = contour_prev;

  _contour[contour_ptr]->_prev = tree_ptr;
  _contour[contour_prev]->_next = tree_ptr;
  _contour[contour_ptr]->_begin = _macro_list[tree_ptr]->get_x() + _macro_list[tree_ptr]->get_width();
  _contour[tree_ptr]->_begin = std::max(_contour[contour_prev]->_end, _contour[tree_ptr]->_begin);
}

void BStarTree::findBlockLocation(const int tree_ptr, int32_t& out_x, int32_t& out_y, int& contour_prev, int& contour_ptr)
{
  int tree_parent = _tree[tree_ptr]->_parent;
  contour_prev = UNDEFINED;
  contour_ptr = UNDEFINED;

  // int     block = _tree[tree_ptr]->_block_index;
  int32_t new_block_contour_begin;
  if (tree_ptr == _tree[tree_parent]->_left) {
    // to the right of _parent, start x where _parent _ends
    new_block_contour_begin = _contour[tree_parent]->_end;
    // use block that's right of _parent's contour for y
    contour_ptr = _contour[tree_parent]->_next;
  } else {
    // above _parent, use _parent's x
    new_block_contour_begin = _contour[tree_parent]->_begin;
    // use parent's contour for y
    contour_ptr = tree_parent;
  }

  new_block_contour_begin += _new_block_x_shift;  // considering obstacles
  contour_prev = _contour[contour_ptr]->_prev;    // _begins of cPtr/tPtr match

  int32_t new_block_contour_end = new_block_contour_begin + _macro_list[tree_ptr]->get_width();
  uint32_t max_ctl = _contour[contour_ptr]->_ctl;
  int32_t contour_ptr_end = (contour_ptr == tree_ptr) ? new_block_contour_end : _contour[contour_ptr]->_end;

  while (contour_ptr_end <= new_block_contour_end + _tolerance) {
    max_ctl = std::max(max_ctl, _contour[contour_ptr]->_ctl);
    contour_ptr = _contour[contour_ptr]->_next;
    contour_ptr_end = (contour_ptr == tree_ptr) ? new_block_contour_end : _contour[contour_ptr]->_end;
  }

  // contour_prev location Update!!!!
  int32_t contour_ptr_begin = (contour_ptr == tree_ptr) ? new_block_contour_begin : _contour[contour_ptr]->_begin;

  if (contour_ptr_begin + _tolerance < new_block_contour_end)
    max_ctl = std::max(max_ctl, _contour[contour_ptr]->_ctl);

  // get location where new block sho1uld be added
  out_x = new_block_contour_begin;
  out_y = max_ctl;
}

}  // namespace ipl::imp
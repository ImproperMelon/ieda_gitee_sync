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
#pragma once

#include "Guide.hpp"
#include "LayerRect.hpp"
#include "MTree.hpp"
#include "PHYNode.hpp"
#include "Segment.hpp"

namespace irt {

class RTNode
{
 public:
  RTNode() = default;
  ~RTNode() = default;
  // getter
  Guide& get_first_guide() { return _guide_segment.get_first(); }
  Guide& get_second_guide() { return _guide_segment.get_second(); }
  std::set<irt_int>& get_pin_idx_set() { return _pin_idx_set; }
  MTree<LayerCoord>& get_routing_tree() { return _routing_tree; }
  // setter
  void set_first_guide(const Guide& first_guide) { _guide_segment.set_first(first_guide); }
  void set_second_guide(const Guide& second_guide) { _guide_segment.set_second(second_guide); }
  void set_pin_idx_set(const std::set<irt_int>& pin_idx_set) { _pin_idx_set = pin_idx_set; }
  void set_routing_tree(const MTree<LayerCoord>& routing_tree) { _routing_tree = routing_tree; }
  // function
  bool isTANode() { return get_first_guide().get_grid_coord() != get_second_guide().get_grid_coord(); }
  bool isDRNode() { return !isTANode(); }

 private:
  Segment<Guide> _guide_segment;
  std::set<irt_int> _pin_idx_set;
  MTree<LayerCoord> _routing_tree;
};

}  // namespace irt

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

#include "LayerCoord.hpp"
#include "LayerRect.hpp"
#include "RTNode.hpp"
#include "TAGroup.hpp"

namespace irt {

class TATask
{
 public:
  TATask() = default;
  ~TATask() = default;
  // getter
  irt_int get_origin_net_idx() { return _origin_net_idx; }
  TNode<RTNode>* get_origin_node() { return _origin_node; }
  irt_int get_task_idx() { return _task_idx; }
  std::vector<TAGroup>& get_ta_group_list() { return _ta_group_list; }
  std::map<LayerCoord, double, CmpLayerCoordByXASC>& get_coord_cost_map() { return _coord_cost_map; }
  PlanarRect& get_bounding_box() { return _bounding_box; }
  std::vector<Segment<LayerCoord>>& get_routing_segment_list() { return _routing_segment_list; }
  // setter
  void set_origin_net_idx(const irt_int origin_net_idx) { _origin_net_idx = origin_net_idx; }
  void set_origin_node(TNode<RTNode>* origin_node) { _origin_node = origin_node; }
  void set_task_idx(const irt_int task_idx) { _task_idx = task_idx; }
  void set_ta_group_list(const std::vector<TAGroup>& ta_group_list) { _ta_group_list = ta_group_list; }
  void set_coord_cost_map(const std::map<LayerCoord, double, CmpLayerCoordByXASC>& coord_cost_map) { _coord_cost_map = coord_cost_map; }
  void set_bounding_box(const PlanarRect& bounding_box) { _bounding_box = bounding_box; }
  void set_routing_segment_list(const std::vector<Segment<LayerCoord>>& routing_segment_list)
  {
    _routing_segment_list = routing_segment_list;
  }
  // function

 private:
  irt_int _origin_net_idx = -1;
  TNode<RTNode>* _origin_node = nullptr;
  irt_int _task_idx = -1;
  std::vector<TAGroup> _ta_group_list;
  std::map<LayerCoord, double, CmpLayerCoordByXASC> _coord_cost_map;
  PlanarRect _bounding_box;
  std::vector<Segment<LayerCoord>> _routing_segment_list;
};

}  // namespace irt

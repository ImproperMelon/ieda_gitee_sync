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

#include "GRModelStat.hpp"
#include "GRNet.hpp"
#include "GRNode.hpp"
#include "GRTask.hpp"
#include "GridMap.hpp"

namespace irt {

class GRModel
{
 public:
  GRModel() = default;
  ~GRModel() = default;
  // getter
  std::vector<GridMap<GRNode>>& get_layer_node_map() { return _layer_node_map; }
  std::vector<GRNet>& get_gr_net_list() { return _gr_net_list; }
  GRModelStat& get_gr_model_stat() { return _gr_model_stat; }
  irt_int get_curr_iter() { return _curr_iter; }
  // setter
  void set_layer_node_map(const std::vector<GridMap<GRNode>>& layer_node_map) { _layer_node_map = layer_node_map; }
  void set_gr_net_list(const std::vector<GRNet>& gr_net_list) { _gr_net_list = gr_net_list; }
  void set_gr_model_stat(const GRModelStat& gr_model_stat) { _gr_model_stat = gr_model_stat; }
  void set_curr_iter(const irt_int curr_iter) { _curr_iter = curr_iter; }
#if 1  // astar
  // config
  double get_wire_unit() const { return _wire_unit; }
  double get_corner_unit() const { return _corner_unit; }
  double get_via_unit() const { return _via_unit; }
  void set_wire_unit(const double wire_unit) { _wire_unit = wire_unit; }
  void set_corner_unit(const double corner_unit) { _corner_unit = corner_unit; }
  void set_via_unit(const double via_unit) { _via_unit = via_unit; }
  // single net
  const irt_int get_curr_net_idx() const { return _gr_net_ref->get_net_idx(); }
  const PlanarRect& get_curr_bounding_box() const { return _gr_net_ref->get_bounding_box().get_grid_rect(); }
  const GridMap<double>& get_curr_cost_map() const { return _gr_net_ref->get_ra_cost_map(); }
  PlanarRect& get_routing_region() { return _routing_region; }
  std::vector<GRTask>& get_gr_task_list() { return _gr_task_list; }
  std::vector<Segment<GRNode*>>& get_node_segment_list() { return _node_segment_list; }
  void set_gr_net_ref(GRNet* gr_net_ref) { _gr_net_ref = gr_net_ref; }
  void set_routing_region(const PlanarRect& routing_region) { _routing_region = routing_region; }
  void set_gr_task_list(const std::vector<GRTask>& gr_task_list) { _gr_task_list = gr_task_list; }
  void set_node_segment_list(const std::vector<Segment<GRNode*>>& node_segment_list) { _node_segment_list = node_segment_list; }
  // single task
  std::vector<GRGroup>& get_start_group_list() { return _start_group_list; }
  std::vector<GRGroup>& get_end_group_list() { return _end_group_list; }
  GRGroup& get_path_group() { return _path_group; }
  void set_start_group_list(const std::vector<GRGroup>& start_group_list) { _start_group_list = start_group_list; }
  void set_end_group_list(const std::vector<GRGroup>& end_group_list) { _end_group_list = end_group_list; }
  void set_path_group(const GRGroup& path_group) { _path_group = path_group; }
  // single path
  GRRouteStrategy& get_gr_route_strategy() { return _gr_route_strategy; }
  std::priority_queue<GRNode*, std::vector<GRNode*>, CmpGRNodeCost>& get_open_queue() { return _open_queue; }
  std::vector<GRNode*>& get_visited_node_list() { return _visited_node_list; }
  GRNode* get_path_head_node() { return _path_head_node; }
  irt_int get_end_group_idx() const { return _end_group_idx; }
  void set_gr_route_strategy(const GRRouteStrategy& gr_route_strategy) { _gr_route_strategy = gr_route_strategy; }
  void set_open_queue(const std::priority_queue<GRNode*, std::vector<GRNode*>, CmpGRNodeCost>& open_queue) { _open_queue = open_queue; }
  void set_visited_node_list(const std::vector<GRNode*>& visited_node_list) { _visited_node_list = visited_node_list; }
  void set_path_head_node(GRNode* path_head_node) { _path_head_node = path_head_node; }
  void set_end_group_idx(const irt_int end_group_idx) { _end_group_idx = end_group_idx; }
#endif

 private:
  std::vector<GridMap<GRNode>> _layer_node_map;
  std::vector<GRNet> _gr_net_list;
  GRModelStat _gr_model_stat;
  irt_int _curr_iter;
#if 1  // astar
  // config
  double _wire_unit = 1;
  double _corner_unit = 1;
  double _via_unit = 1;
  // single net
  GRNet* _gr_net_ref = nullptr;
  PlanarRect _routing_region;
  std::vector<GRTask> _gr_task_list;
  std::vector<Segment<GRNode*>> _node_segment_list;
  // single task
  std::vector<GRGroup> _start_group_list;
  std::vector<GRGroup> _end_group_list;
  GRGroup _path_group;
  // single path
  GRRouteStrategy _gr_route_strategy = GRRouteStrategy::kNone;
  std::priority_queue<GRNode*, std::vector<GRNode*>, CmpGRNodeCost> _open_queue;
  std::vector<GRNode*> _visited_node_list;
  GRNode* _path_head_node = nullptr;
  irt_int _end_group_idx = -1;
#endif
};

}  // namespace irt

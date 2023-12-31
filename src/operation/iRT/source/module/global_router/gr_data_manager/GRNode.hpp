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

#include "DRCChecker.hpp"
#include "GRNodeId.hpp"
#include "GRSourceType.hpp"
#include "LayerCoord.hpp"
#include "RegionQuery.hpp"

namespace irt {

#if 1  // astar
enum class GRNodeState
{
  kNone = 0,
  kOpen = 1,
  kClose = 2
};
#endif

class GRNode : public LayerCoord
{
 public:
  GRNode() = default;
  ~GRNode() = default;
  // getter
  GRNodeId& get_gr_node_id() { return _gr_node_id; }
  PlanarRect& get_base_region() { return _base_region; }
  std::map<Orientation, GRNode*>& get_neighbor_ptr_map() { return _neighbor_ptr_map; }
  std::map<GRSourceType, RegionQuery>& get_source_region_query_map() { return _source_region_query_map; }
  irt_int get_whole_wire_demand() const { return _whole_wire_demand; }
  irt_int get_whole_via_demand() const { return _whole_via_demand; }
  std::map<irt_int, std::map<Orientation, irt_int>>& get_net_orien_wire_demand_map() { return _net_orien_wire_demand_map; }
  std::map<irt_int, irt_int>& get_net_via_demand_map() { return _net_via_demand_map; }
  irt_int get_whole_access_demand() const { return _whole_access_demand; }
  std::map<irt_int, std::map<Orientation, irt_int>>& get_net_orien_access_demand_map() { return _net_orien_access_demand_map; }
  std::map<Orientation, irt_int>& get_orien_access_supply_map() { return _orien_access_supply_map; }
  std::map<Orientation, irt_int>& get_orien_access_demand_map() { return _orien_access_demand_map; }
  irt_int get_resource_supply() const { return _resource_supply; }
  irt_int get_resource_demand() const { return _resource_demand; }
  std::map<Orientation, double>& get_history_orien_access_cost_map() { return _history_orien_access_cost_map; }
  double get_history_resource_cost() const { return _history_resource_cost; }
  std::set<irt_int>& get_passed_net_set() { return _passed_net_set; }
  // setter
  void set_gr_node_id(const GRNodeId& gr_node_id) { _gr_node_id = gr_node_id; }
  void set_base_region(const PlanarRect& base_region) { _base_region = base_region; }
  void set_neighbor_ptr_map(const std::map<Orientation, GRNode*>& neighbor_ptr_map) { _neighbor_ptr_map = neighbor_ptr_map; }
  void set_source_region_query_map(const std::map<GRSourceType, RegionQuery>& source_region_query_map)
  {
    _source_region_query_map = source_region_query_map;
  }
  void set_whole_wire_demand(const irt_int whole_wire_demand) { _whole_wire_demand = whole_wire_demand; }
  void set_whole_via_demand(const irt_int whole_via_demand) { _whole_via_demand = whole_via_demand; }
  void set_net_orien_wire_demand_map(const std::map<irt_int, std::map<Orientation, irt_int>>& net_orien_wire_demand_map)
  {
    _net_orien_wire_demand_map = net_orien_wire_demand_map;
  }
  void set_net_via_demand_map(const std::map<irt_int, irt_int>& net_via_demand_map) { _net_via_demand_map = net_via_demand_map; }
  void set_whole_access_demand(const irt_int whole_access_demand) { _whole_access_demand = whole_access_demand; }
  void set_orien_access_supply_map(const std::map<Orientation, irt_int>& orien_access_supply_map)
  {
    _orien_access_supply_map = orien_access_supply_map;
  }
  void set_orien_access_demand_map(const std::map<Orientation, irt_int>& orien_access_demand_map)
  {
    _orien_access_demand_map = orien_access_demand_map;
  }
  void set_resource_supply(const irt_int resource_supply) { _resource_supply = resource_supply; }
  void set_resource_demand(const irt_int resource_demand) { _resource_demand = resource_demand; }
  void set_history_orien_access_cost_map(const std::map<Orientation, double>& history_orien_access_cost_map)
  {
    _history_orien_access_cost_map = history_orien_access_cost_map;
  }
  void set_history_resource_cost(const double history_resource_cost) { _history_resource_cost = history_resource_cost; }
  void set_passed_net_set(const std::set<irt_int>& passed_net_set) { _passed_net_set = passed_net_set; }
  // function
  GRNode* getNeighborNode(Orientation orientation)
  {
    GRNode* neighbor_node = nullptr;
    if (RTUtil::exist(_neighbor_ptr_map, orientation)) {
      neighbor_node = _neighbor_ptr_map[orientation];
    }
    return neighbor_node;
  }
  RegionQuery& getRegionQuery(GRSourceType gr_source_type) { return _source_region_query_map[gr_source_type]; }
  double getCost(irt_int net_idx, Orientation orientation)
  {
    double cost = 0;
    if (orientation != Orientation::kUp && orientation != Orientation::kDown) {
      // 区分默认还是线网自定义
      irt_int net_access_demand = _whole_access_demand;
      if (RTUtil::exist(_net_orien_access_demand_map, net_idx)) {
        if (RTUtil::exist(_net_orien_access_demand_map[net_idx], orientation)) {
          net_access_demand = _net_orien_access_demand_map[net_idx][orientation];
        }
      }
      irt_int node_access_demand = 0;
      if (RTUtil::exist(_orien_access_demand_map, orientation)) {
        node_access_demand = _orien_access_demand_map[orientation];
      }
      irt_int node_access_supply = 0;
      if (RTUtil::exist(_orien_access_supply_map, orientation)) {
        node_access_supply = _orien_access_supply_map[orientation];
      }
      // 放大，统一access和wire的cost
      irt_int converted_demand = (net_access_demand + node_access_demand) * (_whole_wire_demand / 2);
      irt_int converted_supply = node_access_supply * (_whole_wire_demand / 2);
      cost += RTUtil::calcCost(converted_demand, converted_supply);
      if (RTUtil::exist(_history_orien_access_cost_map, orientation)) {
        cost += _history_orien_access_cost_map[orientation];
      }
    }
    if (orientation != Orientation::kUp && orientation != Orientation::kDown) {
      // 区分默认还是线网自定义
      irt_int net_wire_demand = (_whole_wire_demand / 2);
      if (RTUtil::exist(_net_orien_wire_demand_map, net_idx)) {
        if (RTUtil::exist(_net_orien_wire_demand_map[net_idx], orientation)) {
          net_wire_demand = _net_orien_wire_demand_map[net_idx][orientation];
        }
      }
      cost += RTUtil::calcCost(net_wire_demand + _resource_demand, _resource_supply);
      cost += _history_resource_cost;
    } else {
      // 区分默认还是线网自定义
      irt_int net_via_demand = _whole_via_demand;
      if (RTUtil::exist(_net_via_demand_map, net_idx)) {
        net_via_demand = _net_via_demand_map[net_idx];
      }
      cost += RTUtil::calcCost(net_via_demand + _resource_demand, _resource_supply);
      cost += _history_resource_cost;
    }
    return cost;
  }
  void updateDemand(irt_int net_idx, std::set<Orientation> orien_set, ChangeType change_type)
  {
    std::set<Orientation> access_orien_set;
    std::set<Orientation> wire_orien_set;
    std::set<Orientation> via_orien_set;
    for (const Orientation& orien : orien_set) {
      if (orien == Orientation::kEast || orien == Orientation::kWest || orien == Orientation::kSouth || orien == Orientation::kNorth) {
        access_orien_set.insert(orien);
        wire_orien_set.insert(orien);
      } else if (orien == Orientation::kUp || orien == Orientation::kDown) {
        via_orien_set.insert(orien);
      }
    }
    irt_int update_num = (change_type == ChangeType::kAdd ? 1 : -1);
    if (!access_orien_set.empty()) {
      /**
       * 加access_demand
       */
      for (Orientation access_orien : access_orien_set) {
        irt_int net_access_demand = _whole_access_demand;
        if (RTUtil::exist(_net_orien_access_demand_map, net_idx)) {
          if (RTUtil::exist(_net_orien_access_demand_map[net_idx], access_orien)) {
            net_access_demand = _net_orien_access_demand_map[net_idx][access_orien];
          }
        }
        _orien_access_demand_map[access_orien] += (update_num * net_access_demand);
      }
    }
    if (!wire_orien_set.empty()) {
      /**
       * 加wire_demand
       */
      for (Orientation wire_orien : wire_orien_set) {
        irt_int net_wire_demand = (_whole_wire_demand / 2);
        if (RTUtil::exist(_net_orien_wire_demand_map, net_idx)) {
          if (RTUtil::exist(_net_orien_wire_demand_map[net_idx], wire_orien)) {
            net_wire_demand = _net_orien_wire_demand_map[net_idx][wire_orien];
          }
        }
        _resource_demand += (update_num * net_wire_demand);
      }
    }
    if (!via_orien_set.empty()) {
      /**
       * 加via_demand
       */
      irt_int net_via_demand = _whole_via_demand;
      if (RTUtil::exist(_net_via_demand_map, net_idx)) {
        net_via_demand = _net_via_demand_map[net_idx];
      }
      _resource_demand += (update_num * net_via_demand);
    }
    if (change_type == ChangeType::kAdd) {
      _passed_net_set.insert(net_idx);
    } else if (change_type == ChangeType::kDel) {
      _passed_net_set.erase(net_idx);
    }
  }
#if 1  // astar
  // single net
  std::set<Direction>& get_direction_set() { return _direction_set; }
  void set_direction_set(std::set<Direction>& direction_set) { _direction_set = direction_set; }
  // single path
  GRNodeState& get_state() { return _state; }
  GRNode* get_parent_node() const { return _parent_node; }
  double get_known_cost() const { return _known_cost; }
  double get_estimated_cost() const { return _estimated_cost; }
  void set_state(GRNodeState state) { _state = state; }
  void set_parent_node(GRNode* parent_node) { _parent_node = parent_node; }
  void set_known_cost(const double known_cost) { _known_cost = known_cost; }
  void set_estimated_cost(const double estimated_cost) { _estimated_cost = estimated_cost; }
  // function
  bool isNone() { return _state == GRNodeState::kNone; }
  bool isOpen() { return _state == GRNodeState::kOpen; }
  bool isClose() { return _state == GRNodeState::kClose; }
  double getTotalCost() { return (_known_cost + _estimated_cost); }
#endif

 private:
  GRNodeId _gr_node_id;
  PlanarRect _base_region;
  std::map<Orientation, GRNode*> _neighbor_ptr_map;
  std::map<GRSourceType, RegionQuery> _source_region_query_map;
  /**
   * gcell 布线结果该算多少demand?
   *
   * wire消耗(单个方向)
   *  默认：(_whole_wire_demand/2)
   *  线网方向自定义：(_net_orien_wire_demand_map)
   *
   * via消耗(无方向差别)
   *  默认：(_whole_via_demand)
   *  线网方向自定义：(_net_via_demand_map)
   *
   * access消耗(单个方向)
   *  默认：(_whole_access_demand)
   *  线网方向自定义：(_net_orien_access_demand_map)
   *
   */
  irt_int _whole_wire_demand = 0;
  irt_int _whole_via_demand = 0;
  std::map<irt_int, std::map<Orientation, irt_int>> _net_orien_wire_demand_map;
  std::map<irt_int, irt_int> _net_via_demand_map;
  irt_int _whole_access_demand = 1;
  std::map<irt_int, std::map<Orientation, irt_int>> _net_orien_access_demand_map;
  /**
   * gcell 入口控制
   *
   * _orien_access_supply_map 能使用的方向入口数
   * _orien_access_demand_map 已使用的方向入口数
   */
  std::map<Orientation, irt_int> _orien_access_supply_map;
  std::map<Orientation, irt_int> _orien_access_demand_map;
  /**
   * gcell 集成资源
   *
   * _resource_supply 能使用的供给
   * _resource_demand 已使用的需求
   */
  irt_int _resource_supply = 0;
  irt_int _resource_demand = 0;
  /**
   * gcell 历史代价
   */
  std::map<Orientation, double> _history_orien_access_cost_map;
  double _history_resource_cost = 0.0;
  /**
   * gcell 从此node经过的线网
   */
  std::set<irt_int> _passed_net_set;
#if 1  // astar
  // single net
  std::set<Direction> _direction_set;
  // single path
  GRNodeState _state = GRNodeState::kNone;
  GRNode* _parent_node = nullptr;
  double _known_cost = 0.0;  // include curr
  double _estimated_cost = 0.0;
#endif
};

#if 1  // astar
struct CmpGRNodeCost
{
  bool operator()(GRNode* a, GRNode* b)
  {
    if (RTUtil::equalDoubleByError(a->getTotalCost(), b->getTotalCost(), DBL_ERROR)) {
      if (RTUtil::equalDoubleByError(a->get_estimated_cost(), b->get_estimated_cost(), DBL_ERROR)) {
        return a->get_neighbor_ptr_map().size() < b->get_neighbor_ptr_map().size();
      } else {
        return a->get_estimated_cost() > b->get_estimated_cost();
      }
    } else {
      return a->getTotalCost() > b->getTotalCost();
    }
  }
};
#endif

}  // namespace irt

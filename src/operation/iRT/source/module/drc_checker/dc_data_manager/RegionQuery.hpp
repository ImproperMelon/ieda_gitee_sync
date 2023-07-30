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

#include "../../../../database/interaction/ids.hpp"
#include "LayerRect.hpp"
#include "RQShape.hpp"
#include "RTU.hpp"

namespace irt {

using StaticBox = std::pair<BoostBox, RQShape*>;

class RegionQuery
{
 public:
  RegionQuery() {}
  ~RegionQuery() { destroy(); }
  // getter
  void* get_idrc_region_query() { return _idrc_region_query; }
  std::map<irt_int, std::map<irt_int, std::set<LayerRect, CmpLayerRectByXASC>>>& get_routing_net_rect_map()
  {
    return _routing_net_rect_map;
  }
  std::map<irt_int, std::map<irt_int, std::set<LayerRect, CmpLayerRectByXASC>>>& get_cut_net_rect_map() { return _cut_net_rect_map; }
  std::map<irt_int, std::map<irt_int, std::map<LayerRect, RQShape*, CmpLayerRectByLayerASC>>>& get_routing_net_shape_map()
  {
    return _routing_net_shape_map;
  }
  std::map<irt_int, std::map<irt_int, std::map<LayerRect, RQShape*, CmpLayerRectByLayerASC>>>& get_cut_net_shape_map()
  {
    return _cut_net_shape_map;
  }
  std::map<irt_int, bgi::rtree<std::pair<BoostBox, RQShape*>, bgi::quadratic<16>>>& get_routing_region_map() { return _routing_region_map; }
  std::map<irt_int, bgi::rtree<std::pair<BoostBox, RQShape*>, bgi::quadratic<16>>>& get_cut_region_map() { return _cut_region_map; }
  // setters
  void set_idrc_region_query(void* idrc_region_query) { _idrc_region_query = idrc_region_query; }
  // function
  void destroy()
  {
    for (auto& [net_id, layer_shape_map] : _routing_net_shape_map) {
      for (auto& [layer_idx, shape_map] : layer_shape_map) {
        for (auto& [rect, shape_ptr] : shape_map) {
          delete shape_ptr;
          shape_ptr = nullptr;
        }
      }
    }
    for (auto& [net_id, layer_shape_map] : _cut_net_shape_map) {
      for (auto& [layer_idx, shape_map] : layer_shape_map) {
        for (auto& [rect, shape_ptr] : shape_map) {
          delete shape_ptr;
          shape_ptr = nullptr;
        }
      }
    }
  }

 private:
  void* _idrc_region_query = nullptr;
  std::map<irt_int, std::map<irt_int, std::set<LayerRect, CmpLayerRectByXASC>>> _routing_net_rect_map;                 // layer-net-rect
  std::map<irt_int, std::map<irt_int, std::set<LayerRect, CmpLayerRectByXASC>>> _cut_net_rect_map;                     // layer-net-rect
  std::map<irt_int, std::map<irt_int, std::map<LayerRect, RQShape*, CmpLayerRectByLayerASC>>> _routing_net_shape_map;  // net-layer-rect
  std::map<irt_int, std::map<irt_int, std::map<LayerRect, RQShape*, CmpLayerRectByLayerASC>>> _cut_net_shape_map;      // net-layer-rect
  std::map<irt_int, bgi::rtree<std::pair<BoostBox, RQShape*>, bgi::quadratic<16>>> _routing_region_map;
  std::map<irt_int, bgi::rtree<std::pair<BoostBox, RQShape*>, bgi::quadratic<16>>> _cut_region_map;
};

}  // namespace irt

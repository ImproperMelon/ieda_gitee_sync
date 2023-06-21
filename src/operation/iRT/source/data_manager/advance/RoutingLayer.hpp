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

#include "Direction.hpp"
#include "Logger.hpp"
#include "PlanarRect.hpp"
#include "SpacingTable.hpp"
#include "TrackAxis.hpp"

namespace irt {

class RoutingLayer
{
 public:
  RoutingLayer() = default;
  ~RoutingLayer() = default;
  // getter
  irt_int get_layer_idx() const { return _layer_idx; }
  irt_int get_layer_order() const { return _layer_order; }
  std::string& get_layer_name() { return _layer_name; }
  irt_int get_min_width() const { return _min_width; }
  irt_int get_min_area() const { return _min_area; }
  Direction& get_direction() { return _direction; }
  TrackAxis& get_track_axis() { return _track_axis; }
  SpacingTable& get_spacing_table() { return _spacing_table; }
  // setter
  void set_layer_idx(const irt_int layer_idx) { _layer_idx = layer_idx; }
  void set_layer_order(const irt_int layer_order) { _layer_order = layer_order; }
  void set_layer_name(const std::string& layer_name) { _layer_name = layer_name; }
  void set_min_width(const irt_int min_width) { _min_width = min_width; }
  void set_min_area(const irt_int min_area) { _min_area = min_area; }
  void set_direction(const Direction& direction) { _direction = direction; }
  void set_track_axis(const TrackAxis& track_axis) { _track_axis = track_axis; }
  void set_spacing_table(const SpacingTable& spacing_table) { _spacing_table = spacing_table; }

  // function
  bool isPreferH() const { return _direction == Direction::kHorizontal; }
  TrackGrid& getXTrackGrid() { return _track_axis.get_x_grid_list().front(); }
  TrackGrid& getYTrackGrid() { return _track_axis.get_y_grid_list().front(); }
  TrackGrid& getPreferTrackGrid() { return isPreferH() ? getYTrackGrid() : getXTrackGrid(); }
  TrackGrid& getNonpreferTrackGrid() { return isPreferH() ? getXTrackGrid() : getYTrackGrid(); }
  std::vector<TrackGrid>& getXTrackGridList() { return _track_axis.get_x_grid_list(); }
  std::vector<TrackGrid>& getYTrackGridList() { return _track_axis.get_y_grid_list(); }
  std::vector<TrackGrid>& getPreferTrackGridList() { return isPreferH() ? getYTrackGridList() : getXTrackGridList(); }
  std::vector<TrackGrid>& getNonpreferTrackGridList() { return isPreferH() ? getXTrackGridList() : getYTrackGridList(); }
  irt_int getMinSpacing(const PlanarRect& rect)
  {
    std::vector<irt_int>& width_list = _spacing_table.get_width_list();
    GridMap<irt_int>& width_parallel_length_map = _spacing_table.get_width_parallel_length_map();

    for (size_t i = 0; (i + 1) < width_list.size(); i++) {
      if (width_list[i] <= rect.getWidth() && rect.getWidth() < width_list[i + 1]) {
        return width_parallel_length_map[i][0];
      }
    }
    return width_parallel_length_map[width_parallel_length_map.get_x_size() - 1][0];
  }

 private:
  irt_int _layer_idx = -1;
  irt_int _layer_order = -1;
  std::string _layer_name;
  irt_int _min_width = 0;
  irt_int _min_area = 0;
  Direction _direction = Direction::kNone;
  TrackAxis _track_axis;
  SpacingTable _spacing_table;
};

}  // namespace irt

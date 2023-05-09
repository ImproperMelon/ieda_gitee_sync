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

#include <string>

#include "EGRStrategy.hpp"

namespace irt {

class EGRConfig
{
 public:
  EGRConfig() = default;
  ~EGRConfig() = default;

  std::string temp_directory_path;
  irt_int thread_number;
  std::string log_file_path;
  irt_int accuracy;
  irt_int congestion_cell_x_pitch;
  irt_int congestion_cell_y_pitch;
  std::string bottom_routing_layer;
  std::string top_routing_layer;
  std::vector<std::string> skip_net_name_list;
  std::string strategy;

  irt_int cell_width = -1;
  irt_int cell_height = -1;
  irt_int bottom_routing_layer_idx = -1;
  irt_int top_routing_layer_idx = -1;
  std::set<std::string> skip_net_name_set;
  EGRStrategy egr_strategy;
};

}  // namespace irt

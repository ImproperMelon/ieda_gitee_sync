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

#include "RTU.hpp"

namespace irt {

class TAPanelStat
{
 public:
  TAPanelStat() = default;
  ~TAPanelStat() = default;
  // getter
  double get_total_wire_length() { return _total_wire_length; }
  std::map<std::string, irt_int>& get_drc_type_num_map() { return _drc_type_num_map; }
  irt_int get_total_drc_number() { return _total_drc_number; }
  // setter
  void set_total_wire_length(const double total_wire_length) { _total_wire_length = total_wire_length; }
  void set_total_drc_number(const double total_drc_number) { _total_drc_number = total_drc_number; }
  // function

 private:
  double _total_wire_length = 0;
  std::map<std::string, irt_int> _drc_type_num_map;
  irt_int _total_drc_number = 0;
};

}  // namespace irt

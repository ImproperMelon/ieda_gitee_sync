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

#include "EXTLayerCoord.hpp"

namespace irt {

class ViaNode : public PlanarCoord
{
 public:
  ViaNode() = default;
  ViaNode(const ViaNode& other) : PlanarCoord(other)
  {
    _net_idx = other._net_idx;
    _via_master_idx = other._via_master_idx;
  }
  ~ViaNode() = default;
  // getter
  irt_int get_net_idx() const { return _net_idx; }
  ViaMasterIdx& get_via_master_idx() { return _via_master_idx; }
  // setter
  void set_net_idx(const irt_int net_idx) { _net_idx = net_idx; }
  void set_via_master_idx(const ViaMasterIdx& via_master_idx) { _via_master_idx = via_master_idx; }
  void set_via_master_idx(const irt_int below_layer_idx, const irt_int via_idx)
  {
    _via_master_idx.set_below_layer_idx(below_layer_idx);
    _via_master_idx.set_via_idx(via_idx);
  }
  // function

 private:
  irt_int _net_idx = -1;
  ViaMasterIdx _via_master_idx;
};

}  // namespace irt

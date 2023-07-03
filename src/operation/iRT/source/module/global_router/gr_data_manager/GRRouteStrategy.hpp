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

#include "Logger.hpp"

namespace irt {

enum class GRRouteStrategy
{
  kNone = 0,
  kFullyConsider = 1,
  kEnlarging = 2,
  kIgnoringENV = 3,
  kIgnoringOBS = 4
};

struct GetGRRouteStrategyName
{
  std::string operator()(const GRRouteStrategy& gr_route_strategy) const
  {
    std::string gr_route_strategy_name;
    switch (gr_route_strategy) {
      case GRRouteStrategy::kNone:
        gr_route_strategy_name = "none";
        break;
      case GRRouteStrategy::kFullyConsider:
        gr_route_strategy_name = "fully_consider";
        break;
      case GRRouteStrategy::kEnlarging:
        gr_route_strategy_name = "enlarging";
        break;
      case GRRouteStrategy::kIgnoringENV:
        gr_route_strategy_name = "ignoring_env";
        break;
      case GRRouteStrategy::kIgnoringOBS:
        gr_route_strategy_name = "ignoring_obs";
        break;
      default:
        LOG_INST.error(Loc::current(), "Unrecognized type!");
        break;
    }
    return gr_route_strategy_name;
  }
};

}  // namespace irt

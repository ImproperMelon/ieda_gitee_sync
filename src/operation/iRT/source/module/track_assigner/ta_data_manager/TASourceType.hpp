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

enum class TASourceType
{
  kNone,
  kBlockAndPin,
  kEnclosure,
  kPanelResult
};

struct GetTASourceTypeName
{
  std::string operator()(const TASourceType& ta_source_type) const
  {
    std::string ta_source_type_name;
    switch (ta_source_type) {
      case TASourceType::kNone:
        ta_source_type_name = "none";
        break;
      case TASourceType::kBlockAndPin:
        ta_source_type_name = "block_and_pin";
        break;
      case TASourceType::kEnclosure:
        ta_source_type_name = "enclosure";
        break;
      case TASourceType::kPanelResult:
        ta_source_type_name = "panel_result";
        break;
      default:
        LOG_INST.error(Loc::current(), "Unrecognized type!");
        break;
    }
    return ta_source_type_name;
  }
};

}  // namespace irt

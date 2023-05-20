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
#include <vector>

#include "BStarTree.hh"
#include "SequencePair.hh"
#include "Setting.hh"
#include "database/FPInst.hh"

namespace ipl::imp {

class SolutionFactory
{
 public:
  MPSolution* createSolution(std::vector<FPInst*> macro_list, Setting* set)
  {
    switch (set->get_solution_type()) {
      case SolutionTYPE::kBStar_tree:
        return new BStarTree(macro_list, set);
        break;
      case SolutionTYPE::kSequence_pair:
        return new SequencePair(macro_list, set);
        break;
      default:
        break;
    }
    return nullptr;
  }
};
}  // namespace ipl::imp
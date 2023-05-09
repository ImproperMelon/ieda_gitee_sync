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
/**
 * @file CmdReportPower.cc
 * @author shaozheqing (707005020@qq.com)
 * @brief cmd to report power.
 * @version 0.1
 * @date 2023-05-04
 */

#include "PowerShellCmd.hh"
#include "sta/Sta.hh"

namespace ipower {

CmdReportPower::CmdReportPower(const char* cmd_name) : TclCmd(cmd_name) {}

unsigned CmdReportPower::check() { return 1; }

unsigned CmdReportPower::exec() {
  if (!check()) {
    return 0;
  }

  Sta* ista = Sta::getOrCreateSta();
  Power* ipower = Power::getOrCreatePower(&(ista->get_graph()));

  // set fastest clock for default toggle.
  auto* fastest_clock = ista->getFastestClock();
  PwrClock pwr_fastest_clock(fastest_clock->get_clock_name(),
                             fastest_clock->getPeriodNs());
  // get sta clocks
  auto clocks = ista->getClocks();

  ipower->setupClock(std::move(pwr_fastest_clock), std::move(clocks));

  // build power graph.
  ipower->buildGraph();

  // build seq graph
  ipower->buildSeqGraph();

  // update power.
  ipower->updatePower();

  // report power.
  // TODO add arg
  std::string output_path = ista->get_design_work_space();
  output_path += Str::printf("/%s.pwr", ista->get_design_name().c_str());

  ipower->reportPower(output_path.c_str(), PwrAnalysisMode::kAveraged);

  return 1;
}

}  // namespace ipower
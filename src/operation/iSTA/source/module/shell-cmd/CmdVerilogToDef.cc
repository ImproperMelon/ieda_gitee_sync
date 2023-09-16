// ***************************************************************************************
// Copyright (c) 2023-2025 Peng Cheng Laboratory
// Copyright (c) 2023-2025 Institute of Computing Technology, Chinese Academy of
// Sciences Copyright (c) 2023-2025 Beijing Institute of Open Source Chip
//
// iEDA is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
// http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.
// ***************************************************************************************
/**
 * @file CmdVerilogToDef.cc
 * @author simin tao (taosm@pcl.ac.cn)
 * @brief The implemention of verilog to def command.
 * @version 0.1
 * @date 2023-09-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "ShellCmd.hh"
#include "builder.h"

namespace ista {

CmdVerilogToDef::CmdVerilogToDef(const char* cmd_name) : TclCmd(cmd_name) {
  auto* def_option = new TclStringOption("-def", 0, nullptr);
  addOption(def_option);

  auto* lef_option = new TclStringListOption("-lef", 0);
  addOption(lef_option);

  auto* verilog_option = new TclStringOption("-verilog", 0, nullptr);
  addOption(verilog_option);

  auto* top_option = new TclStringOption("-top", 0, nullptr);
  addOption(top_option);
}

unsigned CmdVerilogToDef::check() { return 1; }

unsigned CmdVerilogToDef::exec() {
  if (!check()) {
    return 0;
  }

  auto* db_builder = new idb::IdbBuilder();

  TclOption* lef_option = getOptionOrArg("-lef");
  auto lef_files = lef_option->getStringList();

  db_builder->buildLef(lef_files);

  TclOption* verilog_option = getOptionOrArg("-verilog");
  auto* verilog_file = verilog_option->getStringVal();

  TclOption* top_option = getOptionOrArg("-top");
  auto* top = top_option->getStringVal();

  std::set<std::string> exclude_cell_names;
  db_builder->buildVerilog(verilog_file, top);

  TclOption* def_option = getOptionOrArg("-def");
  auto* def_file = def_option->getStringVal();
  db_builder->saveDef(def_file);

  return 1;
}

}  // namespace ista

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
/**
 * @File Name: tcl_contest.h
 * @Brief :
 * @Author : Yell (12112088@qq.com)
 * @Version : 1.0
 * @Creat Date : 2023-09-15
 *
 */
#include <iostream>
#include <string>

#include "./../tcl_definition.h"
#include "ScriptEngine.hh"

using ieda::TclCmd;
using ieda::TclOption;
using ieda::TclStringOption;

namespace tcl {

class CmdRunContest : public TclCmd
{
 public:
  explicit CmdRunContest(const char* cmd_name);
  ~CmdRunContest() override = default;

  unsigned check() override;
  unsigned exec() override;

 private:
  // private function
  // private data
};

class CmdRunContestEvaluation : public TclCmd
{
 public:
  explicit CmdRunContestEvaluation(const char* cmd_name);
  ~CmdRunContestEvaluation() override = default;

  unsigned check() override;
  unsigned exec() override;

 private:
  // private function
  // private data
};

}  // namespace tcl
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
#ifndef SRC_EVALUATION_API_EVALAPI_HPP_
#define SRC_EVALUATION_API_EVALAPI_HPP_

#include <string>
#include <vector>

#include "CongestionEval.hpp"
#include "TimingEval.hpp"
#include "ids.hpp"

namespace eval {

#define EvalInst (eval::EvalAPI::getInst())

using std::string;
using std::vector;

class EvalAPI
{
 public:
  static EvalAPI& initInst();
  static EvalAPI& getInst();
  static void destroyInst();

  /****************************** Wirelength Eval: START ******************************/
  int64_t evalTotalWL(const string& wl_type, const vector<WLNet*>& net_list);
  int64_t evalOneNetWL(const string& wl_type, WLNet* wl_net);
  int64_t evalDriver2LoadWL(WLNet* wl_net, const string& sink_pin_name);
  double evalEGRWL();
  void reportWirelength(const string& plot_path, const string& output_file_name, const vector<WLNet*>& net_list);
  /****************************** Wirelength Eval: END *******************************/

  /****************************** Congestion Eval: START ******************************/
  void initCongestionEval(CongGrid* grid, const vector<CongInst*>& inst_list, const vector<CongNet*>& net_list);
  void initCongestionEval();
  vector<float> evalPinDens();
  vector<float> evalPinDens(CongGrid* grid, const vector<CongInst*>& inst_list);
  vector<float> evalInstDens();
  vector<float> evalInstDens(CongGrid* grid, const vector<CongInst*>& inst_list);
  vector<float> evalNetCong(const string& rudy_type);
  vector<float> evalNetCong(CongGrid* grid, const vector<CongNet*>& net_list, const string& rudy_type);
  vector<float> evalGRCong();
  vector<float> getUseCapRatioList();
  vector<int> getTileGridCoordSizeCntXY();
  void plotPinDens(const string& plot_path, const string& output_file_name, CongGrid* grid, const vector<CongInst*>& inst_list);
  void plotInstDens(const string& plot_path, const string& output_file_name, CongGrid* grid, const vector<CongInst*>& inst_list);
  void plotNetCong(const string& plot_path, const string& output_file_name, CongGrid* grid, const vector<CongNet*>& net_list,
                   const string& rudy_type);
  void plotGRCong(const string& plot_path, const string& output_file_name);
  void plotOverflow(const string& plot_path, const string& output_file_name);
  void reportCongestion(const string& plot_path, const string& output_file_name, const vector<CongNet*>& net_list, CongGrid* grid,
                        const vector<CongInst*>& inst_list);
  /****************************** Congestion Eval: END *******************************/

  /****************************** Timing Eval: START ******************************/
  void initTimingEval(idb::IdbBuilder* idb_builder, const char* sta_workspace_path, vector<const char*> lib_file_path_list,
                      const char* sdc_file_path);
  void initTimingEval(int32_t unit);
  double getEarlySlack(const string& pin_name) const;
  double getLateSlack(const string& pin_name) const;
  double getArrivalEarlyTime(const string& pin_name) const;
  double getArrivalLateTime(const string& pin_name) const;
  double getRequiredEarlyTime(const string& pin_name) const;
  double getRequiredLateTime(const string& pin_name) const;
  double reportWNS(const char* clock_name, ista::AnalysisMode mode);
  double reportTNS(const char* clock_name, ista::AnalysisMode mode);
  void updateTiming(const vector<TimingNet*>& timing_net_list);
  void updateTiming(const vector<TimingNet*>& timing_net_list, const vector<string>& name_list, const int& propagation_level);
  void destroyTimingEval();
  /****************************** Timing Eval: END *******************************/

  /****************************** GDS Wrapper: START ******************************/
  vector<GDSNet*>& wrapGDSNetlist(const string& eval_json);
  /****************************** GDS Wrapper: END ********************************/

 private:
  static EvalAPI* _eval_api_inst;
  TimingEval* _timing_eval_inst = nullptr;
  CongestionEval* _congestion_eval_inst = nullptr;

  EvalAPI() = default;
  EvalAPI(const EvalAPI& other) = delete;
  EvalAPI(EvalAPI&& other) = delete;
  ~EvalAPI() = default;
  EvalAPI& operator=(const EvalAPI& other) = delete;
  EvalAPI& operator=(EvalAPI&& other) = delete;
};

}  // namespace eval

#endif // SRC_EVALUATION_API_EVALAPI_HPP_

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

#include "Config.hpp"
#include "DataManager.hpp"
#include "Database.hpp"
#include "Net.hpp"
#include "PAModel.hpp"
#include "PANode.hpp"

namespace irt {

#define PA_INST (irt::PinAccessor::getInst())

class PinAccessor
{
 public:
  static void initInst();
  static PinAccessor& getInst();
  static void destroyInst();
  // function
  void access(std::vector<Net>& net_list);

 private:
  // self
  static PinAccessor* _pa_instance;

  PinAccessor() = default;
  PinAccessor(const PinAccessor& other) = delete;
  PinAccessor(PinAccessor&& other) = delete;
  ~PinAccessor() = default;
  PinAccessor& operator=(const PinAccessor& other) = delete;
  PinAccessor& operator=(PinAccessor&& other) = delete;
  // function
  void accessNetList(std::vector<Net>& net_list);

#if 1  // build pa_model
  PAModel initPAModel(std::vector<Net>& net_list);
  std::vector<PANet> convertToPANetList(std::vector<Net>& net_list);
  PANet convertToPANet(Net& net);
  void buildPAModel(PAModel& pa_model);
  void initGCellRealRect(PAModel& pa_model);
  void updateNetBlockageMap(PAModel& pa_model);
  void cutBlockageList(PAModel& pa_model);
#endif

#if 1  // access pa_model
  void accessPAModel(PAModel& pa_model);
  void accessPANetList(PAModel& pa_model);
  void accessPANet(PAModel& pa_model, PANet& pa_net);
  void initAccessPointList(PAModel& pa_model, PANet& pa_net);
  std::vector<LayerRect> getLegalPinShapeList(PAModel& pa_model, irt_int pa_net_idx, PAPin& pa_pin);
  std::vector<PlanarRect> getViaLegalRectList(PAModel& pa_model, irt_int pa_net_idx, irt_int via_below_layer_idx,
                                              std::vector<EXTLayerRect>& pin_shape_list);
  void mergeAccessPointList(PANet& pa_net);
  void selectAccessPointList(PANet& pa_net);
  void selectAccessPointType(PANet& pa_net);
  void buildBoundingBox(PANet& pa_net);
  void buildAccessPointList(PANet& pa_net);
  void selectGCellAccessPoint(PANet& pa_net);
  void eliminateDRCViolation(PAModel& pa_model, PANet& pa_net);
  void updateNetEnclosureMap(PAModel& pa_model);
  void eliminateConflict(PAModel& pa_model);
  void checkConflict(PANet& pa_net, PAModel& pa_model);
  void selectDistance(PANet& pa_net);
#endif

#if 1  // update pa_model
  void updatePAModel(PAModel& pa_model);
  void buildDrivingPin(PANet& pa_net);
  void updateOriginPAResult(PAModel& pa_model);
#endif

#if 1  // check pa_model
  void checkPAModel(PAModel& pa_model);
#endif

#if 1  // report pa_model
  void countPAModel(PAModel& pa_model);
  void reportPAModel(PAModel& pa_model);
#endif
};

}  // namespace irt

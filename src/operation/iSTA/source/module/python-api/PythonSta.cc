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
 * @file PythonSta.cc
 * @author simin tao (taosm@pcl.ac.cn)
 * @brief The implemention of python interface.
 * @version 0.1
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "PythonSta.hh"

namespace ista {

PYBIND11_MODULE(ista_cpp, m) {
  m.def("set_design_workspace", set_design_workspace, ("design_workspace"));
  m.def("read_netlist", read_netlist, ("file_name"));
  m.def("read_liberty", read_liberty, ("file_name"));
  m.def("link_design", link_design, ("cell_name"));
  m.def("read_spef", read_spef, ("file_name"));
  m.def("read_sdc", read_sdc, py::arg("file_name"));
  m.def("report_timing", report_timing);
}

}  // namespace ista

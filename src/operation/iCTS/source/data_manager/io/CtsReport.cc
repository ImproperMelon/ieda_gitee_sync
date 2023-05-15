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
#include "CtsReport.h"

namespace icts {

std::unique_ptr<CtsReportTable> CtsReportTable::createReportTable(const std::string& tbl_name, const CtsReportType& type)
{
  auto report_tbl = std::make_unique<CtsReportTable>(tbl_name);

  switch (type) {
    case CtsReportType::kWIRE_LENGTH:
      (*report_tbl) << TABLE_HEAD;
      (*report_tbl)[0][0] = "Type";
      (*report_tbl)[0][1] = "Wire Length";
      (*report_tbl) << TABLE_ENDLINE;
      break;
    case CtsReportType::kHP_WIRE_LENGTH:
      (*report_tbl) << TABLE_HEAD;
      (*report_tbl)[0][0] = "Type";
      (*report_tbl)[0][1] = "HP Wire Length";
      (*report_tbl) << TABLE_ENDLINE;
      break;
    case CtsReportType::kCELL_STATS:
      (*report_tbl) << TABLE_HEAD;
      (*report_tbl)[0][0] = "Cell type";
      (*report_tbl)[0][1] = "Count";
      (*report_tbl)[0][2] = "Area";
      (*report_tbl)[0][3] = "Capacitance";
      (*report_tbl) << TABLE_ENDLINE;
      break;
    case CtsReportType::kLIB_CELL_DIST:
      (*report_tbl) << TABLE_HEAD;
      (*report_tbl)[0][0] = "Name";
      (*report_tbl)[0][1] = "Type";
      (*report_tbl)[0][2] = "Inst\nCount";
      (*report_tbl)[0][3] = "Inst Area\n(um^2)";
      (*report_tbl) << TABLE_ENDLINE;
      break;
    case CtsReportType::kNET_LEVEL:
      (*report_tbl) << TABLE_HEAD;
      (*report_tbl)[0][0] = "Level";
      (*report_tbl)[0][1] = "Num";
      (*report_tbl)[0][2] = "Ratio";
      (*report_tbl) << TABLE_ENDLINE;
      break;
    case CtsReportType::kTIMING_NODE_LOG:
      (*report_tbl) << TABLE_HEAD;
      (*report_tbl)[0][0] = "ID";
      (*report_tbl)[0][1] = "Name";
      (*report_tbl)[0][2] = "Net Length";
      (*report_tbl)[0][3] = "Net Snake";
      (*report_tbl)[0][4] = "Total Net Length";
      (*report_tbl)[0][5] = "Location";
      (*report_tbl)[0][6] = "Min Delay";
      (*report_tbl)[0][7] = "Max Delay";
      (*report_tbl)[0][8] = "Insertion Type";
      (*report_tbl)[0][9] = "Slew In";
      (*report_tbl)[0][10] = "Cap Out";
      (*report_tbl)[0][11] = "Insertion Delay";
      (*report_tbl) << TABLE_ENDLINE;
      break;
    case CtsReportType::kHCTS_LOG:
      (*report_tbl) << TABLE_HEAD;
      (*report_tbl)[0][0] = "ID";
      (*report_tbl)[0][1] = "Name";
      (*report_tbl)[0][2] = "Net Length";
      (*report_tbl)[0][3] = "Location";
      (*report_tbl)[0][4] = "Fanout";
      (*report_tbl)[0][5] = "Delay";
      (*report_tbl)[0][6] = "Slew In";
      (*report_tbl)[0][7] = "Cap Out";
      (*report_tbl)[0][8] = "Insertion Delay";
      (*report_tbl)[0][9] = "Sub Total Cap [T]";
      (*report_tbl)[0][10] = "Level [T]";
      (*report_tbl) << TABLE_ENDLINE;
      break;
    default:
      break;
  }
  return report_tbl;
}
}  // namespace icts
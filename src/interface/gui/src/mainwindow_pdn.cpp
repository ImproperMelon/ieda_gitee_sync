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
#include <QPixmap>

#include "gui_io.h"
#include "mainwindow.h"

void MainWindow::createGuiPdn() {
  createMenuPdn();
  createToolbarPdn();
}

void MainWindow::createMenuPdn() {
  QMenu* pdnMenu = menuBar()->addMenu(tr("PDN"));
  pdnMenu->addAction(tr("Create Power Nets"), this, &MainWindow::PdnCreateNet);
  pdnMenu->addAction(tr("Connect IO"), this, &MainWindow::PdnConnectIO);

  pdnMenu->addSeparator();

  pdnMenu->addAction(tr("Place Ports"), this, &MainWindow::PdnPlacePorts);

  pdnMenu->addSeparator();

  QMenu* addStripeMenu = pdnMenu->addMenu(tr("Add Stripe"));
  addStripeMenu->addAction(tr("Add Stripe to Standard Cell"), this, &MainWindow::PdnAddStdCellStripe);
  addStripeMenu->addAction(tr("Add Stripe Grid"), this, &MainWindow::PdnAddStripeGrid);

  pdnMenu->addSeparator();

  pdnMenu->addAction(tr("Connect Power Nets"), this, &MainWindow::PdnConnectAllLayers);

}

  void MainWindow::createToolbarPdn(){}

  void MainWindow::PdnCreateNet(){}

  void MainWindow::PdnConnectIO(){}

  void MainWindow::PdnPlacePorts(){}

  void MainWindow::PdnAddStdCellStripe(){}

  void MainWindow::PdnAddStripeGrid(){}

  void MainWindow::PdnConnectAllLayers(){}


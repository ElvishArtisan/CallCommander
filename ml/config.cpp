// config.cpp
//
// A Qt-based application for controlling broadcast telephone interfaces.
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <stdlib.h>

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtCore/QFile>

#include <sys/types.h>
#include <sys/stat.h>

#include <askbox.h>
#include <list_connections.h>
#include <ml.h>

#include "defaults.h"
#include "globals.h"
#include "mithlond.h"

void MainWidget::LoadConfig()
{
  bool ask_for_profile=true;
  QString filename;

  //
  // Load the Configuration
  //
  ml_config=new MlConfig(GetConfFile(qApp->argc(),qApp->argv()));
  ml_config->load();

  //
  // Try to load the specified connection profile
  //
  if(!ml_config->connectionName().isEmpty()) {
    telos_connection->setName(ml_config->connectionName());
    ask_for_profile=!telos_connection->load();
  }

  //
  // Connection Dialog
  //
  if(ask_for_profile) {
    telos_connection->setOfferStartConnection(true);
    ListConnections *list=
      new ListConnections(telos_connection,tr("Open\nConnection"),
			  ml_config,this);
    if(list->exec()<0) {
      exit(0);
    }
  }
}


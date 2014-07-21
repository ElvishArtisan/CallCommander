// mcs.h
//
// A Qt-based application for automatic call recording
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef MCS_H
#define MCS_H

#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtCore/QTimer>
#include <QtGui/QPixmap>

#include <connection_data.h>
#include <telos2101_driver.h>
#include <telos100_driver.h>
#include <ts612_driver.h>
#include <virtual_driver.h>
#include <asterisk_driver.h>
#include <mskimmer_widget.h>

#define DEFAULT_CONFIG_FILE "/etc/ml.conf"

class MainWidget : public QWidget
{
  Q_OBJECT
 public:
  MainWidget(QWidget *parent=0,const char *name=0);
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void connectedData(unsigned,bool);
  void closeData();

 private:
  McsWidget *main_widget;
  McsWidget *conf_widget;
  BusDriver *mcs_driver;
  ConnectionData *mcs_connection;
  QString mcs_config;
  QPixmap *mcs_callcommander_map;
};


#endif 


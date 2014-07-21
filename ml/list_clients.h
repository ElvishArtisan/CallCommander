// list_clients.h
//
// A Client List Dialog for CallCommander
//
//   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef LIST_CLIENTS_H
#define LIST_CLIENTS_H

#include <vector>

#include <QtGui/QDialog>
#include <QtGui/QPixmap>
#include <QtGui/QResizeEvent>
#include <QtGui/QShowEvent>

#include <bus_driver.h>
#include <client_data.h>

#include "client_listview.h"

class ListClients : public QDialog
{
  Q_OBJECT
 public:
  ListClients(BusDriver *driver,QWidget *parent=0,const char *name=0);
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 protected:
  void resizeEvent(QResizeEvent *e);
  void showEvent(QShowEvent *e);

 private slots:
  void clientListChangedData(unsigned id,std::vector <ClientData *> *list);
  void populateListData();

 private:
  BusDriver *list_driver;
  ClientListView *list_client_list;
  int list_width;
  QPixmap *list_client_map;
  QPixmap *list_client_segment_map;
  QPixmap *list_logic_map;
  QPixmap *list_monitor_map;
  QPixmap *list_recorder_map;
};


#endif


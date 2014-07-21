// list_connections.h
//
// List CallCommander Connections
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

#ifndef LIST_CONNECTIONS_H
#define LIST_CONNECTIONS_H

#include <QtGui/QDialog>
#include <Qt3Support/q3listview.h>
#include <QtGui/QPushButton>

#include <connection_data.h>
#include <mlconfig.h>

#include <edit_connection.h>
#include <list_shows.h>
#include <list_consoles.h>
#include <list_devices.h>

class ListConnections : public QDialog
{
  Q_OBJECT
 public:
  ListConnections(ConnectionData *data,const QString &ok_text,
		  MlConfig *config,QWidget *parent=0,const char *name=0);
  ~ListConnections();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void newData();
  void editData();
  void deleteData();
  void connectData();
  void cancelData();
  void listClickedData(Q3ListViewItem *item);
  void listDoubleclickedData(Q3ListViewItem *item,const QPoint &pt,int col);
  
 private:
  void RefreshList();
  Q3ListView *list_view;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
  QPushButton *list_connect_button;
  ConnectionData *list_data;
  MlConfig *list_config;
};


#endif  // LIST_CONNECTIONS_H

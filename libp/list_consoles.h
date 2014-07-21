// list_consoles.h
//
// List CallCommander Console Configurations
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

#ifndef LIST_CONSOLES_H
#define LIST_CONSOLES_H

#include <vector>

#include <QtGui/QDialog>
#include <Qt3Support/q3listview.h>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>

#include <connection_data.h>
#include <telos2101_driver.h>

class ListConsoles : public QDialog
{
  Q_OBJECT
 public:
  ListConsoles(ConnectionData *conn,BusDriver *driver,
	       QWidget *parent=0,const char *name=0);
  ~ListConsoles();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void scanDeviceData();
  void busErrorData(unsigned id,BusDriver::Error err);
  void busConsoleListData(unsigned id,
			  std::vector<ConsoleData *> *console_list);
  void listClickedData(Q3ListViewItem *item);
  void listDoubleclickedData(Q3ListViewItem *item,const QPoint &pt,int col);
  void okData();
  void cancelData();

 private:
  ConnectionData *list_connection;
  QLabel *list_question_label;
  Q3ListView *list_console_view;
  QPushButton *list_ok_button;
  BusDriver *list_bus_driver;
  QTimer *list_scan_timer;
};


#endif  // LIST_CONSOLES_H

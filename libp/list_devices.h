// list_devices.h
//
// List CallCommander Devices
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

#ifndef LIST_DEVICES_H
#define LIST_DEVICES_H

#include <QtGui/QDialog>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>

#include <connection_data.h>

class ListDevices : public QDialog
{
  Q_OBJECT
  public:
   ListDevices(ConnectionData *conn,QWidget *parent=0,const char *name=0);
   ~ListDevices();
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;

  private slots:
   void okData();
   void cancelData();

 private:
   ConnectionData *list_connection;
   QComboBox *list_type_box;
};


#endif  // LIST_DEVICES_H

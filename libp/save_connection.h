// save_connection.h
//
// Save a CallCommander Connection
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

#ifndef SAVE_CONNECTION_H
#define SAVE_CONNECTION_H

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>

#include <connection_data.h>

class SaveConnection : public QDialog
{
  Q_OBJECT
 public:
  SaveConnection(ConnectionData *conn,QWidget *parent=0,const char *name=0);
  ~SaveConnection();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void textChangedData(const QString &text);
  void okData();
  void cancelData();

 private:
  ConnectionData *save_connection;
  QLineEdit *save_filename_edit;
  QLineEdit *save_description_edit;
  QCheckBox *save_launch_box;
  QPushButton *save_ok_button;
};


#endif  // SAVE_CONNECTION_H

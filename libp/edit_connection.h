// edit_connection.h
//
// Edit a CallCommander Connection
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

#ifndef EDIT_CONNECTION_H
#define EDIT_CONNECTION_H

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <Qt3Support/q3progressdialog.h>
#include <QtGui/QComboBox>

#include <connection_data.h>
#include <telos2101_driver.h>

class EditConnection : public QDialog
{
  Q_OBJECT
 public:
  EditConnection(ConnectionData *conn,QWidget *parent=0,const char *name=0);
  ~EditConnection();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void textChangedData(const QString &text);
  void busConnectedData(unsigned id,bool state);
  void busErrorData(unsigned id,BusDriver::Error err);
  void connectionStepChangedData(unsigned id,int step);
  void okData();
  void cancelData();

 private:
  ConnectionData *edit_connection;
  QLineEdit *edit_hostname_edit;
  QComboBox *edit_serialport_box;
  QLineEdit *edit_username_edit;
  QLineEdit *edit_password_edit;
  QSpinBox *edit_tcpport_spin;
  QPushButton *edit_ok_button;
  BusDriver *edit_bus_driver;
  Q3ProgressDialog *edit_progress_dialog;
};


#endif  // EDIT_CONNECTION_H

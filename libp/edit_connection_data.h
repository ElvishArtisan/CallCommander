// edit_connection_data.h
//
// Edit CallCommander Connection Parameters
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

#ifndef EDIT_CONNECTION_DATA_H
#define EDIT_CONNECTION_DATA_H

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QLabel>

#include <connection_data.h>

class EditConnectionData : public QDialog
{
  Q_OBJECT
 public:
  EditConnectionData(ConnectionData *conn,QStringList *slist,QWidget *parent=0,
		     const char *name=0);
  ~EditConnectionData();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void saveData();
  void saveasData();
  void okData();
  void cancelData();

 private:
  bool Save();
  bool SetConnectionData(ConnectionData *conn);
  void SetCaption();
  ConnectionData *edit_connection;
  QStringList *edit_slist;
  QLabel *edit_type_label;
  QLineEdit *edit_description_edit;
  QLineEdit *edit_hostname_edit;
  QSpinBox *edit_tcpport_spin;
  QLineEdit *edit_serialport_edit;
  QLineEdit *edit_username_edit;
  QLineEdit *edit_userpasswd1_edit;
  QLineEdit *edit_userpasswd2_edit;
  QLineEdit *edit_showname_edit;
  QLineEdit *edit_showpasswd1_edit;
  QLineEdit *edit_showpasswd2_edit;
  QLineEdit *edit_location_edit;
  QSpinBox *edit_console_spin;
  QLineEdit *edit_consolename_edit;
  QPushButton *edit_ok_button;
};


#endif  // EDIT_CONNECTION_DATA_H

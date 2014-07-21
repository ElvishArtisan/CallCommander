// edit_user.h
//
// Edit a CallCommander User.
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

#ifndef EDIT_USER_H
#define EDIT_USER_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>

#include <mllistselector.h>

class EditUser : public QDialog
{
 Q_OBJECT
 public:
  EditUser(QString loginname,QWidget *parent=0,const char *name=0);
  ~EditUser();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void changePasswordData();
  void adminActivatedData(int item);
  void okData();
  void cancelData();

 private:
  QLineEdit *edit_loginname_edit;
  QLineEdit *edit_fullname_edit;
  QLineEdit *edit_description_edit;
  QLineEdit *edit_phone_edit;
  QComboBox *edit_screen_box;
  QComboBox *edit_admin_box;
  QString edit_loginname;
  QString edit_password;
  bool edit_password_changed;
  MLListSelector *edit_show_sel;
};


#endif



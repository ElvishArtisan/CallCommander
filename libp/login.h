// login.h
//
// Login Widget for CallCommander.
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

#ifndef LOGIN_H
#define LOGIN_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <Qt3Support/q3textedit.h>
#include <QtGui/QPixmap>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>

class Login : public QDialog
{
 Q_OBJECT
 public:
  Login(QString *username,QString *password,bool show_db_button,
	QWidget *parent=0);
  ~Login();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void dbData();
  void okData();
  void cancelData();

 private:
  bool CanEditConfig() const;
  QString *login_name;
  QLineEdit *login_name_edit;
  QString *login_password;
  QLineEdit *login_password_edit;
  QPushButton *login_db_button;
};


#endif  // LOGIN_H

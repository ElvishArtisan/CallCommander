// change_password.h
//
// Set Password Widget for CallCommander.
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

#ifndef CHANGE_PASSWORD_H
#define CHANGE_PASSWORD_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <Qt3Support/q3textedit.h>
#include <QtGui/QPixmap>
#include <QtGui/QRadioButton>

class ChangePassword : public QDialog
{
  Q_OBJECT
  public:
   ChangePassword(QString *password,QWidget *parent=0,const char *name=0);
   ~ChangePassword();
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;

  protected:

  private slots:
   void okData();
   void cancelData();

  private:
   QLineEdit *passwd_password_1_edit;
   QLineEdit *passwd_password_2_edit;
   QString *passwd_password;
};


#endif  // CHANGE_PASSWORD_H

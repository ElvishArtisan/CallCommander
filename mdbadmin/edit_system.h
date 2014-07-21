// edit_system.h
//
// Edit a CallCommander Virtual System.
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

#ifndef EDIT_SYSTEM_H
#define EDIT_SYSTEM_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>

class EditSystem : public QDialog
{
 Q_OBJECT
 public:
  EditSystem(QString showname,QWidget *parent=0,const char *name=0);
  ~EditSystem();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void selectedData();
  void okData();
  void cancelData();

 private:
  QLineEdit *edit_showname_edit;
  QLineEdit *edit_showpasswd1_edit;
  QLineEdit *edit_showpasswd2_edit;
  QLineEdit *edit_hostname_edit;
  QLineEdit *edit_location_edit;
  QLineEdit *edit_connection_edit;
};


#endif  // EDIT_SYSTEM_H

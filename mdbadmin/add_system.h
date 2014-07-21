// add_system.h
//
// Add a CallCommander Virtual System.
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

#ifndef ADD_SYSTEM_H
#define ADD_SYSTEM_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>

class AddSystem : public QDialog
{
 Q_OBJECT
 public:
  AddSystem(QString *showname,QWidget *parent=0,const char *name=0);
  ~AddSystem();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void okData();
  void cancelData();

 private:
  QLineEdit *add_showname_edit;
  QString *add_showname;
};


#endif  // ADD_SYSTEM_H

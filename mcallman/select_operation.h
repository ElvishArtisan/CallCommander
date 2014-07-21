// select_operation.h
//
// Select CallCommander Show Records.
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

#ifndef SELECT_OPERATION_H
#define SELECT_OPERATION_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>

class SelectOperation : public QDialog
{
 Q_OBJECT
 public:
  SelectOperation(QString showcode,QString showtitle,
	   QWidget *parent=0,const char *name=0);
  ~SelectOperation();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void actionsData();
  void detailsData();
  void contestsData();
  void closeData();

 private:
  QString select_showcode;
};


#endif  // SELECT_OPERATION_H

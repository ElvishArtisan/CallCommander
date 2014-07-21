// dbdialog.h
//
// Database Connection Dialog for CallCommander.
//
//   (C) Copyright 2002-2011 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DBDIALOG_H
#define DBDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

#include <mldconfig.h>

class DbDialog : public QDialog
{
 Q_OBJECT
 public:
  DbDialog(QWidget *parent=0);
  ~DbDialog();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void okData();
  void cancelData();

 private:
  QLineEdit *db_hostname_edit;
  QLineEdit *db_username_edit;
  QLineEdit *db_password_edit;
  QLineEdit *db_dbname_edit;
  MldConfig *db_config;
};


#endif  // DBDIALOG_H

// mdbadmin.h
//
// The Database Administration Utility for CallCommander.
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


#ifndef MDBADMIN_H
#define MDBADMIN_H

#include <QtGui/QWidget>
#include <QtGui/QSizePolicy>
#include <QtSql/QSqlDatabase>
#include <QtGui/QPixmap>

#define MDBADMIN_USAGE "[--check-db][--mysql-admin-user=<username>][--mysql-admin-password=<password>]\n\n--check-db'\n     Check and (if necessary) create or update the database schema to the\n     current version, then exit.\n\n--mysql-admin-user=<username>\n--mysql-admin-password=<password>\n     When creating a new database, use the supplied credentials for the\n     MySQL administrative user instead of prompting for them"

class MainWidget : public QWidget
{
  Q_OBJECT
 public:
  MainWidget(QWidget *parent=0,const char *name=0);
  ~MainWidget();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void manageUsersData();
  void manageShowsData();
  void manageDirectoryData();
  void manageCallerIdData();
  void manageVirtualData();
  void manageLogicModuleData();
  void showInfoData();
  void quitMainWidget();

 private:
  void ReloadMonitors();
  void ReloadModules();
  QPixmap *mdb_callcommander_map;
};


#endif 

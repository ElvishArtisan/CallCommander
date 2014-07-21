// opendb.cpp
//
// Open a CallCommander Database
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

#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif  // WIN32
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtGui/QMessageBox>
#include <QtSql/QSqlQuery>

#include <ml.h>

#include "globals.h"
#include "opendb.h"
#include "createdb.h"
#include "mysql_login.h"

void ShowDbError(const QString &msg,bool interactive)
{
  if(interactive) {
    QMessageBox::warning(NULL,"MDbAdmin",msg);
  }
  else {
    fprintf(stderr,"mdbadmin: %s\n",(const char *)msg.toAscii());
  }
}

bool OpenDb(const QString &dbname,const QString &login,const QString &pwd,
	    const QString &host,bool interactive)
{
  QString admin_name;
  QString admin_pwd;
  QString sql;
  QSqlQuery *q;
  QString msg;
  MySqlLogin *mysql_login;

  //
  // Open Database
  //
  QSqlDatabase db=QSqlDatabase::addDatabase(admin_config->mysqlDbtype());
  db.setDatabaseName(dbname);
  db.setUserName(login);
  db.setPassword(pwd);
  db.setHostName(host);
  if(!db.open()) {
    return false;
  }

  //
  // Check Schema
  //
  q=new QSqlQuery("select DB from VERSION");
  if(!q->first()) {
    delete q;
    if(!CreateDb()) {   // Can't create tables.
      ShowDbError("Unable to create CallCommander Database",interactive);
      db.removeDatabase(dbname);
      return false;
    }
    if(!InitDb()) {  // Can't initialize tables.
      ShowDbError("Unable to initialize CallCommander Database",
		  interactive);
      db.removeDatabase(dbname);
      return false;
    }
    q=new QSqlQuery("select DB from VERSION");
    if(!q->first()) {
      ShowDbError("Unable to create CallCommander Database",interactive);
      db.removeDatabase(dbname);
      return false;
    }
  }
  int ver=q->value(0).toInt();
  delete q;
  if(ver<ML_VERSION_DATABASE) {
    if(interactive) {
      if(QMessageBox::warning(NULL,"Update Needed",
	    "The CallCommander Database needs to be updated.\nContinue?",
	    QMessageBox::Yes,QMessageBox::No)==QMessageBox::No) {
	db.removeDatabase(dbname);
	return false;
      }      
    }
    if(!UpdateDb(ver)) {
      ShowDbError("Unable to update CallCommander Database",interactive);
      db.removeDatabase(dbname);
      return false;
    }
    msg=QString().
      sprintf("The CallCommander Database has\nbeen updated to Version %d",
	      ML_VERSION_DATABASE);
    if(interactive) {
      QMessageBox::information(NULL,"Database Updated",msg);
    }
  }
  return true;
}

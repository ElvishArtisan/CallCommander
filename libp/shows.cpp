// shows.cpp
//
// Common Code for Managing Show Entries in CallCommander.
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

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "shows.h"

void CreateShow(QString showcode,QString title)
{
  //
  // Create the SHOWS Entry
  //
  QString sql=QString().sprintf("insert into SHOWS set\
                                 SHOW_CODE=\"%s\",TITLE=\"%s\"",
				(const char *)showcode,
				(const char *)title);
  QSqlQuery *q=new QSqlQuery(sql);
  delete q;

  //
  // Create the DETAILS Table
  //
  sql=QString().sprintf("create table if not exists DETAILS_%s (\
                         ID int unsigned primary key auto_increment,\
                         COMPLETE enum('N','Y') default 'N',\
                         GROUP_NUMBER int,\
                         LINE_NUMBER int,\
                         NUMBER char(14) default \"\",\
                         ID_NAME char(15) default \"\",\
                         CALL_DATE date,\
                         CALL_TIME time,\
                         ROUTE_ACTION int,\
                         NAME char(16) default \"\",\
                         AGE int,\
                         QUALITY int,\
                         CELLPHONE enum('N','Y') default 'N',\
                         CITY char(20) default \"\",\
                         STATE char(2) default \"\",\
                         ZIPCODE char(10) default \"\",\
                         STATION char(4) default \"\",\
                         GENDER enum('M','F'),\
                         COMMENT char(94) default \"\",\
                         RING_TIMER int default 0,\
                         HOLD_TIMER int default 0,\
                         AIR_TIMER int default 0,\
                         INDEX NUMBER_IDX (NUMBER))",
			(const char *)showcode);
  q=new QSqlQuery(sql);
  delete q;

  //
  // Create the ACTIONS Table
  //
  sql=QString().sprintf("create table if not exists ACTIONS_%s (\
                         NUMBER char(14) primary key,\
                         ROUTE_ACTION int not null default 0,\
                         LABEL_ACTION int not null default 0,\
                         WARNING_TEXT char(255) default \"\",\
                         NAME char(16) default \"\",\
                         AGE int,\
                         CITY char(20) default \"\",\
                         STATE char(2) default \"\",\
                         ZIPCODE char(10) default \"\",\
                         GENDER enum('M','F'),\
			 CELLPHONE enum('N','Y') default 'N',\
                         CALL_COUNT int default 1)",
			(const char *)showcode);
  q=new QSqlQuery(sql);
  delete q;
}


void DeleteShow(QString showcode)
{
  //
  // Delete the ACTIONS Table
  //
  QString sql=
    QString().sprintf("drop table ACTIONS_%s",(const char *)showcode);
  QSqlQuery *q=new QSqlQuery(sql);
  delete q;

  //
  // Delete the DETAILS Table
  //
  sql=QString().sprintf("drop table DETAILS_%s",(const char *)showcode);
  q=new QSqlQuery(sql);
  delete q;

  //
  // Delete the SHOWS Entry
  //
  sql=QString().sprintf("delete from SHOWS where SHOW_CODE=\"%s\"",
			(const char *)showcode);
  q=new QSqlQuery(sql);
  delete q;
}

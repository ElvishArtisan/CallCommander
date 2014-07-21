// createdb.cpp
//
// Create, Initialize and/or Update a CallCommander Database
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
#include <stdio.h>
#ifndef WIN32
#include <netdb.h>
#include <sys/socket.h>
#endif  // WIN32

#include <QtSql/QSqlDatabase>
#include <QtCore/QDateTime>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QVariant>

#include <ml.h>
#include <shows.h>
#include <escapestring.h>

#include "createdb.h"

bool RunQuery(QString sql)
{
  QSqlQuery *q=new QSqlQuery(sql);
  if(!q->isActive()) {
    printf("SQL: %s\n",(const char *)sql);
    printf("SQL Error: %s\n",(const char *)q->lastError().databaseText());
    delete q;
    return false;
  }
  delete q;
  return true;
}


bool CreateDb()
{
  QString sql;

//
// Create VERSION Table
//
  sql=QString("create table if not exists VERSION (\
               DB int not null primary key)");
  if(!RunQuery(sql)) {
    return false;
  }

//
// Create SHOWS Table
//
  sql=QString("create table if not exists SHOWS (\
               SHOW_CODE char(3) not null primary key,\
               TITLE char(64),\
               DEFAULT_LABEL_ACTION int not null default 0,\
               IS_ACTIVE enum('N','Y') default 'N',\
               CID_HOSTNAME char(255),\
               CID_CONNECTION char(255),\
               CID_CONNECTION_PATH char(255),\
               CID_SOURCE_ID int,\
               START_TIME time default \"00:00:00\",\
               END_TIME time default \"23:59:59\",\
               SUN enum('N','Y') default 'N',\
               MON enum('N','Y') default 'N',\
               TUE enum('N','Y') default 'N',\
               WED enum('N','Y') default 'N',\
               THU enum('N','Y') default 'N',\
               FRI enum('N','Y') default 'N',\
               SAT enum('N','Y') default 'N',\
               LOCAL_AREA_CODE char(10),\
               KILL_CALLS enum('N','Y') default 'N')");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create DIRECTORY Table
  //
  sql=QString("create table if not exists DIRECTORY (\
               NUMBER char(14) not null primary key,\
               CITY char(20),\
               STATE char(2),\
               CELLPHONE enum('N','Y') default 'N',\
               CUSTOM enum('N','Y') default 'N')");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create USERS Table
  //
  sql=QString("create table if not exists USERS (\
               LOGIN_NAME char(32) not null primary key,\
               FULL_NAME char(64),\
               PHONE_NUMBER char(14),\
               DESCRIPTION char(64),\
               PASSWORD char(32),\
               ADMIN_PRIV enum('N','Y') not null default 'N',\
               SCREENER_PRIV enum('N','Y') not null default 'N')");
  if(!RunQuery(sql)) {
    return false;
  }
  
  //
  // Create USER_PERMS Table
  //
  sql=QString("create table if not exists USER_PERMS (\
               ID int unsigned primary key auto_increment,\
               LOGIN_NAME char(8) not null,\
               SHOW_CODE char(3) not null,\
               index LOGIN_NAME_IDX (LOGIN_NAME),\
               index FULL_IDX (LOGIN_NAME,SHOW_CODE))");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create WIRELESS_NAMES Table
  //
  sql=QString("create table if not exists WIRELESS_NAMES (\
               NAME char(60) primary key)");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create CITY_NAMES Table
  //
  sql=QString("create table if not exists CITY_NAMES (\
               NAME char(20) primary key,\
               FULL_NAME char(20))");
  if(!RunQuery(sql)) {
    return false;
  }

//
// Create REPORTS Table
//
  sql=QString("create table if not exists REPORTS (\
               ID int unsigned primary key auto_increment,\
               SHOW_CODE char(3) not null,\
               TITLE char(64),\
               INCLUDE_LINE1 enum('N','Y') default 'Y',\
               INCLUDE_LINE2 enum('N','Y') default 'Y',\
               INCLUDE_LINE3 enum('N','Y') default 'Y',\
               INCLUDE_LINE4 enum('N','Y') default 'Y',\
               INCLUDE_LINE5 enum('N','Y') default 'Y',\
               INCLUDE_LINE6 enum('N','Y') default 'Y',\
               INCLUDE_LINE7 enum('N','Y') default 'Y',\
               INCLUDE_LINE8 enum('N','Y') default 'Y',\
               INCLUDE_LINE9 enum('N','Y') default 'Y',\
               INCLUDE_LINE10 enum('N','Y') default 'Y',\
               INCLUDE_LINE11 enum('N','Y') default 'Y',\
               INCLUDE_LINE12 enum('N','Y') default 'Y',\
               START_AGE int default 0,\
               END_AGE int default 120,\
               CELLPHONE char(1),\
               CITY char(20),\
               STATE char(2),\
               ZIPCODE char(10),\
               STATION char(4),\
               GENDER char(1),\
               RING_TIMER int default 0,\
               HOLD_TIMER int default 0,\
               AIR_TIMER int default 0,\
               HEADER1 char(128),\
               HEADER2 char(128),\
               TYPE int not null,\
               index TITLE_IDX(SHOW_CODE,TITLE))");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create CID_SOURCES Table
  //
  sql=QString("create table if not exists CID_SOURCES (\
               ID int primary key auto_increment,\
               HOSTNAME char(255),\
               HOST_ID int default -1,\
               TTY_PORT char(255),\
               BAUD_RATE int default 9600,\
               DATA_BITS int default 8,\
               STOP_BITS int default 1,\
               PARITY int default 0,\
               index HOSTNAME_IDX(HOSTNAME,HOST_ID))");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create CID_MAPS Table
  //
  sql=QString("create table if not exists CID_MAPS (\
               ID int primary key auto_increment,\
               SHOW_CODE char(3),\
               SOURCE_LINE int,\
               TARGET_LINE int,\
               index SHOWCODE_IDX(SHOW_CODE,SOURCE_LINE))");
  if(!RunQuery(sql)) {
    return false;
  }

//
// Create SHOW_SLOTS Table
//
  sql=QString("create table if not exists SHOW_SLOTS (\
               ID int not null primary key auto_increment,\
               SHOW_CODE char(3) not null,\
               FILTER_POLICY int default 0,\
               START_TIME time default \"00:00:00\",\
               END_TIME time default \"23:59:59\",\
               SUN enum('N','Y') default 'N',\
               MON enum('N','Y') default 'N',\
               TUE enum('N','Y') default 'N',\
               WED enum('N','Y') default 'N',\
               THU enum('N','Y') default 'N',\
               FRI enum('N','Y') default 'N',\
               SAT enum('N','Y') default 'N',\
               INDEX SHOW_CODE_IDX(SHOW_CODE))");
  if(!RunQuery(sql)) {
    return false;
  }

//
// Create CONNECTIONS Table
//
  sql=QString("create table if not exists CONNECTIONS (\
               NAME char(255) primary key,\
               DESCRIPTION char(255),\
               SYSTEM_TYPE int default 0,\
               HOST_NAME char(255),\
               TCP_PORT int unsigned,\
               TTY_PORT char(255),\
               USER_NAME char(64),\
               USER_PASSWORD char(64),\
               SHOW_NAME char(64),\
               SHOW_PASSWORD char(64),\
               LOCATION char(64),\
               CONSOLE int,\
               CONSOLE_NAME char(64))");
  if(!RunQuery(sql)) {
    return false;
  }

//
// Create VIRTUAL_SYSTEMS Table
//
  sql=QString("create table if not exists VIRTUAL_SYSTEMS (\
               ID int primary key auto_increment,\
               SHOW_NAME char(64) unique,\
               SHOW_PASSWORD char(32),\
               CONNECTION char(255),\
               HOSTNAME char(255),\
               LOCATION char(64),\
               index HOSTNAME_IDX(HOSTNAME))");
  if(!RunQuery(sql)) {
    return false;
  }

//
// Create LOGIC_MODULES Table
//
  sql="create table if not exists LOGIC_MODULES (\
       ID int primary key auto_increment,\
       HOSTNAME char(255),\
       CONNECTION_NAME char(255),\
       MODULE_PATH char(255),\
       MODULE_ARGS char(255),\
       LOG_PATH char(255),\
       DESCRIPTION char(255),\
       USE_REALTIME enum('N','Y') default 'N',\
       INDEX HOSTNAME_IDX(HOSTNAME))";
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create CID_FILTERS Table
  //
  sql="create table if not exists CID_FILTERS (\
       ID int primary key auto_increment,\
       SLOT_ID int not null,\
       FILTER char(64),\
       index SLOT_ID_IDX(SLOT_ID))";
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create WINNERS Table
  //
  sql="create table if not exists WINNERS (\
       ID int primary key auto_increment,\
       SHOW_CODE char(3) not null,\
       NUMBER char(14),\
       DETAIL_ID int unsigned,\
       STATUS int not null default 1,\
       FIRST_NAME char(32),\
       LAST_NAME char(32),\
       GENDER enum('M','F','U') default 'U',\
       AGE int,\
       EMAIL char(64),\
       PHONE char(14),\
       ADDRESS1 char(64),\
       ADDRESS2 char(64),\
       CITY char(64),\
       STATE char(2),\
       ZIPCODE char(10),\
       ORIGIN_LOGIN_NAME char(32) not null,\
       ORIGIN_DATETIME datetime not null,\
       DISPOSITION_LOGIN_NAME char(32),\
       DISPOSITION_DATETIME datetime,\
       PRIZE_DESCRIPTION text,\
       REMARKS text,\
       index SHOW_CODE_IDX(SHOW_CODE),\
       index SHOW_CODE_NUMBER_IDX(SHOW_CODE,NUMBER))";
  if(!RunQuery(sql)) {
    return false;
  }

  return true;
}


bool InitDb()
{
  QString sql;

  //
  // Generate Version Number
  //
  sql=QString().sprintf("insert into VERSION (DB) values (%d)",
			ML_VERSION_DATABASE);
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create Default User
  //
  sql=QString().sprintf("insert into USERS set\
                         LOGIN_NAME=\"%s\",\
                         FULL_NAME=\"%s\",\
                         DESCRIPTION=\"%s\",\
                         PASSWORD=\"\",\
                         ADMIN_PRIV=\"Y\"",
			ML_DEFAULT_LOGIN_NAME,
			ML_DEFAULT_FULL_NAME,
			ML_DEFAULT_DESCRIPTION_NAME);
  if(!RunQuery(sql)) {
    return false;
  }
  sql=QString().sprintf("insert into USER_PERMS set\
                         LOGIN_NAME=\"%s\",\
                         SHOW_CODE=\"%s\"",
			ML_DEFAULT_LOGIN_NAME,
			ML_DEFAULT_SHOW_CODE);
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create Default Show
  //
  CreateShow(ML_DEFAULT_SHOW_CODE,ML_DEFAULT_SHOW_TITLE);

  //
  // Generic Wireless ID Strings
  //
  if(!RunQuery("insert into WIRELESS_NAMES set NAME=\"WIRELESS\"")) {
    return false;
  }
  if(!RunQuery("insert into WIRELESS_NAMES set NAME=\"AIR\"")) {
    return false;
  }
  if(!RunQuery("insert into WIRELESS_NAMES set NAME=\"CELL\"")) {
    return false;
  }
  if(!RunQuery("insert into WIRELESS_NAMES set NAME=\"MOBILE\"")) {
    return false;
  }

  //
  // Company Wireless ID Strings
  //
  if(!RunQuery("insert into WIRELESS_NAMES set NAME=\"NEXTEL\"")) {
    return false;
  }
  if(!RunQuery("insert into WIRELESS_NAMES set NAME=\"CINGULAR\"")) {
    return false;
  }
  if(!RunQuery("insert into WIRELESS_NAMES set NAME=\"AT&T WIRELESS\"")) {
    return false;
  }
  if(!RunQuery("insert into WIRELESS_NAMES set NAME=\"SPRINT\"")) {
    return false;
  }
  if(!RunQuery("insert into WIRELESS_NAMES set NAME=\"CELLCO\"")) {
    return false;
  }

  //
  // City Contractions
  //
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"MILWAUK\",FULL_NAME=\"MILWAUKEE\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"DETROIT\",FULL_NAME=\"DETROIT\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"CHICGO\",FULL_NAME=\"CHICAGO\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"NWYRCY\",FULL_NAME=\"NEW YORK\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"NASSAU\",FULL_NAME=\"NASSAU\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"WSCHST\",FULL_NAME=\"WESTCHESTER\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"PHLDLPH\",FULL_NAME=\"PHILADELPHIA\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"PHPHSB\",FULL_NAME=\"PHPHSB\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"PTGSBN\",FULL_NAME=\"PTGSBN\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"HARISBG\",FULL_NAME=\"HARRISBURG\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"WSHNGTN\",FULL_NAME=\"WASHINGTON\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"WSNGTN\",FULL_NAME=\"WASHINGTON\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"HNTNGTN\",FULL_NAME=\"HUNTINGTON\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"CHRLSTN\",FULL_NAME=\"CHARLESTON\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"PRKRSBG\",FULL_NAME=\"PARKERSBURG\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"NWPTNWS\",FULL_NAME=\"NWPTNWS\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"WHELNG\",FULL_NAME=\"WHEELING\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"NRFOLK\",FULL_NAME=\"NORFOLK\"")) {
    return false;
  }
  if(!RunQuery("insert into CITY_NAMES \
                set NAME=\"PTTSBG\",FULL_NAME=\"PITTSBURGH\"")) {
    return false;
  }
  return true;
}


bool UpdateDb(int ver)
{
  QString sql;
  QSqlQuery *q,*q1,*q2;

  // **** Start of version updates ****

  if(ver<1) {
  }

  if(ver<2) {
    sql="select SHOW_CODE from SHOWS";
    q=new QSqlQuery(sql);
    while(q->next()) {
      sql=QString().sprintf("alter table DETAILS_%s\
                           add column CALL_DATE date after CALL_DATETIME",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table DETAILS_%s\
                             add column CALL_TIME time after CALL_DATE",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;

      sql=QString().sprintf("select ID,CALL_DATETIME from DETAILS_%s",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      while(q1->next()) {
	sql=QString().sprintf("update DETAILS_%s set\
                               CALL_DATE=\"%s\",CALL_TIME=\"%s\"\
                               where ID=%d",
			      (const char *)q->value(0).toString(),
			      (const char *)q1->value(1).toDate().
			      toString("yyyy-MM-dd"),
			      (const char *)q1->value(1).toTime().
			      toString("hh:mm:ss"),
			      q1->value(0).toUInt());
	q2=new QSqlQuery(sql);
	delete q2;
      }
      delete q1;
      sql=QString().sprintf("alter table DETAILS_%s drop column CALL_DATETIME",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
    }
    delete q;
  }

  if(ver<3) {
    sql="select SHOW_CODE from SHOWS";
    q=new QSqlQuery(sql);
    while(q->next()) {
      sql=QString().sprintf("alter table ACTIONS_%s \
                             alter column WARNING_TEXT set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("update ACTIONS_%s \
                             set WARNING_TEXT=\"\"\
                             where ISNULL(WARNING_TEXT)",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table ACTIONS_%s \
                             alter column NAME set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("update ACTIONS_%s \
                             set NAME=\"\"\
                             where ISNULL(NAME)",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table ACTIONS_%s \
                             alter column CITY set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("update ACTIONS_%s \
                             set CITY=\"\"\
                             where ISNULL(CITY)",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table ACTIONS_%s \
                             alter column STATE set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("update ACTIONS_%s \
                             set STATE=\"\"\
                             where ISNULL(STATE)",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table ACTIONS_%s \
                             alter column ZIPCODE set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("update ACTIONS_%s \
                             set ZIPCODE=\"\"\
                             where ISNULL(ZIPCODE)",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table DETAILS_%s \
                             alter column NUMBER set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table DETAILS_%s \
                             alter column ID_NAME set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table DETAILS_%s \
                             alter column NAME set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table DETAILS_%s \
                             alter column CITY set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table DETAILS_%s \
                             alter column STATE set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table DETAILS_%s \
                             alter column ZIPCODE set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table DETAILS_%s \
                             alter column STATION set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString().sprintf("alter table DETAILS_%s \
                             alter column COMMENT set default \"\"",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
    }
    delete q;
  }

  if(ver<4) {
    sql=QString("create table if not exists REPORTS (\
                 ID int unsigned primary key auto_increment,\
                 SHOW_CODE char(3) not null,\
                 TITLE char(64),\
                 INCLUDE_LINE1 enum('N','Y') default 'Y',\
                 INCLUDE_LINE2 enum('N','Y') default 'Y',\
                 INCLUDE_LINE3 enum('N','Y') default 'Y',\
                 INCLUDE_LINE4 enum('N','Y') default 'Y',\
                 INCLUDE_LINE5 enum('N','Y') default 'Y',\
                 INCLUDE_LINE6 enum('N','Y') default 'Y',\
                 INCLUDE_LINE7 enum('N','Y') default 'Y',\
                 INCLUDE_LINE8 enum('N','Y') default 'Y',\
                 INCLUDE_LINE9 enum('N','Y') default 'Y',\
                 INCLUDE_LINE10 enum('N','Y') default 'Y',\
                 INCLUDE_LINE11 enum('N','Y') default 'Y',\
                 INCLUDE_LINE12 enum('N','Y') default 'Y',\
                 START_AGE int default 0,\
                 END_AGE int default 120,\
                 CELLPHONE char(1),\
                 CITY char(20),\
                 STATE char(2),\
                 ZIPCODE char(10),\
                 STATION char(4),\
                 GENDER char(1),\
                 RING_TIMER int default 0,\
                 HOLD_TIMER int default 0,\
                 AIR_TIMER int default 0,\
                 HEADER1 char(128),\
                 HEADER2 char(128),\
                 TYPE int not null,\
                 index TITLE_IDX(SHOW_CODE,TITLE))");
    if(!RunQuery(sql)) {
      return false;
    }
  }

  if(ver<5) {
    sql="select SHOW_CODE from SHOWS";
    q=new QSqlQuery(sql);
    while(q->next()) {
      sql=QString().sprintf("alter table ACTIONS_%s \
                             add column CALL_COUNT int default 1",
			    (const char *)q->value(0).toString());
      q1=new QSqlQuery(sql);
      delete q1;
    }
    delete q;
  }

  if(ver<6) {
    sql="alter table SHOWS add column IS_ACTIVE enum('N','Y') default 'N'";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column CID_CONNECTION char(255)";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column CID_TTY_PORT char(255)";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column START_TIME time default \"00:00:00\"";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column END_TIME time default \"23:59:59\"";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column SUN enum('N','Y') default 'N'";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column MON enum('N','Y') default 'N'";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column TUE enum('N','Y') default 'N'";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column WED enum('N','Y') default 'N'";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column THU enum('N','Y') default 'N'";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column FRI enum('N','Y') default 'N'";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column SAT enum('N','Y') default 'N'";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column LOCAL_AREA_CODE char(10)";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column KILL_CALLS enum('N','Y') default 'N'";
    q=new QSqlQuery(sql);
    delete q;
  }

  //
  // No version 7!
  //

  if(ver<8) {
    sql="alter table SHOWS add column CID_SOURCE_ID int after CID_CONNECTION";
    q=new QSqlQuery(sql);
    delete q;

    sql=QString("create table if not exists CID_SOURCES (\
               ID int primary key auto_increment,\
               HOSTNAME char(255),\
               HOST_ID int default -1,\
               TTY_PORT char(255),\
               BAUD_RATE int default 9600,\
               DATA_BITS int default 8,\
               STOP_BITS int default 1,\
               PARITY int default 0,\
               index HOSTNAME_IDX(HOSTNAME,HOST_ID))");
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<9) {
    sql="alter table SHOWS add column CID_CONNECTION_PATH char(255)\
         after CID_CONNECTION";
    q=new QSqlQuery(sql);
    delete q;

    sql=QString("create table if not exists CID_MAPS (\
                 ID int primary key auto_increment,\
                 SHOW_CODE char(3),\
                 SOURCE_LINE int,\
                 TARGET_LINE int,\
                 index SHOWCODE_IDX(SHOW_CODE,SOURCE_LINE))");
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<10) {
    sql=QString("create table if not exists SHOW_SLOTS (\
                 ID int not null primary key auto_increment,\
                 SHOW_CODE char(3) not null,\
                 START_TIME time default \"00:00:00\",\
                 END_TIME time default \"23:59:59\",\
                 SUN enum('N','Y') default 'N',\
                 MON enum('N','Y') default 'N',\
                 TUE enum('N','Y') default 'N',\
                 WED enum('N','Y') default 'N',\
                 THU enum('N','Y') default 'N',\
                 FRI enum('N','Y') default 'N',\
                 SAT enum('N','Y') default 'N',\
                 INDEX SHOW_CODE_IDX(SHOW_CODE))");
    q=new QSqlQuery(sql);
    delete q;

    sql="select SHOW_CODE,START_TIME,END_TIME,SUN,MON,TUE,WED,THU,FRI,SAT \
         from SHOWS where IS_ACTIVE=\"Y\"";
    q=new QSqlQuery(sql);
    while(q->next()) {
      sql=QString().sprintf("insert into SHOW_SLOTS set SHOW_CODE=\"%s\",\
                             START_TIME=\"%s\",END_TIME=\"%s\",\
                             SUN=\"%s\",MON=\"%s\",TUE=\"%s\",WED=\"%s\",\
                             THU=\"%s\",FRI=\"%s\",SAT=\"%s\"",
			    (const char *)q->value(0).toString(),
			    (const char *)q->value(1).toTime().
			    toString("hh:mm:ss"),
			    (const char *)q->value(2).toTime().
			    toString("hh:mm:ss"),
			    (const char *)q->value(3).toString(),
			    (const char *)q->value(4).toString(),
			    (const char *)q->value(5).toString(),
			    (const char *)q->value(6).toString(),
			    (const char *)q->value(7).toString(),
			    (const char *)q->value(8).toString(),
			    (const char *)q->value(9).toString());
      q1=new QSqlQuery(sql);
      delete q1;
    }
    delete q;
  }

  if(ver<11) {
    sql=QString("create table if not exists CONNECTIONS (\
               NAME char(255) primary key,\
               DESCRIPTION char(255),\
               SYSTEM_TYPE int default 0,\
               HOST_NAME char(255),\
               TCP_PORT int unsigned,\
               TTY_PORT char(255),\
               USER_NAME char(64),\
               USER_PASSWORD char(64),\
               SHOW_NAME char(64),\
               SHOW_PASSWORD char(64),\
               LOCATION char(64),\
               CONSOLE int,\
               CONSOLE_NAME char(64))");
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<12) {
    sql=QString("create table if not exists VIRTUAL_SYSTEMS (\
                 ID int primary key auto_increment,\
                 SHOW_NAME char(64) unique,\
                 SHOW_PASSWORD char(32),\
                 CONNECTION char(255),\
                 HOSTNAME char(255),\
                 LOCATION char(64),\
                 index HOSTNAME_IDX(HOSTNAME))");
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SHOWS add column CID_HOSTNAME char(255) after IS_ACTIVE";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<13) {
    sql="alter table USERS modify PASSWORD char(41)";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<14) {
    sql="alter table USERS add column SCREENER_PRIV enum('N','Y') default 'N' \
         after ADMIN_PRIV";
    q=new QSqlQuery(sql);
    delete q;

    sql="update USERS set SCREENER_PRIV=\"Y\"";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<15) {
    sql="alter table USERS modify LOGIN_NAME char(32)";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<16) {
    sql="create table if not exists LOGIC_MODULES (\
         ID int primary key auto_increment,\
         HOSTNAME char(255),\
         CONNECTION_NAME char(255),\
         MODULE_PATH char(255),\
         LOG_PATH char(255),\
         DESCRIPTION char(255),\
         INDEX HOSTNAME_IDX(HOSTNAME))";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<17) {
    sql="alter table LOGIC_MODULES add column MODULE_ARGS char(255) \
         after MODULE_PATH";
    q=new QSqlQuery(sql);
    delete q;
  }
 
  if(ver<18) {
    sql="alter table LOGIC_MODULES add column USE_REALTIME enum('N','Y')\
         default 'N' after DESCRIPTION";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<19) {
    sql="alter table SHOW_SLOTS add column FILTER_POLICY int default 0 \
         after SHOW_CODE";
    q=new QSqlQuery(sql);
    delete q;

    sql="create table if not exists CID_FILTERS (\
         ID int primary key auto_increment,\
         SLOT_ID int not null,\
         FILTER char(64),\
         index SLOT_ID_IDX(SLOT_ID))";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<20) {
  sql="create table if not exists WINNERS (\
       ID int primary key auto_increment,\
       SHOW_CODE char(3) not null,\
       NUMBER char(14),\
       DETAIL_ID int unsigned,\
       STATUS int not null default 1,\
       FIRST_NAME char(32),\
       LAST_NAME char(32),\
       GENDER enum('M','F','U') default 'U',\
       AGE int,\
       EMAIL char(64),\
       PHONE char(14),\
       ADDRESS1 char(64),\
       ADDRESS2 char(64),\
       CITY char(64),\
       STATE char(2),\
       ZIPCODE char(10),\
       ORIGIN_LOGIN_NAME char(32) not null,\
       ORIGIN_DATETIME datetime not null,\
       DISPOSITION_LOGIN_NAME char(32),\
       DISPOSITION_DATETIME datetime,\
       PRIZE_DESCRIPTION text,\
       REMARKS text,\
       index SHOW_CODE_IDX(SHOW_CODE),\
       index SHOW_CODE_NUMBER_IDX(SHOW_CODE,NUMBER))";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<21) {
    sql="select SHOW_CODE from SHOWS";
    q=new QSqlQuery(sql);
    while(q->next()) {
      sql=QString().sprintf("alter table DETAILS_%s add column \
                             COMPLETE enum('N','Y') default 'N' after ID",
			    (const char *)
			    EscapeString(q->value(0).toString()));
      q1=new QSqlQuery(sql);
      delete q1;

      sql=QString().sprintf("update DETAILS_%s set COMPLETE=\"Y\"",
			    (const char *)
			    EscapeString(q->value(0).toString()));
      q1=new QSqlQuery(sql);
      delete q1;
    }
    delete q;
  }

  // **** End of version updates ****
  
  //
  // Update Version Field
  //
  q=new QSqlQuery(QString().sprintf("update VERSION set DB=%d",
				    ML_VERSION_DATABASE));
  delete q;
  return true;
}

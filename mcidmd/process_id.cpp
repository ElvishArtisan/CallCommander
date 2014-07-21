// process_id.cpp
//
// A telephone services server for Call Commander
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

#include <syslog.h>

#include <mlconf.h>

#include <telos2101_driver.h>
#include <calleriddetail.h>
#include <mcidmd.h>

#include <QtSql/QSqlQuery>

void MainObject::ProcessId(CidMonitor *mon,CallerIdDetail *cid)
{
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  MlTimeslot *slot=NULL;

  //
  // Verify Monitor is Active
  //
  QDateTime current_datetime=
    QDateTime(QDate::currentDate(),QTime::currentTime());
  if((slot=mon->slotList()->timeslot(current_datetime))==NULL) {
    return;
  }

  //
  // Normalize the Phone Number
  //
  int line=mon->cidLineMap(cid->lineNumber()-1);
  if(line<0) {  // CID Disabled
    return;
  }
  QString pnum;
  if(cid->number().length()==7) {
    pnum=mon->areaCode()+cid->number();
  }
  else {
    pnum=cid->number();
  }
  mon->setPhoneNumber(line,pnum);
  mon->setPhoneName(line,cid->name());

  //
  // Process Filter Rules
  //
  if(mon->killCalls()) {
    switch(slot->filterPolicy()) {
    case MlTimeslot::AcceptPolicy:
      if(slot->filterMatch(pnum)) {
	KillCall(mon,line,pnum);
	return;
      }
      break;

    case MlTimeslot::DenyPolicy:
      if(!slot->filterMatch(pnum)) {
	KillCall(mon,line,pnum);
	return;
      }
      break;
    }

  }

  //
  // Create an ACTIONS record, if needed
  //
  sql=QString().sprintf("select NUMBER from ACTIONS_%s where NUMBER=\"%s\"",
			(const char *)mon->showCode(),
			(const char *)pnum);
  q=new QSqlQuery(sql);
  if(!q->first()) {
    CreateActionRecord(mon,line,pnum);
  }
  else {
    IncrementCallerCount(mon,line,pnum);
  }
  delete q;

  //
  // Create the DETAILS record
  //
  sql=QString().sprintf("insert into DETAILS_%s set NUMBER=\"%s\"",
			(const char *)mon->showCode(),
			(const char *)pnum);
  q=new QSqlQuery(sql);
  delete q;
  sql=QString().sprintf("select LAST_INSERT_ID() from DETAILS_%s",
			(const char *)mon->showCode());
  q=new QSqlQuery(sql);
  if(q->first()) {
    mon->setDetailId(line,q->value(0).toUInt());
  }
  delete q;

  //
  // Process Actions
  //
  sql=QString().sprintf("select NAME,AGE,CITY,STATE,ZIPCODE,GENDER,CELLPHONE,\
                         ROUTE_ACTION,LABEL_ACTION,WARNING_TEXT\
                         from ACTIONS_%s where NUMBER=\"%s\"",
			(const char *)mon->showCode(),
			(const char *)pnum);
  q=new QSqlQuery(sql);
  if(q->first()) {
    //
    // Process Route Actions
    //
    switch((MldConfig::RouteAction)q->value(7).toInt()) {
	case MldConfig::Drop:
	  if(mon->killCalls()) {
	    KillCall(mon,line,pnum);
	  }
	  break;

	case MldConfig::Warn:
	  mon->setWarningText(line,q->value(9).toString());
	  if(mon->lineState(line)==BusDriver::LineIdle) {
	    mon->setRouteAction(line,MldConfig::Warn);
	  }
	  else {  // Late notification -- send the warning directly
	    syslog(LOG_DAEMON|LOG_DEBUG,"processed late CallerID message");
	    mon->busDriver()->sendString(BusDriver::LineWarningGlobalElement,
					 line,mon->warningText(line));
	  }
	  break;

	default:
	  mon->setPhoneNumber(line,pnum);
	  if(mon->lineState(line)!=BusDriver::LineIdle) { // Late Notification!
	    syslog(LOG_DAEMON|LOG_DEBUG,"processed late CallerID message");
	    mon->busDriver()->sendString(BusDriver::PhoneElement,line,pnum);
	  }
	  break;
    }

    //
    // Process Label Actions
    //
    switch((MldConfig::LabelAction)q->value(8).toInt()) {
	case MldConfig::UseFirstKnown:
	  sql=QString().sprintf("select NAME,AGE,CITY,STATE,ZIPCODE,\
                                 GENDER,CELLPHONE \
                                 from DETAILS_%s where (NUMBER=\"%s\")&&\
                                 (NAME!=\"\") \
                                 order by CALL_DATE asc,CALL_TIME asc",
				(const char *)mon->showCode(),
				(const char *)pnum);
	  q1=new QSqlQuery(sql);
	  if(q1->first()) {
	    ApplyActionRecord(q1,mon->metaData(line),false);
	  }
	  else {
	    ApplyActionRecord(q,mon->metaData(line),false);
	  }
	  delete q1;
	  break;

	case MldConfig::UseLastKnown:
	  sql=QString().sprintf("select NAME,AGE,CITY,STATE,ZIPCODE,\
                                 GENDER,CELLPHONE \
                                 from DETAILS_%s where (NUMBER=\"%s\")&&\
                                (NAME!=\"\") \
                                 order by CALL_DATE desc,CALL_TIME desc",
				(const char *)mon->showCode(),
				(const char *)pnum);
	  q1=new QSqlQuery(sql);
	  if(q1->first()) {
	    ApplyActionRecord(q1,mon->metaData(line),true);
	  }
	  else {
	    ApplyActionRecord(q,mon->metaData(line),true);
	  }
	  delete q1;
	  break;

	case MldConfig::UseSpecified:
	  ApplyActionRecord(q,mon->metaData(line),false);
	  break;

	case MldConfig::Blank:
	  break;
    }
  }
  delete q;

  //
  // Get Last Call Date/Time
  //
  sql=QString().sprintf("select CALL_DATE,CALL_TIME,AIR_TIMER from DETAILS_%s \
                         where (NUMBER=\"%s\")&&(NAME!=\"\") \
                         order by CALL_DATE desc,CALL_TIME desc",
			(const char *)mon->showCode(),
			(const char *)pnum);
  q=new QSqlQuery(sql);
  if(q->first()) {
    mon->metaData(line)->
      setElement(BusDriver::LastCallDatetimeElement,
		 QString().sprintf("%d",QDateTime(q->value(0).toDate(),
						  q->value(1).toTime()).
				   toTime_t()));
    q->seek(-1);
    while(q->next()) {
      if(q->value(2).toInt()>0) {
	mon->metaData(line)->
	  setElement(BusDriver::LastOnAirDatetimeElement,
		     QString().sprintf("%d",QDateTime(q->value(0).toDate(),
						      q->value(1).toTime()).
				       toTime_t()));
	break;
      }
    }
  }
  mon->metaData(line)->setElement(BusDriver::CounterElement,q->size()+1);
  delete q;

  //
  // Get Last Prize Award Date/Time
  //
  sql=QString().sprintf("select ORIGIN_DATETIME from WINNERS where \
                         (SHOW_CODE=\"%s\")&&(NUMBER=\"%s\") \
                         order by ORIGIN_DATETIME desc",
			(const char *)mon->showCode(),
			(const char *)pnum);
  q=new QSqlQuery(sql);
  if(q->first()) {
    mon->metaData(line)->
      setElement(BusDriver::LastAwardDatetimeElement,
		 q->value(0).toDateTime().toTime_t());
  }
  delete q;

  if(mon->lineState(line)!=BusDriver::LineIdle) {  // Late notfication
    syslog(LOG_DAEMON|LOG_DEBUG,"processed late CallerID message");
    mon->metaData(line)->sendData(mon->busDriver(),line);
  }
}


void MainObject::KillCall(CidMonitor *mon,int line,const QString &pnum)
{
  mon->setRouteAction(line,MldConfig::Drop);
  if(mon->lineState(line)!=BusDriver::LineIdle) { // Late!
    mon->busDriver()->
      selectLine(line,mon->connection()->console(),MCIDMD_KILL_BANK);
  }
  syslog(LOG_DAEMON|LOG_DEBUG,"killed call from %s",(const char *)pnum);
}


void MainObject::CreateActionRecord(CidMonitor *mon,int line,QString pnum)
{
  QString sql;
  QSqlQuery *q;
  MldConfig::LabelAction label_action=MldConfig::Blank;

  //
  // Get Default Label Action
  //
  sql=QString().sprintf("select DEFAULT_LABEL_ACTION from SHOWS\
                         where SHOW_CODE=\"%s\"",
			(const char *)mon->showCode());
  q=new QSqlQuery(sql);
  if(q->first()) {
    label_action=(MldConfig::LabelAction)q->value(0).toInt();
  }
  delete q;

  //
  // Check the Directory
  //
  sql=QString().sprintf("select CITY,STATE,CELLPHONE from DIRECTORY \
                         where NUMBER=\"%s\"",
			(const char *)pnum.left(6));
  q=new QSqlQuery(sql);

  //
  // Create the Record
  //
  if(q->first()) {
    sql=QString().sprintf("insert into ACTIONS_%s set\
                           NUMBER=\"%s\",\
                           LABEL_ACTION=%d,\
                           CITY=\"%s\",\
                           STATE=\"%s\",\
                           CELLPHONE=\"%s\"",
			  (const char *)mon->showCode(),
			  (const char *)pnum,
			  label_action,
			  (const char *)q->value(0).toString(),
			  (const char *)q->value(1).toString(),
			  (const char *)q->value(2).toString());
  }
  else {
    sql=QString().sprintf("insert into ACTIONS_%s set\
                           NUMBER=\"%s\",\
                           LABEL_ACTION=%d",
			  (const char *)mon->showCode(),
			  (const char *)pnum,
			  label_action);
  }
  delete q;
  q=new QSqlQuery(sql);
  delete q;
}


void MainObject::ApplyActionRecord(QSqlQuery *q,CallMetaData *meta,
				   bool use_datetime)
{
  meta->setElement(BusDriver::NameElement,q->value(0).toString());
  meta->setElement(BusDriver::AgeElement,q->value(1).toInt());
  meta->setElement(BusDriver::CityElement,q->value(2).toString());
  meta->setElement(BusDriver::StateElement,q->value(3).toString());
  meta->setElement(BusDriver::ZipElement,q->value(4).toString());
  meta->setElement(BusDriver::GenderElement,q->value(5).toString());
  meta->setElement(BusDriver::CellphoneElement,q->value(6).toString());
}


void MainObject::IncrementCallerCount(CidMonitor *mon,int line,QString pnum)
{
  QString sql;
  QSqlQuery *q;

  //
  // Increment the counter
  //
  sql=QString().sprintf("select CALL_COUNT from ACTIONS_%s\
                         where NUMBER=\"%s\"",
			(const char *)mon->showCode(),
			(const char *)pnum);
  q=new QSqlQuery(sql);
  if(q->first()) {
    sql=QString().sprintf("update ACTIONS_%s set CALL_COUNT=%d\
                         where NUMBER=\"%s\"",
			  (const char *)mon->showCode(),q->value(0).toInt()+1,
			  (const char *)pnum);
    delete q;
    q=new QSqlQuery(sql);
  }
  delete q;
}


void MainObject::LogCall(CidMonitor *mon,int line)
{
  QString sql;
  QSqlQuery *q;

  if(mon->startDateTime(line).isNull()) {
    return;
  }
  sql=QString().
    sprintf("update DETAILS_%s set\
             COMPLETE=\"Y\",\
             LINE_NUMBER=%d,\
             NUMBER=\"%s\",\
             ID_NAME=\"%s\",\
             CALL_DATE=\"%s\",\
             CALL_TIME=\"%s\",\
             ROUTE_ACTION=%d,\
             NAME=\"%s\",\
             AGE=%d,\
             QUALITY=%d,\
             CELLPHONE=\"%s\",\
             CITY=\"%s\",\
             STATE=\"%s\",\
             ZIPCODE=\"%s\",\
             GENDER=\"%s\",\
             COMMENT=\"%s\",\
             RING_TIMER=%u,\
             HOLD_TIMER=%u,\
             AIR_TIMER=%u \
             where ID=%u",
	    (const char *)mon->showCode(),
	    line+1,
	    (const char *)mon->phoneNumber(line),
	    (const char *)mon->phoneName(line),
	    (const char *)mon->startDateTime(line).toString("yyyy-MM-dd"),
	    (const char *)mon->startDateTime(line).toString("hh:mm:ss"),
	    mon->routeAction(line),
	    (const char *)mon->metaData(line)->
	    element(BusDriver::NameElement).toString(),
	    mon->metaData(line)->element(BusDriver::AgeElement).toInt(),
	    mon->metaData(line)->element(BusDriver::QualityElement).toInt(),
	    (const char *)MLYesNo(mon->metaData(line)->
			       element(BusDriver::CellphoneElement).toBool()),
	    (const char *)mon->metaData(line)->
	    element(BusDriver::CityElement).toString(),
	    (const char *)mon->metaData(line)->
	    element(BusDriver::StateElement).toString(),
	    (const char *)mon->metaData(line)->
	    element(BusDriver::ZipElement).toString(),
	    (const char *)mon->metaData(line)->
	    element(BusDriver::GenderElement).toString(),
	    (const char *)mon->metaData(line)->
	    element(BusDriver::CommentElement).toString(),
	    (unsigned)mon->timer(line,CidMonitor::RingTimer)*1000,
	    (unsigned)mon->timer(line,CidMonitor::HoldTimer)*1000,
	    (unsigned)mon->timer(line,CidMonitor::AirTimer)*1000,
	    mon->detailId(line));
  q=new QSqlQuery(sql);
  delete q;
}

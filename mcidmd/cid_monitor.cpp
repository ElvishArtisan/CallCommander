// cid_monitor.cpp
//
// A Container Class for CallCommander CallerID Monitor Processes.
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

#include <cid_monitor.h>

CidMonitor::CidMonitor()
{
  clear();
}


QString CidMonitor::showCode() const
{
  return cid_showcode;
}


void CidMonitor::setShowCode(const QString &showcode)
{
  cid_showcode=showcode;
}


BusDriver *CidMonitor::busDriver() const
{
  return cid_driver;
}


void CidMonitor::setBusDriver(BusDriver *driver)
{
  cid_driver=driver;
}


ConnectionData *CidMonitor::connection() const
{
  return cid_connection;
}


void CidMonitor::setConnection(ConnectionData *data)
{
  cid_connection=data;
}


bool CidMonitor::connected() const
{
  return cid_connected;
}


void CidMonitor::setConnected(bool state)
{
  cid_connected=state;
}


CallerIdTrap *CidMonitor::cidTrap()
{
  return cid_trap;
}


void CidMonitor::setCidTrap(CallerIdTrap *trap)
{
  cid_trap=trap;
}


MlTimeslotList *CidMonitor::slotList() const
{
  return cid_slotlist;
}


void CidMonitor::setSlotList(MlTimeslotList *list)
{
  cid_slotlist=list;
}


QString CidMonitor::areaCode() const
{
  return cid_area_code;
}


void CidMonitor::setAreaCode(const QString &code)
{
  cid_area_code=code;
}


bool CidMonitor::killCalls() const
{
  return cid_kill_calls;
}


void CidMonitor::setKillCalls(bool state)
{
  cid_kill_calls=state;
}


unsigned CidMonitor::detailId(unsigned line) const
{
  if(line<cid_detail_ids.size()) {
    return cid_detail_ids[line];
  }
  return 0;
}


void CidMonitor::setDetailId(unsigned line,unsigned id)
{
  for(unsigned i=cid_detail_ids.size();i<=line;i++) {
    cid_detail_ids.push_back(0);
  }
  cid_detail_ids[line]=id;
}


BusDriver::LineState CidMonitor::lineState(unsigned line) const
{
  if(line<cid_line_states.size()) {
    return cid_line_states[line];
  }
  return BusDriver::LineInactive;
}


void CidMonitor::setLineState(unsigned line,BusDriver::LineState lstate)
{
  for(unsigned i=cid_line_states.size();i<=line;i++) {
    cid_line_states.push_back(BusDriver::LineInactive);
  }
  for(unsigned i=cid_call_timers.size();i<=line;i++) {
    cid_call_timers.push_back(new unsigned long long[CidMonitor::LastTimer]);
  }
  cid_line_states[line]=lstate;
}


CallMetaData *CidMonitor::metaData(unsigned line)
{
  for(unsigned i=cid_metadata.size();i<=line;i++) {
    cid_metadata.push_back(new CallMetaData());
  }
  return cid_metadata[line];
}


QDateTime CidMonitor::startDateTime(unsigned line) const
{
  if(line<cid_start_datetimes.size()) {
    return cid_start_datetimes[line];
  }
  return QDateTime();
}


void CidMonitor::setStartDateTime(unsigned line,const QDateTime &datetime)
{
  for(unsigned i=cid_start_datetimes.size();i<=line;i++) {
    cid_start_datetimes.push_back(QDateTime());
  }
  cid_start_datetimes[line]=datetime;
}


QString CidMonitor::phoneNumber(unsigned line) const
{
  if(line<cid_phone_numbers.size()) {
    return cid_phone_numbers[line];
  }
  return QString();
}


void CidMonitor::setPhoneNumber(unsigned line,const QString &pnum)
{
  for(unsigned i=cid_phone_numbers.size();i<=line;i++) {
    cid_phone_numbers.push_back(QString());
  }
  cid_phone_numbers[line]=pnum;
}


QString CidMonitor::phoneName(unsigned line) const
{
  if(line<cid_phone_names.size()) {
    return cid_phone_names[line];
  }
  return QString();
}


void CidMonitor::setPhoneName(unsigned line,const QString &name)
{
  for(unsigned i=cid_phone_names.size();i<=line;i++) {
    cid_phone_names.push_back(QString());
  }
  cid_phone_names[line]=name;
}


QString CidMonitor::warningText(unsigned line) const
{
  if(line<cid_warning_texts.size()) {
    return cid_warning_texts[line];
  }
  return QString();
}


void CidMonitor::setWarningText(unsigned line,const QString &str)
{
  for(unsigned i=cid_warning_texts.size();i<=line;i++) {
    cid_warning_texts.push_back(QString());
  }
  cid_warning_texts[line]=str;
}


MldConfig::RouteAction CidMonitor::routeAction(unsigned line) const
{
  if(line<cid_route_actions.size()) {
    return cid_route_actions[line];
  }
  return MldConfig::Accept;
}


void CidMonitor::setRouteAction(unsigned line,MldConfig::RouteAction action)
{
  for(unsigned i=cid_route_actions.size();i<=line;i++) {
    cid_route_actions.push_back(MldConfig::Accept);
  }
  cid_route_actions[line]=action;
}


int CidMonitor::cidLineMap(int src_line)
{
  if(src_line<(int)cid_line_maps.size()) {
    return cid_line_maps[src_line];
  }
  return src_line;
}


void CidMonitor::setCidLineMap(int src_line,int dest_line)
{
  for(unsigned i=cid_line_maps.size();i<=(unsigned)src_line;i++) {
    cid_line_maps.push_back(i);
  }
  cid_line_maps[src_line]=dest_line;
}


unsigned long long CidMonitor::timer(unsigned line,CidMonitor::CallTimer timer) const
{
  if(line<cid_call_timers.size()) {
    return cid_call_timers[line][timer];
  }
  return 0ll;
}


void CidMonitor::clearTimers(unsigned line)
{
  if(cid_call_timers.size()>line) {
    for(unsigned i=0;i<CidMonitor::LastTimer;i++) {
      cid_call_timers[line][i]=0ll;
    }
  }
}


void CidMonitor::clear()
{
  cid_showcode="";
  cid_driver=NULL;
  cid_connection=NULL;
  cid_connected=false;
  cid_trap=NULL;
  cid_slotlist=NULL;
  cid_start_time=QTime();
  cid_end_time=QTime();
  cid_area_code="";
  cid_kill_calls=false;
  for(unsigned i=0;i<7;i++) {
    cid_dow_active[i]=false;
  }
  cid_detail_ids.clear();
  cid_line_states.clear();
  cid_metadata.clear();
  cid_phone_numbers.clear();
  cid_phone_names.clear();
  cid_warning_texts.clear();
  cid_route_actions.clear();
  cid_line_maps.clear();
  cid_call_timers.clear();
};


void CidMonitor::strobeTimers()
{
  for(unsigned i=0;i<cid_call_timers.size();i++) {
    switch(lineState(i)) {
    case BusDriver::LineElsewhere:
    case BusDriver::LineHandset:
    case BusDriver::LineSpeaker:
    case BusDriver::LineHold:
    case BusDriver::LineScreenedHold:
    case BusDriver::LineNext:
      cid_call_timers[i][CidMonitor::HoldTimer]++;
      break;

    case BusDriver::LineOnAir:
    case BusDriver::LineLocked:
      cid_call_timers[i][CidMonitor::AirTimer]++;
      break;

    case BusDriver::LineRinging:
      cid_call_timers[i][CidMonitor::RingTimer]++;
      break;

    case BusDriver::LineInactive:
    case BusDriver::LineIdle:
    case BusDriver::LineBusied:
      break;
    }
  }
}

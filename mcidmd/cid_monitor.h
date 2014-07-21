// cid_monitor.h
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

#ifndef CID_MONITOR_H
#define CID_MONITOR_H

#include <vector>

#include <QtCore/QString>
#include <QtCore/QDateTime>

#include <mldconfig.h>
#include <bus_driver.h>
#include <connection_data.h>
#include <calleridtrap.h>
#include <call_meta_data.h>
#include <ml_timeslotlist.h>

class CidMonitor
{
 public:
  enum CallTimer {AirTimer=0,HoldTimer=1,RingTimer=2,LastTimer=3};
  CidMonitor();
  QString showCode() const;
  void setShowCode(const QString &showcode);
  BusDriver *busDriver() const;
  void setBusDriver(BusDriver *driver);
  ConnectionData *connection() const;
  void setConnection(ConnectionData *data);
  CallerIdTrap *cidTrap();
  void setCidTrap(CallerIdTrap *trap);
  bool connected() const;
  void setConnected(bool state);
  MlTimeslotList *slotList() const;
  void setSlotList(MlTimeslotList *list);
  QString areaCode() const;
  void setAreaCode(const QString &code);
  bool killCalls() const;
  void setKillCalls(bool state);
  unsigned detailId(unsigned line) const;
  void setDetailId(unsigned line,unsigned id);
  BusDriver::LineState lineState(unsigned line) const;
  void setLineState(unsigned line,BusDriver::LineState lstate);
  CallMetaData *metaData(unsigned line);
  QDateTime startDateTime(unsigned line) const;
  void setStartDateTime(unsigned line,const QDateTime &datetime);
  QString phoneNumber(unsigned line) const;
  void setPhoneNumber(unsigned line,const QString &pnum);
  QString phoneName(unsigned line) const;
  void setPhoneName(unsigned line,const QString &name);
  QString warningText(unsigned line) const;
  void setWarningText(unsigned line,const QString &str);
  MldConfig::RouteAction routeAction(unsigned line) const;
  void setRouteAction(unsigned line,MldConfig::RouteAction action);
  int cidLineMap(int src_line);
  void setCidLineMap(int src_line,int dest_line);
  unsigned long long timer(unsigned line,CidMonitor::CallTimer timer) const;
  void clearTimers(unsigned line);
  void clear();
  void strobeTimers();

 private:
  QString cid_showcode;
  BusDriver *cid_driver;
  ConnectionData *cid_connection;
  CallerIdTrap *cid_trap;
  bool cid_connected;
  MlTimeslotList *cid_slotlist;
  QTime cid_start_time;
  QTime cid_end_time;
  bool cid_dow_active[7];
  QString cid_area_code;
  bool cid_kill_calls;
  std::vector<unsigned> cid_detail_ids;
  std::vector<BusDriver::LineState> cid_line_states;
  std::vector<CallMetaData *> cid_metadata;
  std::vector<QDateTime> cid_start_datetimes;
  std::vector<QString> cid_phone_numbers;
  std::vector<QString> cid_phone_names;
  std::vector<QString> cid_warning_texts;
  std::vector<MldConfig::RouteAction> cid_route_actions;
  std::vector<int> cid_line_maps;
  std::vector<unsigned long long *> cid_call_timers;
};


#endif  // CID_MONITOR_H

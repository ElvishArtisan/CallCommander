// mcidmd.h
//
// The Caller ID Monitoring Daemon for CallCommander
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

#ifndef MCIDMD_H
#define MCIDMD_H

#include <vector>

#include <QtCore/QObject>
#include <QtSql/QSqlDatabase>
#include <Qt3Support/q3socketdevice.h>
#include <QtSql/QSqlQuery>

#include <mltimeengine.h>

#include <mldconfig.h>
#include <calleridtrap.h>

#include "cid_monitor.h"
#include "award_data.h"

#define MCIDMD_KILL_BANK 0
#define MCIDMD_USAGE "[-d]\n\nWhere '-d' places the daemon in \"debug\" mode.\n"

class MainObject : public QObject
{
  Q_OBJECT
 public:
  MainObject(QObject *parent=0,const char *name=0);

 private slots:
  void connectionStartData(int eid);
  void connectionStopData(int eid);
  void currentLineStateData(unsigned id,BusDriver::LineState lstate,
			    unsigned line,int bank,int console);
  void strobeData();
  void receivedStringData(unsigned id,BusDriver::Element elem,unsigned line,
			  const QString &str);
  void callerIdReceivedData(int id,CallerIdDetail *cid);
  void connectionClosedData(unsigned id);
  void watchdogData(unsigned id,bool state);
  void restartTimerData();

 private:
  void ProcessId(CidMonitor *mon,CallerIdDetail *cid);
  void KillCall(CidMonitor *mon,int line,const QString &pnum);
  void CreateActionRecord(CidMonitor *mon,int line,QString pnum);
  void ApplyActionRecord(QSqlQuery *q,CallMetaData *meta,bool use_datetime);
  void IncrementCallerCount(CidMonitor *mon,int line,QString pnum);
  void LogCall(CidMonitor *mon,int line);
  void Init(bool initial_startup=false);
  void Release();
  void StartConnection(CidMonitor *mon);
  void StopConnection(CidMonitor *mon);
  QString GetCanonicalName() const;
  bool AuthenticateReloadPacket(char *b,int n);
  std::vector <CallerIdTrap *> cid_traps;
  std::vector <CidMonitor *> cid_monitors;
  std::vector <AwardData *> cid_awards;
  MLTimeEngine *cid_start_engine;
  MLTimeEngine *cid_stop_engine;
  std::vector<unsigned> cid_eid_index;
  bool cid_debug;
  MldConfig *cid_config;
  QSqlDatabase cid_db;
  Q3SocketDevice *cid_reload_socket;
  QTimer *cid_strobe_timer;
};


#endif  // MCIDMD_H

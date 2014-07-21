// mlmh.h
//
// The CallCommander Logic Module Host
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef MLMH_H
#define MLMH_H

#include <QtCore/QObject>
#include <QtSql/QSqlDatabase>
#include <Qt3Support/q3socketdevice.h>
#include <QtCore/QTimer>

#include <mldconfig.h>
#include <bus_driver.h>
#include <calleriddetail.h>
#include <connection_data.h>
#include <bus_driver.h>
#include <client_data.h>

#define MLMH_SHUTDOWN_POLL_INTERVAL 100
#define MLMH_RECEIVE_POLL_INTERVAL 100
#define MLMH_REQUEST_METADATA_DELAY 5000
#define MLMH_USAGE "[options] <connection> <module-path> <module-args>\nOptions:\n--use-realtime\n     Use realtime permissions.\n"

class MainObject : public QObject
{
  Q_OBJECT
 public:
  MainObject(QObject *parent=0,const char *name=0);

 private slots:
  void watchdogStateChangedData(unsigned id,bool state);
  void currentShowData(unsigned id,const QString &show,bool state);
  void currentConsoleData(unsigned id,int console,ConsoleData::Mode mode);
  void currentLineStateData(unsigned id,BusDriver::LineState lstate,
			    unsigned line,int bank,int console);
  void currentDumpDelayStateData(unsigned id,bool state);
  void currentRecordStateData(unsigned id,bool state);
  void currentBusyAllStateData(unsigned id,bool state);
  void currentScreenStateData(unsigned id,bool state);
  void currentMuteStateData(unsigned id,bool state);
  void currentConferenceStateData(unsigned id,bool state);
  void currentVipStateData(unsigned id,bool state);
  void receivedStringData(unsigned id,BusDriver::Element elem,unsigned line,
			  const QString &str);
  void metadataUpdateRequestedData(unsigned id,ClientData::ClientType type);
  void connectedData(unsigned id,bool state);
  void callbackTimerData(int timernum);
  void shutdownTimerData();
  void exitTimerData();
  void receiveTimerData();
  void requestMetadataData();

 private:
  MldConfig *mlm_config;
  QSqlDatabase mlm_db;
  void *mlm_plugin_handle;
  ConnectionData *mlm_connection;
  QTimer *mlm_shutdown_timer;
  QTimer *mlm_metadata_timer;
  bool mlm_use_realtime;
};

//
// Host Object
//
extern MainObject *global_host;


#endif  // MLMH_H

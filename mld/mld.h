// mld.h
//
// A telephone services server for Call Commander
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

#ifndef MLD_H
#define MLD_H

#include <map>

#include <QtCore/QObject>
#include <Qt3Support/q3serversocket.h>
#include <QtSql/QSqlDatabase>
#include <QtCore/QSignalMapper>
#include <Qt3Support/q3socketdevice.h>

#include <ml.h>
#include <mldconfig.h>

#include <mld_connection.h>
#include <mld_system.h>
#include <mld_socket.h>


class MainObject : public QObject
{
  Q_OBJECT
 public:
  MainObject(QObject *parent=0,const char *name=0);

 private slots:
  void newConnection(int fd);
  void socketKill(int fd);
  void zombieData();
  void readyReadData(int fd);
  void connectedData(unsigned id,bool state);
  void currentShowData(unsigned id,const QString &show_name,bool state);
  void currentConsoleListData(unsigned id,std::vector<ConsoleData *> *data);
  void currentConsoleData(unsigned id,int console,ConsoleData::Mode mode);
  void errorData(unsigned id,BusDriver::Error err);
  void lineStateData(unsigned id,BusDriver::LineState lstate,
		     unsigned line,int bank,int console);
  void busyAllStateData(unsigned id,bool state);
  void dumpDelayStateData(unsigned id,bool state);
  void recordStateData(unsigned id,bool state);
  void screenStateData(unsigned id,bool state);
  void muteStateData(unsigned id,bool state);
  void conferenceStateData(unsigned id,bool state);
  void vipStateData(unsigned id,bool state);
  void receivedStringData(unsigned id,BusDriver::Element elem,unsigned line,
			  const QString &str);
  void restartTimerData();
  void watchdogData(unsigned id,bool state);

 private:
  void ParseLine(int fd);
  void SendCommand(int fd,QString cmd);
  void EchoArgs(int fd,vector<QString> *args,bool state);
  void Init(bool initial_startup=false);
  void Release();
  void RollupArgs(std::vector<QString> *args,unsigned n);
  QString EscapeString(const char &str);
  QString RestoreString(const char &);
  QString GetCanonicalName() const;
  bool AuthenticateReloadPacket(char *b,int n);
  QSqlDatabase mld_database;
  std::map<int,MldConnection *> mld_connections;
  std::map<int,MldSystem *> mld_systems;
  QTimer *mld_zombie_timer;
  MldSocket *mld_server;
  bool mld_debug;
  QString mld_conf_file;
  QSignalMapper *mld_ready_mapper;
  QSignalMapper *mld_kill_mapper;
  Q3SocketDevice *mld_reload_socket;
};


#endif  // MLD_H

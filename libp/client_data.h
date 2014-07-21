// client_data.h
//
// A Container Class for CallCommander Client Data
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

#ifndef CLIENT_DATA_H
#define CLIENT_DATA_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtNetwork/QHostAddress>

#include <console_data.h>

class ClientData
{
 public:
  enum ClientType {TypeUnknown=0,TypeUser=1,TypeCidMonitor=2,
		   TypeAudioRecorder=3,TypeLogicModule=4};
  ClientData();
  QString userName() const;
  void setUserName(const QString &username);
  QHostAddress ipAddress() const;
  void setIpAddress(QHostAddress addr);
  Q_UINT16 tcpPort() const;
  void setTcpPort(Q_UINT16 port);
  QDateTime loginDateTime() const;
  void setLoginDateTime(QDateTime datetime);
  QString consoleName() const;
  void setConsoleName(const QString &name);
  QString location() const;
  void setLocation(const QString &name);
  ConsoleData::Mode consoleMode() const;
  void setConsoleMode(ConsoleData::Mode mode);
  ClientData::ClientType clientType() const;
  void setClientType(ClientData::ClientType type);
  QDateTime lastKeepalive() const;
  void setLastKeepalive(const QDateTime &datetime);
  int version() const;
  QString versionString() const;
  void setVersion(int ver);
  void setVersion(const QString &ver);
  bool segmentSource() const;
  void setSegmentSource(bool state);
  bool load(const QString &buffer);
  QString save() const;
  void clear();
  static QString clientText(ClientData::ClientType type);
  
 private:
  QString client_user_name;
  QHostAddress client_ip_address;
  Q_UINT16 client_tcp_port;
  QDateTime client_login_datetime;
  QString client_console_name;
  QString client_location;
  ConsoleData::Mode client_console_mode;
  ClientData::ClientType client_client_type;
  QDateTime client_last_keepalive;
  int client_version;
  bool client_segment_source;
};

#endif  // CLIENT_DATA_H

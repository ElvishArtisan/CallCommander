// client_data.cpp
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

#define _BSD_SOURCE
#include <time.h>
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif  // WIN32

#include <QtGui/QMessageBox>

#include "client_data.h"

ClientData::ClientData()
{
  clear();
}


QString ClientData::userName() const
{
  return client_user_name;
}


void ClientData::setUserName(const QString &username)
{
  client_user_name=username;
}


QHostAddress ClientData::ipAddress() const
{
  return client_ip_address;
}


void ClientData::setIpAddress(QHostAddress addr)
{
  client_ip_address=addr;
}


Q_UINT16 ClientData::tcpPort() const
{
  return client_tcp_port;
}


void ClientData::setTcpPort(Q_UINT16 port)
{
  client_tcp_port=port;
}


QDateTime ClientData::loginDateTime() const
{
  /*
  struct tm *tm=localtime(&client_login_time);
  QDateTime datetime(QDate(tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday),
		     QTime(tm->tm_hour,tm->tm_min,tm->tm_sec));

  */
/*
  tm=gmtime(&client_login_time);
  QDateTime gm_datetime(QDate(tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday),
			QTime(tm->tm_hour,tm->tm_min,tm->tm_sec));
  QMessageBox::information(NULL,"DEBUG",QString().
  sprintf("ClientData::loginDateTime() -- Local: %s  GMT: %s\n",
	 (const char *)datetime.toString("MM/dd/yyyy - hh:mm:ss"),
	 (const char *)gm_datetime.toString("MM/dd/yyyy - hh:mm:ss")));
*/
  return client_login_datetime;
}


void ClientData::setLoginDateTime(QDateTime datetime)
{
  client_login_datetime=datetime;
}


QString ClientData::consoleName() const
{
  return client_console_name;
}


void ClientData::setConsoleName(const QString &name)
{
  client_console_name=name;
}


QString ClientData::location() const
{
  return client_location;
}


void ClientData::setLocation(const QString &name)
{
  client_location=name;
}


ConsoleData::Mode ClientData::consoleMode() const
{
  return client_console_mode;
}


void ClientData::setConsoleMode(ConsoleData::Mode mode)
{
  client_console_mode=mode;
}


ClientData::ClientType ClientData::clientType() const
{
  return client_client_type;
}


void ClientData::setClientType(ClientData::ClientType type)
{
  client_client_type=type;
}


QDateTime ClientData::lastKeepalive() const
{
  return client_last_keepalive;
}


void ClientData::setLastKeepalive(const QDateTime &datetime)
{
  client_last_keepalive=datetime;
}


int ClientData::version() const
{
  return client_version;
}


QString ClientData::versionString() const
{
  return QString().sprintf("%d.%d.%d",
			   0xFF&(client_version>>16),
			   0xFF&(client_version>>8),
			   0xFF&client_version);
}


void ClientData::setVersion(int ver)
{
  client_version=ver;
}


void ClientData::setVersion(const QString &ver)
{
  int left=ver.find(".");
  int right=ver.findRev(".");
  int first=ver.left(left).toInt();
  QString str=ver.left(right);
  int second=str.right(str.length()-left-1).toInt();
  int third=ver.right(ver.length()-right-1).toInt();

  client_version=((0xFF&first)<<16)+((0xFF&second)<<8)+(0xFF&third);
}


bool ClientData::segmentSource() const
{
  return client_segment_source;
}


void ClientData::setSegmentSource(bool state)
{
  client_segment_source=state;
}


bool ClientData::load(const QString &buffer)
{
  QStringList fields;

  fields=buffer.split("|");
  for(int i=0;i<fields.size();i++) {
    switch(i) {
    case 0:  // User Name
      client_user_name=fields[0];
      break;

    case 1:  // IP Address
      client_ip_address.setAddress(fields[1]);
      break;

    case 2:  // TCP Port
      client_tcp_port=fields[2].toUInt();
      break;

    case 3:  // Login Datetime
      client_login_datetime=
	QDateTime::fromString(fields[3],"yyyy-MM-dd hh:mm:ss");
      break;

    case 4:  // Console Name
      client_console_name=fields[4];
      break;

    case 5:  // Console Mode
      client_console_mode=(ConsoleData::Mode)fields[5].toUInt();
      break;

    case 6:  // Client Type
      client_client_type=(ClientData::ClientType)fields[6].toUInt();
      break;

    case 7:  // Location
      client_location=fields[7];
      break;

    case 8:  // Version
      client_version=fields[8].toInt();
      break;

    case 9:  // Segment Source
      client_segment_source=fields[9].toInt()!=0;
      break;
    }
  }

  return true;
}


QString ClientData::save() const
{
  //  struct tm *tm=gmtime(&client_login_time);
  return QString().sprintf(
    "%s|%s|%u|%s|%s|%u|%u|%s|%d|%d",
    (const char *)client_user_name,
    (const char *)client_ip_address.toString(),
    client_tcp_port,
    (const char *)client_login_datetime.toString("yyyy-MM-dd hh:mm:ss").
    toAscii(),
    (const char *)client_console_name,
    client_console_mode,
    client_client_type,
    (const char *)client_location,
    client_version,
    client_segment_source);
}


void ClientData::clear()
{
  client_user_name="";
  client_ip_address.setAddress("127.0.0.1");
  client_tcp_port=0;
  client_login_datetime=QDateTime();
  client_console_name="";
  client_location="";
  client_console_mode=ConsoleData::Unavailable;
  client_client_type=ClientData::TypeUnknown;
  client_last_keepalive=QDateTime();
  setVersion(VERSION);
  client_segment_source=false;
}


QString ClientData::clientText(ClientData::ClientType type)
{
  QString str;
  switch(type) {
      case ClientData::TypeUnknown:
	str="Unknown";
	break;

      case ClientData::TypeUser:
	str="User";
	break;

      case ClientData::TypeCidMonitor:
	str="CallerID Monitor";
	break;

      case ClientData::TypeAudioRecorder:
	str="Audio Recorder";
	break;

      case ClientData::TypeLogicModule:
	str="Logic Module";
	break;
  }
  return str;
}

// connection_data.h
//
// A Container Class for CallCommander Connection Entries.
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

#ifndef CONNECTION_DATA_H
#define CONNECTION_DATA_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QSettings>

#include <mlttydevice.h>

#include <bus_driver.h>

class ConnectionData
{
 public:
  enum SystemType {TypeUnknown=0,TypeTelos2101=1,TypeTelos100Key=2,
		   TypeTelos100Dim=3,TypeTelos100OneXSix=4,
		   TypeGentnerTs612=5,TypeVirtual=6,TypeAsterisk=7};
  enum StorageType {StorageNone=0,StorageFilesystem=1,StorageDatabase=2};
  ConnectionData(ConnectionData::StorageType storage_type);
  bool exists() const;
  ConnectionData::SystemType systemType() const;
  void setSystemType(ConnectionData::SystemType type);
  int subType() const;
  void setSubType(int subtype);
  ConnectionData::StorageType storageType() const;
  BusDriver::ConnectionType connectionType() const;
  void setConnectionType(BusDriver::ConnectionType type);
  QString name() const;
  void setName(const QString &name);
  QString description() const;
  void setDescription(const QString &desc);
  QString hostName() const;
  void setHostName(const QString &host_name);
  Q_UINT16 tcpPort() const;
  void setTcpPort(Q_UINT16 port);
  QString ttyPort() const;
  void setTtyPort(const QString &port);
  int ttySpeed() const;
  void setTtySpeed(int speed);
  int ttyWordLength() const;
  void setTtyWordLength(int len);
  int ttyStopBits() const;
  void setTtyStopBits(int bits);
  MLTTYDevice::Parity ttyParity() const;
  void setTtyParity(MLTTYDevice::Parity parity);
  QString userName() const;
  void setUserName(const QString &user_name);
  QString userPassword() const;
  void setUserPassword(const QString &passwd);
  QString showName() const;
  void setShowName(const QString &show_name);
  QString showPassword() const;
  void setShowPassword(const QString &passwd);
  QString location() const;
  void setLocation(const QString &host_name);
  int console() const;
  void setConsole(int console);
  QString consoleName() const;
  void setConsoleName(const QString &name);
  bool startConnection() const;
  void setStartConnection(bool state);
  bool offerStartConnection() const;
  void setOfferStartConnection(bool state);
  bool load();
  bool save() const;
  void remove() const;
  void clear();
  static QString defaultFilename(const QString &name);
  static QStringList connections(ConnectionData::StorageType type);
  
 private:
  bool LoadFile(const QString &name);
  bool LoadTable(const QString &name);
  bool SaveFile(const QString &name) const;
  bool SaveTable(const QString &name) const;
  ConnectionData::StorageType data_storage_type;
  ConnectionData::SystemType data_system_type;
  int data_sub_type;
  BusDriver::ConnectionType data_connection_type;
  QString data_name;
  QString data_description;
  QString data_host_name;
  Q_UINT16 data_tcp_port;
  QString data_tty_port;
  int data_tty_speed;
  int data_tty_word_length;
  int data_tty_stop_bits;
  MLTTYDevice::Parity data_tty_parity;
  QString data_user_name;
  QString data_user_passwd;
  QString data_show_name;
  QString data_show_passwd;
  QString data_location;
  int data_console;
  QString data_console_name;
  bool data_start_connection;
  bool data_offer_start_connection;
  QSettings::Scope data_scope;
};

#endif  // CONNECTION_DATA_H

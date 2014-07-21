// virtual_driver.h
//
// A Bus Driver for the CallCommander Virtual Protocol
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

#ifndef VIRTUAL_DRIVER_H
#define VIRTUAL_DRIVER_H

#include <vector>

#include <QtCore/QTimer>
#include <Qt3Support/q3socket.h>

#include <ml.h>
#include <bus_driver.h>

//
// Defines
//
#define VIRTUAL_DRIVER_HEARTBEAT_INTERVAL 5000
#define VIRTUAL_DRIVER_WATCHDOG_TIMEOUT 30000


class VirtualDriver : public BusDriver
{
  Q_OBJECT;

 public:
  VirtualDriver(unsigned id,QObject *parent=0,const char *name=0);
  ~VirtualDriver();
  static QString deviceText();
  static QString defaultUserName();
  static QString defaultUserPassword();
  static BusDriver::ConnectionType connectionType();
  static Q_INT16 defaultTcpPort();
  static QString escapeString(const QString &str);
  static QString restoreString(const QString &str);
  static QString stripQuotes(QString str);
  int totalConnectionSteps();
  void connect(const QString &host_name,const QString &user_name,
	       const QString &user_passwd,Q_UINT16 port,
	       const QString &show_name,const QString &show_passwd,
	       const QString &studio_name,int console,
	       const QString &console_name);
  void connect(const QString &tty_port,
	       const QString &show_name,const QString &show_passwd,
	       const QString &studio_name,int console,
	       const QString &console_name);
  void connectToHost(const QString &host_name,const QString &user_name,
			     const QString &user_passwd,Q_UINT16 port=0);
  void disconnect();
  void requestShowList();
  bool connectToShow(const QString &show_name,const QString &show_passwd,
		     const QString &studio_name);
  void requestConsoleList();
  void connectToConsole(int console,const QString &console_name);
  void requestLineState();
  void selectLine(unsigned line,int console,int bank);
  void hold(int bank);
  void hold(int bank,int console);
  void drop(int bank);
  void drop(int bank,int console);
  void requestBusyAllState();
  void busyAll();
  void requestDumpDelayState();
  void dumpDelay(bool state);
  void requestRecordState();
  void toggleRecord();
  void takeNext(int bank);
  void sendDtmf(char key);
  void flash();
  void screen(bool state);
  void mute(bool state);
  void conference(bool state);
  void vip(bool state);
  void sendString(BusDriver::Element elem,unsigned line,const QString &str);
  void scanDevice();

 private slots:
  void hostFoundData();
  void connectedData();
  void connectionClosedData();
  void errorData(int err);
  void timeoutData();
  void readyReadData();
  void watchdogData();
  void heartbeatData();

 private:
  void ParseLine();
  void SendCommand(const QString &cmd);
  void RollupArgs(std::vector<QString> *args,unsigned n);
#ifdef WIN32
  int iscntrl(int c);
#endif  // WIN32
  Q3Socket *bus_socket;
  QTimer *bus_socket_timer;
  char bus_buffer[ML_VIRTUAL_MAX_MSG_LENGTH];
  unsigned bus_ptr;
  QString bus_hostname;
  QString bus_username;
  QString bus_userpasswd;
  Q_UINT16 bus_port;
  QString bus_showname;
  QString bus_showpasswd;
  int bus_console;
  QString bus_console_name;
  std::vector<ShowData *> bus_show_data;
  std::vector<ConsoleData *> bus_console_data;
  QTimer *bus_heartbeat_timer;
  QTimer *bus_watchdog_timer;
  bool bus_watchdog_running;
  QString bus_show_name;
  QString bus_show_passwd;
  QString bus_studio_name;
};


#endif  // VIRTUAL_DRIVER_H

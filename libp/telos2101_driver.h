// telos2101_driver.h
//
// Abstract Base Network Protocol Driver for CallCommander
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

#ifndef TELOS2101_DRIVER_H
#define TELOS2101_DRIVER_H

#include <vector>

#include <QtCore/QTimer>
#include <Qt3Support/q3socketdevice.h>

#include <tlsaplib.h>

#include <bus_driver.h>

//
// Defines
//
#define TELOS2101_DRIVER_TICK_INTERVAL 1
#define TELOS2101_DRIVER_SESSION_ID 0
#define TELOS2101_DRIVER_DEFAULT_USER_NAME "tlsap"
#define TELOS2101_DRIVER_DEFAULT_USER_PASSWORD "Telos"
#define TELOS2101_DRIVER_SHOW_TIMEOUT 5000
#define TELOS2101_DRIVER_VIRTUAL_CONSOLE_NAME "Virtual Director"
#define TELOS2101_DRIVER_HEARTBEAT_INTERVAL 5000
#define TELOS2101_DRIVER_WATCHDOG_TIMEOUT 30000

//
// Callback Stuff
//
extern TlsAP_Callbacks ph_callbacks;


class Telos2101Driver : public BusDriver
{
  Q_OBJECT;

 public:
  Telos2101Driver(unsigned id,QObject *parent=0,const char *name=0);
  ~Telos2101Driver();
  static QString deviceText();
  static QString defaultUserName();
  static QString defaultUserPassword();
  static BusDriver::ConnectionType connectionType();
  static Q_INT16 defaultTcpPort();
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
  void showListData(const QString &show_name,const QString &host_name,
		    bool is_active,bool passwd_reqd,bool is_last);
  void showInfoData(unsigned max_lines,unsigned max_hybrids);
  void consoleListData(const QString &console_name,ConsoleData::Mode mode,
		       bool is_free,bool is_last);
  void lineStateData(int line,int bank,int line_state);
  void modeData(int mode);
  void recordData(int state);
  void busyAllData(int state);
  void delayDumpData(int state);
  void callerIdData(int line,const char *pnum);
  void textData(int type,int line,const QString &str);
  void errorData(long err_num,const char *err_text);

 private slots:
  void messageScanData();
  void showTimerData();
  void watchdogTimerData();
  void heartbeatTimerData();

 private:
  bool Init();
  TlsAP_Client bus_client;
  Q3SocketDevice *bus_socket;
  QTimer *bus_message_timer;
  QTimer *bus_heartbeat_timer;
  QTimer *bus_watchdog_timer;
  bool bus_watchdog_active;
  bool bus_connected;
  QString bus_hostname;
  QString bus_username;
  QString bus_userpasswd;
  Q_UINT16 bus_port;
  QString bus_showname;
  QString bus_showpasswd;
  QString bus_studioname;
  int bus_console;
  std::vector<ShowData *> bus_shows;
  std::vector<ConsoleData *> bus_consoles;
  bool bus_console_found;
  QTimer *bus_show_timer;
  bool bus_current_show_auth;
};


#endif  // TELOS2101_DRIVER_H

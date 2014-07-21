// asterisk_driver.h
//
// A CallCommander Bus Protocol Driver for the Asterisk Manager Interface
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef ASTERISK_DRIVER_H
#define ASTERISK_DRIVER_H

#include <queue>
#include <vector>

#include <QtCore/QTimer>
#include <QtCore/QDateTime>

#include <ml.h>
#include <bus_driver.h>
#include <mlami.h>
#include <mlasteriskconfig.h>

#define ASTERISK_DUMP_LENGTH 500
#define ASTERISK_WATCHDOG_PAUSE 5
#define ASTERISK_DTMF_KEY_INTERVAL 200
#define ASTERISK_SOCKET_INTERVAL 100

class AsteriskDriver : public BusDriver
{
  Q_OBJECT;

 public:
  AsteriskDriver(unsigned id,QObject *parent=0,const char *name=0);
  ~AsteriskDriver();
  static QString deviceText();
  static BusDriver::ConnectionType connectionType();
  static Q_INT16 defaultTcpPort();
  static QString escapeString(const QString &str);
  static QString restoreString(const QString &str);
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
  void connectToHost(const QString &host_name,
		     const QString &user_name,
		     const QString &user_passwd,Q_UINT16 port);
  void disconnect();
  void requestShowList();
  bool connectToShow(const QString &show_name,
			     const QString &show_passwd,
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
  void connectedData(unsigned ami_id,bool state);
  void actionReceivedData(unsigned ami_id,unsigned action_id,
			  std::vector<QString> *labels,
			  std::vector<QString> *values);
  void eventReceivedData(unsigned ami_id,
			 std::vector<QString> *labels,
			 std::vector<QString> *values);

 private:
  void Park(unsigned line,const QString &chan);
  void Hangup(const QString &chan);
  void SeizeLine(unsigned line,int bank,int console);
  void EmitLineState(unsigned line);
  void UpdateNextLine();
  int bus_console;
  std::vector<ConsoleData *> bus_console_data;
  MLAmi *bus_ami;
  QString bus_hostname;
  Q_UINT16 bus_port;
  QString bus_username;
  QString bus_userpasswd;
  QString bus_console_name;
  BusDriver::LineState bus_line_state[MAX_LINES];
  int bus_line_bank[MAX_LINES];
  int bus_line_console[MAX_LINES];
  QDateTime bus_line_datetime[MAX_LINES];
  int bus_next_line;
  MLAsteriskConfig *bus_askconfig;
  unsigned bus_init_action_id;
};


#endif  // ASTERISK_DRIVER_H

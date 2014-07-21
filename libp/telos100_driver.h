// telos100_driver.h
//
// A CallCommander Bus Protocol Driver for Telos 100 Phone Systems
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

#ifndef TELOS100_DRIVER_H
#define TELOS100_DRIVER_H

#include <queue>

#include <QtCore/QTimer>

#include <mlttydevice.h>

#include <bus_driver.h>
#include <telos100_states.h>

#define TELOS100_TTY_SPEED 1200
#define TELOS100_DUMP_LENGTH 500
#define TELOS100_WATCHDOG_PAUSE 5
#define TELOS100_DTMF_KEY_INTERVAL 200

class Telos100Driver : public BusDriver
{
  Q_OBJECT;

 public:
  enum SubType {KeyType=0,DimType=1,OneXSixType=2};
  Telos100Driver(Telos100Driver::SubType type,unsigned id,
		 QObject *parent=0,const char *name=0);
  ~Telos100Driver();
  static QString deviceText();
  static QString deviceText(Telos100Driver::SubType type);
  static BusDriver::ConnectionType connectionType();
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
  void connectToSerial(const QString &tty_port);
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
  void receiveTty();
  void dumpData();
  void dtmfKeyTimeoutData();

 private:
  void DispatchStatus(char cmd,int line);
  void SendState(int line);
  void WatchdogRestart();
  Telos100Driver::SubType bus_subtype;
  MLTTYDevice *bus_tty_device;
  Telos100States *bus_states;
  int bus_console;
  char bus_cmd;
  char bus_istate;
  bool bus_record_state;
  QTimer *bus_dump_timer;
  bool bus_watchdog_active;
  QTimer *bus_dtmf_key_timer;
  std::queue<char> bus_dtmf_keys;
};


#endif  // TELOS100_DRIVER_H

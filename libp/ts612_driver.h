// ts612_driver.h
//
// A CallCommander Bus Protocol Driver for Gentner TS-612 Systems.
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

#ifndef TS612_DRIVER_H
#define TS612_DRIVER_H

#include <QtCore/QTimer>

#include <mlttydevice.h>

#include <bus_driver.h>

#define TS612_TTY_SPEED 9600
#define TS612_TTY_INTERVAL 50
#define TS612_TTY_DELAY 10000
#define TS612_DUMP_LENGTH 500
#define TS612_MAX_LINES 12
#define TS612_WATCHDOG_INTERVAL 10000


class Ts612Driver : public BusDriver
{
  Q_OBJECT;

 public:
  Ts612Driver(unsigned id,QObject *parent=0,const char *name=0);
  ~Ts612Driver();
  static QString deviceText();
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
  void watchdogData();

 private:
  void DispatchStatus(int cmd,int line);
  void SendState(int line);
  MLTTYDevice *bus_tty_device;
  unsigned char bus_raw_button[32];
  bool bus_record_state;
  bool bus_busy_state;
  bool bus_screen_state;
  bool bus_mute_state;
  bool bus_conf_state;
  bool bus_vip_state;
  char bus_state[TS612_MAX_LINES];
  QTimer *bus_dump_timer;
  QTimer *bus_watchdog_timer;
  int bus_console;
  char bus_cmd;
  char bus_istate;
};


#endif  // TS612_DRIVER_H

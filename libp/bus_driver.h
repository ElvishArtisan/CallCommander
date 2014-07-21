// bus_driver.h
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

#ifndef BUS_DRIVER_H
#define BUS_DRIVER_H

#include <vector>

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include <show_data.h>
#include <console_data.h>
#include <client_data.h>


class BusDriver : public QObject
{
  Q_OBJECT;

 public:
  enum Error {NoError=0,ConnectionRefused=1,HostNotFound=2,
	      ReadError=3,UnknownError=4};
  enum LineState {LineInactive=0,LineIdle=1,LineElsewhere=2,LineHandset=3,
		  LineSpeaker=4,LineHold=5,LineScreenedHold=6,
		  LineOnAir=7,LineRinging=8,LineLocked=9,LineBusied=10,
		  LineNext=11};
  enum ConnectionType {UnknownConnection=0,TcpConnection=1,SerialConnection=2};
  enum Element {MessageGlobalElement=1,LookupGlobalElement=2,
		LineWarningGlobalElement=3,HeartbeatGlobalElement=4,
		ClientKeepAliveGlobalElement=5,
		ClientLeaveGlobalElement=6,RequestMetadataGlobalElement=7,
		DelayOffsetGlobalElement=8,ClientKillGlobalElement=9,
		LocalAreaCodeGlobalElement=10,SegclockGlobalElement=11,
		ClearMessageGlobalElement=12,
		NameElement=256,Addr1Element=257,Addr2Element=258,
		CityElement=259,StateElement=260,ZipElement=261,
		PhoneElement=262,EmailElement=263,GenderElement=264,
		StationElement=265,CommentElement=266,CellphoneElement=267,
		QualityElement=268,CounterElement=269,AgeElement=270,
		RingTimerElement=271,AirTimerElement=272,HoldTimerElement=273,
		LastCallDatetimeElement=274,LastOnAirDatetimeElement=275,
		LastAwardDatetimeElement=276,DetailIdElement=277,
		FirstNameAwardElement=278,LastNameAwardElement=279,
		Addr1AwardElement=280,Addr2AwardElement=281,
		CityAwardElement=282,StateAwardElement=283,
		ZipAwardElement=284,PhoneAwardElement=285,
		EmailAwardElement=286,GenderAwardElement=287,
		AgeAwardElement=288,PrizeAwardElement=289,
		RemarksAwardElement=290,NumberAwardElement=291,
		UsernameAwardElement=292,DetailIdAwardElement=293,
		CommitAwardElement=294,LastElement=295};
  BusDriver(unsigned id,QObject *parent=0,const char *name=0);
  ~BusDriver();
  unsigned id() const;
  bool dumpStrings() const;
  void setDumpStrings(bool state);
  static QString deviceText();
  static QString errorText(BusDriver::Error err);
  static QString defaultUserName();
  static QString defaultUserPassword();
  static BusDriver::ConnectionType connectionType();
  static Q_INT16 defaultTcpPort();
  std::vector <ClientData *> *clientList();
  void setClientType(ClientData::ClientType type);
  void setSegmentSource(bool state);
  void setPromiscuousMode(bool state);
  void requestMetadataUpdate(ClientData *data);
  virtual int totalConnectionSteps();
  QString userName() const;
  virtual void connect(const QString &host_name,const QString &user_name,
		       const QString &user_passwd,Q_UINT16 port,
		       const QString &show_name,const QString &show_passwd,
		       const QString &studio_name,int console,
		       const QString &console_name);
  virtual void connect(const QString &tty_port,
		       const QString &show_name,const QString &show_passwd,
		       const QString &studio_name,int console,
		       const QString &console_name);
  virtual void connectToHost(const QString &host_name,const QString &user_name,
			     const QString &user_passwd,Q_UINT16 port=0);
  virtual void connectToSerial(const QString &tty_port);
  virtual void disconnect()=0;
  virtual void requestShowList()=0;
  virtual bool connectToShow(const QString &show_name,
			     const QString &show_passwd,
			     const QString &studio_name);
  virtual void requestConsoleList()=0;
  virtual void connectToConsole(int console,const QString &console_name);
  virtual void requestLineState()=0;
  virtual void selectLine(unsigned line,int console,int bank)=0;
  virtual void hold(int bank)=0;
  virtual void hold(int bank,int console)=0;
  virtual void drop(int bank)=0;
  virtual void drop(int bank,int console)=0;
  virtual void requestBusyAllState()=0;
  virtual void busyAll()=0;
  virtual void requestDumpDelayState()=0;
  virtual void dumpDelay(bool state)=0;
  virtual void requestRecordState()=0;
  virtual void toggleRecord()=0;
  virtual void takeNext(int bank)=0;
  virtual void sendDtmf(char key)=0;
  virtual void flash()=0;
  virtual void screen(bool state)=0;
  virtual void mute(bool state)=0;
  virtual void conference(bool state)=0;
  virtual void vip(bool state)=0;
  virtual void sendString(BusDriver::Element elem,unsigned line,
			  const QString &str)=0;
  virtual void scanDevice()=0;

 signals:
  void connected(unsigned id,bool state);
  void connectionClosed(unsigned id);
  void error(unsigned id,BusDriver::Error err);
  void watchdogStateChanged(unsigned id,bool state);
  void currentShowList(unsigned id,std::vector<ShowData *> *show_list);
  void currentShow(unsigned id,const QString &show,bool state);
  void currentConsoleList(unsigned id,
			  std::vector<ConsoleData *> *console_list);
  void currentConsole(unsigned id,int console,ConsoleData::Mode mode);
  void currentLineState(unsigned id,BusDriver::LineState lstate,
			unsigned line,int bank,int console);
  void currentDumpDelayState(unsigned id,bool state);
  void currentRecordState(unsigned id,bool state);
  void currentBusyAllState(unsigned id,bool state);
  void currentScreenState(unsigned id,bool state);
  void currentMuteState(unsigned id,bool state);
  void currentConferenceState(unsigned id,bool state);
  void currentVipState(unsigned id,bool state);
  void receivedString(unsigned id,BusDriver::Element elem,unsigned line,
		      const QString &str);
  void clientListChanged(unsigned id,std::vector <ClientData *> *list);
  void metadataUpdateRequested(unsigned id,ClientData::ClientType type);
  void connectionStepChanged(unsigned id,int step);

 protected:
  void updateConnectionState(bool state);
  void processString(BusDriver::Element elem,unsigned line,const QString &str);
  std::vector <ClientData *> bus_clients;

 private slots:
  void keepaliveTimerData();

 private:
  void SyncMetadata(BusDriver::Element elem,const QString &str);
  void SyncClient(BusDriver::Element elem,const QString &str);
  void RemoveClient(BusDriver::Element elem,const QString &str);
  void KillClient(BusDriver::Element elem,const QString &str);
  unsigned bus_id;
  bool bus_dump_strings;
  QTimer *bus_keepalive_timer;
  bool bus_connected;
  bool bus_promiscuous_mode;
};


#endif  // BUS_DRIVER_H

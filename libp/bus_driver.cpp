// bus_driver.cpp
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

#include <stdlib.h>

#include <stdio.h>

#include "ml.h"
#include "bus_driver.h"

BusDriver::BusDriver(unsigned id,QObject *parent,const char *name)
  : QObject(parent,name)
{
  bus_id=id;
  bus_dump_strings=false;
  bus_connected=false;
  bus_promiscuous_mode=false;
  bus_clients.push_back(new ClientData());

  //
  // KeepAlive Timer
  //
  bus_keepalive_timer=new QTimer(this,"bus_keepalive_timer");
  QObject::connect(bus_keepalive_timer,SIGNAL(timeout()),
		   this,SLOT(keepaliveTimerData()));
  bus_keepalive_timer->start(ML_SEND_CLIENT_KEEPALIVE_INTERVAL);
}


BusDriver::~BusDriver()
{
}


unsigned BusDriver::id() const
{
  return bus_id;
}


bool BusDriver::dumpStrings() const
{
  return bus_dump_strings;
}


void BusDriver::setDumpStrings(bool state)
{
  bus_dump_strings=state;
}


QString BusDriver::deviceText()
{
  return QString("Generic Bus Driver");
}


QString BusDriver::errorText(BusDriver::Error err)
{
  QString str;
  switch(err) {
      case BusDriver::NoError:
	str=tr("OK");
	break;

      case BusDriver::ConnectionRefused:
	str=tr("Connection refused");
	break;

      case BusDriver::HostNotFound:
	str=tr("Host not found");
	break;

      case BusDriver::ReadError:
	str=tr("Read error");
	break;

      case BusDriver::UnknownError:
	str=tr("Unknown error");
	break;
  }
  return str;
}


QString BusDriver::defaultUserName()
{
  return QString();
}


QString BusDriver::defaultUserPassword()
{
  return QString();
}


Q_INT16 BusDriver::defaultTcpPort()
{
  return 0;
}


std::vector <ClientData *> *BusDriver::clientList()
{
  return &bus_clients;
}


void BusDriver::setClientType(ClientData::ClientType type)
{
  bus_clients[0]->setClientType(type);
}


void BusDriver::setSegmentSource(bool state)
{
  bus_clients[0]->setSegmentSource(state);
}


void BusDriver::setPromiscuousMode(bool state)
{
  bus_promiscuous_mode=state;
}


void BusDriver::requestMetadataUpdate(ClientData *data)
{
  if(data==NULL) {  // Send an empty packet
    ClientData *d=new ClientData();
    sendString(BusDriver::RequestMetadataGlobalElement,0,d->save());
    delete d;
  }
  else {
    sendString(BusDriver::RequestMetadataGlobalElement,0,data->save());
  }
}


BusDriver::ConnectionType BusDriver::connectionType()
{
  return BusDriver::UnknownConnection;
}


int BusDriver::totalConnectionSteps()
{
  return 1;
}


QString BusDriver::userName() const
{
  return bus_clients[0]->userName();
}


void BusDriver::connect(const QString &host_name,
			const QString &user_name,
			const QString &user_passwd,Q_UINT16 port,
			const QString &show_name,
			const QString &show_passwd,
			const QString &studio_name,int console,
			const QString &console_name)
{
  bus_clients[0]->setUserName(user_name);
  bus_clients[0]->setLocation(studio_name);
  bus_clients[0]->setConsoleName(console_name);
  bus_clients[0]->
    setLoginDateTime(QDateTime(QDate::currentDate(),QTime::currentTime()));
}


void BusDriver::connect(const QString &tty_port,
			const QString &show_name,
			const QString &show_passwd,
			const QString &studio_name,int console,
			const QString &console_name)
{
  if(getenv("USER")!=NULL) {
    bus_clients[0]->setUserName(getenv("USER"));
  }
  else {
    bus_clients[0]->setUserName(ML_DEFAULT_LOGIN_NAME);
  }
  bus_clients[0]->setLocation(studio_name);
  bus_clients[0]->setConsoleName(console_name);
  bus_clients[0]->setLocation(studio_name);
  bus_clients[0]->
    setLoginDateTime(QDateTime(QDate::currentDate(),QTime::currentTime()));
}


void BusDriver::connectToHost(const QString &host_name,
			      const QString &user_name,
			      const QString &user_passwd,Q_UINT16 port)
{
  bus_clients[0]->setUserName(user_name);
  bus_clients[0]->
    setLoginDateTime(QDateTime(QDate::currentDate(),QTime::currentTime()));
}


void BusDriver::connectToSerial(const QString &tty_port)
{
  if(getenv("USER")!=NULL) {
    bus_clients[0]->setUserName(getenv("USER"));
  }
  else {
    bus_clients[0]->setUserName(ML_DEFAULT_LOGIN_NAME);
  }
  bus_clients[0]->
    setLoginDateTime(QDateTime(QDate::currentDate(),QTime::currentTime()));
}


bool BusDriver::connectToShow(const QString &show_name,
			      const QString &show_passwd,
			      const QString &studio_name)
{
  bus_clients[0]->setLocation(studio_name);
  return true;
}


void BusDriver::connectToConsole(int console,const QString &console_name)
{
  bus_clients[0]->setConsoleName(console_name);
}


void BusDriver::updateConnectionState(bool state)
{
//  printf("updateConnectionState(%d)\n",state);
  if(state) {
    sendString(BusDriver::ClientKeepAliveGlobalElement,0,
	       bus_clients[0]->save());
  }
  else {
    sendString(BusDriver::ClientLeaveGlobalElement,0,bus_clients[0]->save());
  }
  emit clientListChanged(bus_id,&bus_clients);
}


void BusDriver::processString(BusDriver::Element elem,
			      unsigned line,const QString &str)
{
  if(dumpStrings()) {
    printf("RECV: Line:%u  Elem:%u  String:\"%s\"\n",line,elem,
	   (const char *)str.toAscii());
  }
  switch(elem) {
    case BusDriver::HeartbeatGlobalElement:
      break;
      
    case BusDriver::ClientKeepAliveGlobalElement:
      SyncClient(elem,str);
      break;
      
    case BusDriver::ClientLeaveGlobalElement:
      RemoveClient(elem,str);
      break;
      
    case BusDriver::RequestMetadataGlobalElement:
      SyncMetadata(elem,str);
      emit receivedString(bus_id,elem,line,str);
      break;
      
    case BusDriver::ClientKillGlobalElement:
      KillClient(elem,str);
      break;
      
    default:
      emit receivedString(bus_id,elem,line,str);
      break;
  }
}


void BusDriver::keepaliveTimerData()
{
  bool clients_changed=false;
  sendString(BusDriver::ClientKeepAliveGlobalElement,0,bus_clients[0]->save());
  QDateTime current_datetime(QDate::currentDate(),QTime::currentTime());
  for(unsigned i=1;i<bus_clients.size();i++) {
    if(bus_clients[i]->lastKeepalive().secsTo(current_datetime)>
       ML_MAX_CLIENT_KEEPALIVE_INTERVAL) {
      std::vector<ClientData *>::iterator it=bus_clients.begin()+i;
      bus_clients.erase(it,it+1);
      clients_changed=true;
    }
  }
  if(clients_changed) {
    emit clientListChanged(bus_id,&bus_clients);
  }
}


void BusDriver::SyncMetadata(BusDriver::Element elem,const QString &str)
{
  ClientData *data=new ClientData();
  if(!data->load(str)) {
    fprintf(stderr,"BusDriver: corrupt client sync message received\n");
    return;
  }

  //
  // Is this for us?
  //
  if(((data->ipAddress()==bus_clients[0]->ipAddress())&&
     (data->tcpPort()==bus_clients[0]->tcpPort()))||
     bus_promiscuous_mode) {
    emit metadataUpdateRequested(bus_id,data->clientType());
  }
  delete data;
  return;
}


void BusDriver::SyncClient(BusDriver::Element elem,const QString &str)
{
  QDateTime current_datetime(QDate::currentDate(),QTime::currentTime());
  ClientData *data=new ClientData();
  if(!data->load(str)) {
    fprintf(stderr,"BusDriver: corrupt client sync message received\n");
    return;
  }

  // printf("IP ADDR: %s:%u\n",(const char *)data->ipAddress().toString(),data->tcpPort());
  //
  // Do we exist?
  //
  for(unsigned i=1;i<bus_clients.size();i++) {
    if((data->ipAddress()==bus_clients[i]->ipAddress())&&
       (data->tcpPort()==bus_clients[i]->tcpPort())) {
      bus_clients[i]->setLastKeepalive(current_datetime);
      //
      // Check for Changes
      //
      if((data->loginDateTime()!=bus_clients[i]->loginDateTime())||
	 (data->consoleName()!=bus_clients[i]->consoleName())||
	 (data->location()!=bus_clients[i]->location())||
	 (data->consoleMode()!=bus_clients[i]->consoleMode())||
	 (data->clientType()!=bus_clients[i]->clientType())) {
	*bus_clients[i]=*data;
	emit clientListChanged(bus_id,&bus_clients);
      }
      delete data;
      return;
    }
  }
  delete data;

  //
  // If not, create the entry
  //
  bus_clients.push_back(new ClientData());
  bus_clients.back()->load(str);
  bus_clients.back()->setLastKeepalive(current_datetime);
  emit clientListChanged(bus_id,&bus_clients);
}


void BusDriver::RemoveClient(BusDriver::Element elem,const QString &str)
{
  ClientData *data=new ClientData();
  if(!data->load(str)) {
    fprintf(stderr,"BusDriver: corrupt client sync message received\n");
    return;
  }

  //
  // Find the entry
  //
  for(unsigned i=1;i<bus_clients.size();i++) {
    if((data->ipAddress()==bus_clients[i]->ipAddress())&&
       (data->tcpPort()==bus_clients[i]->tcpPort())) {
      std::vector<ClientData *>::iterator it=bus_clients.begin()+i;
      bus_clients.erase(it,it+1);
      delete data;
      emit clientListChanged(bus_id,&bus_clients);
      return;
    }
  }
  fprintf(stderr,
	  "BusDriver: tried to remove non-existent client entry for %s:%u\n",
	  (const char *)data->ipAddress().toString(),data->tcpPort());
  delete data;
}


void BusDriver::KillClient(BusDriver::Element elem,const QString &str)
{
  bool ok=false;

  QHostAddress addr;
  addr.setAddress(str.left(str.find(":")));
  if(addr.isNull()) {
    return;
  }
  Q_UINT16 port=str.right(str.length()-str.find(":")-1).toInt(&ok);
  if(!ok) {
    return;
  }
  if((bus_clients[0]->ipAddress()==addr)||(bus_clients[0]->tcpPort()==port)) {
    exit(0);
  }
}

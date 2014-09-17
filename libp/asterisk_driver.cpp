// asterisk_driver.cpp
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

//#include <sys/socket.h>
//#include <sys/un.h>
#include <sys/types.h>

#include "ml.h"
#include "asterisk_driver.h"

AsteriskDriver::AsteriskDriver(unsigned id,QObject *parent,const char *name)
  : BusDriver(id,parent,name)
{
  bus_next_line=-1;
  bus_console=ML_MAX_CONSOLES;
  for(unsigned i=0;i<MAX_LINES;i++) {
    bus_line_state[i]=BusDriver::LineInactive;
    bus_line_bank[i]=255;
    bus_line_console[i]=255;
  }

  //
  // AMI Connector
  //
  bus_ami=new MLAmi(0,this,"bus_ami");
  QObject::connect(bus_ami,SIGNAL(connected(unsigned,bool)),
		   this,SLOT(connectedData(unsigned,bool)));
  QObject::connect(bus_ami,SIGNAL(actionReceived(unsigned,unsigned,
						 std::vector<QString> *,
						 std::vector<QString> *)),
		   SLOT(actionReceivedData(unsigned,unsigned,
					   std::vector<QString> *,
					   std::vector<QString> *)));
  QObject::connect(bus_ami,SIGNAL(eventReceived(unsigned,
						std::vector<QString> *,
						std::vector<QString> *)),
		   SLOT(eventReceivedData(unsigned,
					  std::vector<QString> *,
					  std::vector<QString> *)));

  //
  // Asterisk Configuration
  //
  bus_askconfig=new MLAsteriskConfig();
}


AsteriskDriver::~AsteriskDriver()
{
}


QString AsteriskDriver::deviceText()
{
  return "Asterisk Open Source PBX";
}


BusDriver::ConnectionType AsteriskDriver::connectionType()
{
  return BusDriver::TcpConnection;
}


Q_INT16 AsteriskDriver::defaultTcpPort()
{
  return ML_DEFAULT_AMI_TCP_PORT;
}


QString AsteriskDriver::escapeString(const QString &str)
{
  QString ret;

  for(int i=0;i<str.length();i++) {
    switch(((const char *)str)[i]) {
	case ' ':
	case '!':
	case '\\':
	  ret+="\\";
	  ret+=str.at(i);
	  break;

	default:
	  ret+=str.at(i);
	  break;
    }
  }
  return ret;
}


QString AsteriskDriver::restoreString(const QString &str)
{
  bool esc=false;
  QString ret;

  for(int i=0;i<str.length();i++) {
    switch(((const char *)str)[i]) {    
	case '\\':
	  if(esc) {
	    ret+="\\";
	    esc=false;
	  }
	  else {
	    esc=true;
	  }
	  break;

      default:
	ret+=str.at(i);
	esc=false;
	break;
    }
  }

  return ret;
}


int AsteriskDriver::totalConnectionSteps()
{
  return 2;
}


void AsteriskDriver::connect(const QString &host_name,const QString &user_name,
			     const QString &user_passwd,Q_UINT16 port,
			     const QString &show_name,const QString &show_passwd,
			     const QString &studio_name,int console,
			     const QString &console_name)
{
  bus_hostname=host_name;
  bus_username=user_name;
  bus_userpasswd=user_passwd;
  bus_port=port;
  bus_console=console;
  bus_console_name=console_name;
  BusDriver::connect(host_name,user_name,user_passwd,port,show_name,
		     show_passwd,studio_name,console,console_name);
}


void AsteriskDriver::connect(const QString &tty_port,
			     const QString &show_name,
			     const QString &show_passwd,
			     const QString &studio_name,int console,
			     const QString &console_name)
{
}


void AsteriskDriver::connectToHost(const QString &host_name,
				   const QString &user_name,
				   const QString &user_passwd,Q_UINT16 port)
{
  bus_hostname=host_name;
  bus_username=user_name;
  bus_userpasswd=user_passwd;
  bus_port=port;

  emit connectionStepChanged(id(),1);
  bus_ami->connectToHost(bus_hostname,user_name,user_passwd,port);
//  bus_socket_timer->start(VIRTUAL_DRIVER_WATCHDOG_TIMEOUT,true);
  BusDriver::connectToHost(host_name,user_name,user_passwd,port);
}


void AsteriskDriver::disconnect()
{
  emit connectionClosed(id());
}


void AsteriskDriver::requestShowList()
{
  std::vector<ShowData *> show_data;
  show_data.push_back(new ShowData("Asterisk","Asterisk",true,false));
  emit currentShowList(id(),&show_data);
  delete show_data.back();
}


bool AsteriskDriver::connectToShow(const QString &show_name,
				   const QString &show_passwd,
				   const QString &studio_name)
{
  BusDriver::connectToShow(show_name,show_passwd,studio_name);
  emit currentShow(id(),show_name,true);
  return true;
}


void AsteriskDriver::requestConsoleList()
{
  std::vector<ConsoleData *> console_data;

  console_data.push_back(new ConsoleData("Screener 1",0,
					 ConsoleData::Producer,true));
  console_data.push_back(new ConsoleData("Screener 2",1,
					 ConsoleData::Producer,true));
  console_data.push_back(new ConsoleData("On Air",ML_MAX_CONSOLES,
					 ConsoleData::Talent,true));
  emit currentConsoleList(id(),&console_data);
  for(unsigned i=0;i<console_data.size();i++) {
    delete console_data[i];
  }
}


void AsteriskDriver::connectToConsole(int console,const QString &console_name)
{
  bus_console=console;
  BusDriver::connectToConsole(console,console_name);
  if(console==ML_MAX_CONSOLES) {
    bus_clients[0]->setConsoleMode(ConsoleData::Talent);
    emit currentConsole(id(),console,ConsoleData::Talent);
  }
  else {
    bus_clients[0]->setConsoleMode(ConsoleData::Producer);
    emit currentConsole(id(),console,ConsoleData::Producer);
  }
  updateConnectionState(true);
}


void AsteriskDriver::requestLineState()
{
  for(unsigned i=0;i<MAX_LINES;i++) {
    EmitLineState(i);
  }
}
 

void AsteriskDriver::selectLine(unsigned line,int console,int bank)
{
  QString exten=bus_askconfig->extension(bank,console);
  if(exten.isEmpty()) {
    return;
  }
  QString context=bus_askconfig->context(bank,console);
  if(context.isEmpty()) {
    return;
  }
  QString chan=bus_askconfig->line2Asterisk(line);
  if(chan.isEmpty()) {
    return;
  }
  std::vector<QString> labels;
  std::vector<QString> values;

  //
  // Hang up abandoned connections
  //
  if(console==ML_MAX_CONSOLES) {
    for(unsigned i=0;i<MAX_LINES;i++) {
      if((line!=i)&&(bus_line_state[i]==BusDriver::LineOnAir)&&\
	 (bus_line_bank[i]==bank)) {
	drop(bank,console);
	bus_line_state[i]=BusDriver::LineIdle;
	bus_line_bank[i]=255;
	bus_line_console[i]=255;
	EmitLineState(i);
      }
    }
  }
  else {
    for(unsigned i=0;i<MAX_LINES;i++) {
      if((line!=i)&&((bus_line_state[i]==BusDriver::LineHandset)||
		     (bus_line_state[i]==BusDriver::LineSpeaker))&&	\
	 (bus_line_console[i]==console)) {
	drop(bank,console);
	bus_line_state[i]=BusDriver::LineIdle;
	bus_line_bank[i]=255;
	bus_line_console[i]=255;
	EmitLineState(i);
      }
    }
  }

  //
  // Seize New Line
  //
  switch(bus_line_state[line]) {
    case BusDriver::LineRinging:
      labels.push_back("Channel");
      values.push_back(chan);

      labels.push_back("Exten");
      values.push_back(exten);

      labels.push_back("Context");
      values.push_back(context);

      labels.push_back("Priority");
      values.push_back("1");

      bus_ami->sendAction("Redirect",&labels,&values);

      SeizeLine(line,bank,console);
      break;

    case BusDriver::LineIdle:
      bus_line_datetime[line]=
	QDateTime(QDate::currentDate(),QTime::currentTime());

      labels.push_back("Channel");
      values.push_back(chan);

      labels.push_back("Exten");
      values.push_back(exten);

      labels.push_back("Context");
      values.push_back(context);

      labels.push_back("Priority");
      values.push_back("2");

      bus_ami->sendAction("Originate",&labels,&values);

      SeizeLine(line,bank,console);
      break;

    case BusDriver::LineHold:
    case BusDriver::LineScreenedHold:
    case BusDriver::LineNext:
      labels.push_back("Channel");
      values.push_back(chan);

      labels.push_back("Exten");
      values.push_back(exten);

      labels.push_back("Context");
      values.push_back(context);

      labels.push_back("Priority");
      values.push_back("2");

      bus_ami->sendAction("Redirect",&labels,&values);      

      SeizeLine(line,bank,console);
      break;

    case BusDriver::LineOnAir:
    case BusDriver::LineLocked:
      break;

/*
    case BusDriver::LineOnAir:
      if((console==ML_MAX_CONSOLES)&&(bank==bus_line_bank[line])) {
	bus_line_state[line]=BusDriver::LineLocked;
	EmitLineState(line);
      }
      break;

    case BusDriver::LineLocked:
      if((console==ML_MAX_CONSOLES)&&(bank==bus_line_bank[line])) {
	bus_line_state[line]=BusDriver::LineOnAir;
	EmitLineState(line);
      }
      break;
*/

    case BusDriver::LineInactive:
    case BusDriver::LineElsewhere:
    case BusDriver::LineHandset:
    case BusDriver::LineSpeaker:
    case BusDriver::LineBusied:
      break;
  }
}


void AsteriskDriver::hold(int bank)
{
  hold(bank,bus_console);
}


void AsteriskDriver::hold(int bank,int console)
{
  QString chan;

  for(unsigned i=0;i<MAX_LINES;i++) {
    switch(bus_line_state[i]) {
      case BusDriver::LineOnAir:
	if((console==ML_MAX_CONSOLES)&&(bank==bus_line_bank[i])) {
	  Park(i,bus_askconfig->line2Asterisk(i));
	  bus_line_state[i]=BusDriver::LineHold;
	  bus_line_bank[i]=255;
	  bus_line_console[i]=255;
	  EmitLineState(i);
	}
	break;
	
      case BusDriver::LineHandset:
      case BusDriver::LineSpeaker:
	if(console==bus_line_console[i]) {
	  Park(i,bus_askconfig->line2Asterisk(i));
	  if(bank==0) {
	    bus_line_state[i]=BusDriver::LineHold;
	  }
	  else {
	    bus_line_state[i]=BusDriver::LineScreenedHold;
	  }
	  bus_line_bank[i]=255;
	  bus_line_console[i]=255;
	  EmitLineState(i);
	}
	break;
	
      default:
	break;
    }
  }
}


void AsteriskDriver::drop(int bank)
{
  drop(bank,bus_console);
}


void AsteriskDriver::drop(int bank,int console)
{
  QString chan;

  for(unsigned i=0;i<MAX_LINES;i++) {
    switch(bus_line_state[i]) {
      case BusDriver::LineOnAir:
	if((console==ML_MAX_CONSOLES)&&(bank==bus_line_bank[i])) {
	  Hangup(bus_askconfig->line2Asterisk(i));
	}
	break;
	
      case BusDriver::LineHandset:
      case BusDriver::LineSpeaker:
	if(console==bus_line_console[i]) {
	  Hangup(bus_askconfig->line2Asterisk(i));
	}
	break;
	
      default:
	break;
    }
  }
}


void AsteriskDriver::requestBusyAllState()
{
}


void AsteriskDriver::busyAll()
{
}


void AsteriskDriver::requestDumpDelayState()
{
}


void AsteriskDriver::dumpDelay(bool state)
{
}


void AsteriskDriver::requestRecordState()
{
}


void AsteriskDriver::toggleRecord()
{
}


void AsteriskDriver::takeNext(int bank)
{
  QDateTime dt=QDateTime(QDate::currentDate(),QTime::currentTime());
  int line=-1;

  for(int i=0;i<MAX_LINES;i++) {
    if((!bus_line_datetime[i].isNull())&&(bus_line_datetime[i]<dt)) {
      dt=bus_line_datetime[i];
      line=i;
    }
  }
  if((line>=0)&&(bus_console==ML_MAX_CONSOLES)) {
    selectLine(line,ML_MAX_CONSOLES,bank);
  }
}


void AsteriskDriver::sendDtmf(char key)
{
  QString chan;
  std::vector<QString> labels;
  std::vector<QString> values;

  for(unsigned i=0;i<MAX_LINES;i++) {
    switch(bus_line_state[i]) {
      case BusDriver::LineOnAir:
      case BusDriver::LineLocked:
      case BusDriver::LineHandset:
      case BusDriver::LineSpeaker:
	chan=bus_askconfig->line2Asterisk(i);
	if(!chan.isEmpty()) {
	  labels.push_back("Channel");
	  values.push_back(chan);

	  labels.push_back("Digit");
	  values.push_back(QString().sprintf("%c",key));

	  bus_ami->sendAction("PlayDTMF",&labels,&values);
	}
	break;

      default:
	break;
    }
  }
}


void AsteriskDriver::flash()
{
}


void AsteriskDriver::screen(bool state)
{
}


void AsteriskDriver::mute(bool state)
{
}


void AsteriskDriver::conference(bool state)
{
}


void AsteriskDriver::vip(bool state)
{
}


void AsteriskDriver::sendString(BusDriver::Element elem,unsigned line,
				const QString &str)
{
}


void AsteriskDriver::scanDevice()
{
}


void AsteriskDriver::connectedData(unsigned ami_id,bool state)
{
  if(!state) {
    emit connected(id(),state);
    return;
  }
  std::vector<QString> labels;
  std::vector<QString> values;
  labels.push_back("Filename");
  values.push_back("callcommander.conf");
  bus_init_action_id=bus_ami->sendAction("GetConfig",&labels,&values);
}


void AsteriskDriver::actionReceivedData(unsigned ami_id,unsigned action_id,
					std::vector<QString> *labels,
					std::vector<QString> *values)
{
  std::vector<QString> l;
  std::vector<QString> v;

  for(unsigned i=0;i<labels->size();i++) {
    // printf("%s: %s\n",(const char *)labels->at(i),(const char *)values->at(i));
    if(action_id==bus_init_action_id) {
      for(unsigned j=0;j<values->size();j++) {
	if(labels->at(j).contains("Category-")) {
	  values->at(j)=QString().sprintf("[%s]",(const char *)values->at(j));
	}
      }
      bus_askconfig->load(values);
      for(int j=0;j<MAX_LINES;j++) {
	if(!bus_askconfig->line2Asterisk(j).isEmpty()) {
	  bus_line_state[j]=BusDriver::LineIdle;
	}
      }
      emit connected(id(),true);
      return;
    }
  }
}


void AsteriskDriver::eventReceivedData(unsigned ami_id,
				       std::vector<QString> *labels,
				       std::vector<QString> *values)
{
  /*
  QString channel1;
  QString channel2;
  QString state;
  QString cid;
  int line;

  for(unsigned i=0;i<labels->size();i++) {
    // printf("%s: %s\n",(const char *)labels->at(i),(const char *)values->at(i));
    if(labels->at(i).toLower()=="event") {       // Line Ringing?
      if((values->at(i).toLower()=="newstate")||
	 (values->at(i).toLower()="newchannel")) {
	for(unsigned k=0;k<labels->size();k++) {
	  if(labels->at(k).toLower()=="channel") {
	    channel1=values->at(k);
	  }
	  if(labels->at(k).toLower()=="state") {	  
	    state=values->at(k).toLower();
	  }
	  if(labels->at(k).toLower()=="calleridnum") {	  
	    cid=values->at(k).toLower();
	  }
	}
	if((state=="ring")||(state=="ringing")) {
	  if((line=bus_askconfig->asterisk2Line(channel1))>=0) {
	    bus_line_state[line]=BusDriver::LineRinging;
	    bus_line_bank[line]=255;
	    bus_line_console[line]=255;
	    bus_line_datetime[line]=
	      QDateTime(QDate::currentDate(),QTime::currentTime());
	    EmitLineState(line);
	  }
	}
      }
      if(values->at(i).lower()=="hangup") {    // Line Hung Up?
	if((line=bus_askconfig->asterisk2Line(channel1))>=0) {
	  bus_line_state[line]=BusDriver::LineIdle;
	  bus_line_bank[line]=255;
	  bus_line_console[line]=255;
	  bus_line_datetime[line]=QDateTime();
	  EmitLineState(line);
	}
      }
      if(values->at(i).lower()=="newcallerid") {
	for(unsigned k=0;k<labels->size();k++) {
	  if(labels->at(k).lower()=="channel") {
	    channel1=values->at(k);
	  }
	  if(labels->at(k).lower()=="callerid") {	  
	    cid=values->at(k).lower();
	  }
	}
	if((line=bus_askconfig->asterisk2Line(channel1))>=0) {
	  if(!cid.lower().contains("unknown")) {
	    emit receivedString(id(),BusDriver::PhoneElement,line,cid);
	  }
	}
      }
    }
  }
  */
}


void AsteriskDriver::Park(unsigned line,const QString &chan)
{
  if(chan.isEmpty()) {
    return;
  }
  std::vector<QString> labels;
  std::vector<QString> values;
  labels.push_back("Channel");
  values.push_back(chan);

  labels.push_back("Exten");
  values.push_back(bus_askconfig->parkingExtension());

  labels.push_back("Context");
  values.push_back(bus_askconfig->parkingContext());

  labels.push_back("Priority");
  values.push_back("1");

  bus_ami->sendAction("Redirect",&labels,&values);
}


void AsteriskDriver::Hangup(const QString &chan)
{
  if(chan.isEmpty()) {
    return;
  }
  std::vector<QString> labels;
  std::vector<QString> values;
  labels.push_back("Channel");
  values.push_back(chan);
  bus_ami->sendAction("Hangup",&labels,&values);
}


void AsteriskDriver::SeizeLine(unsigned line,int bank,int console)
{
  if(console==ML_MAX_CONSOLES) {
    bus_line_state[line]=BusDriver::LineOnAir;
    bus_line_bank[line]=bank;
    bus_line_console[line]=255;
  }
  else {
    bus_line_state[line]=BusDriver::LineHandset;
    bus_line_bank[line]=255;
    bus_line_console[line]=console;
  }
  EmitLineState(line);
}


void AsteriskDriver::EmitLineState(unsigned line)
{
  printf("emitting currentLineState(%d,%d,%d,%d,%d)\n",
	 id(),bus_line_state[line],line,
	 bus_line_bank[line],bus_line_console[line]);

  emit currentLineState(id(),bus_line_state[line],line,
			bus_line_bank[line],bus_line_console[line]);
  UpdateNextLine();
}


void AsteriskDriver::UpdateNextLine()
{
  int next=-1;
  QDateTime base=
    QDateTime(QDate::currentDate(),QTime::currentTime()).addDays(1);;
  QDateTime dt=base;

  for(int i=0;i<MAX_LINES;i++) {
    if((bus_line_state[i]==BusDriver::LineScreenedHold)||
       (bus_line_state[i]==BusDriver::LineNext)) {
      if((!bus_line_datetime[i].isNull())&&(dt>bus_line_datetime[i])) {
	dt=bus_line_datetime[i];
	next=i;
      }
    }
  }
  if(next!=bus_next_line) {
    if(bus_next_line>=0) {
      if(bus_line_state[bus_next_line]==BusDriver::LineNext) {
	bus_line_state[bus_next_line]=BusDriver::LineScreenedHold;
	emit currentLineState(id(),bus_line_state[bus_next_line],bus_next_line,
			      bus_line_bank[bus_next_line],
			      bus_line_console[bus_next_line]);
      }
    }
    if(next>=0) {
      bus_line_state[next]=BusDriver::LineNext;
      emit currentLineState(id(),bus_line_state[next],next,
			    bus_line_bank[next],bus_line_console[next]);
    }
    bus_next_line=next;
  }
}

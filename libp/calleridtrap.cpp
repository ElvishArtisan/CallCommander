// calleridtrap.cpp
//
// Serial Driver for CallerID.com devices.
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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

#include <ctype.h>
#ifndef WIN32
#include <syslog.h>
#endif  // WIN32

#include "calleridtrap.h"

CallerIdTrap::CallerIdTrap(int id,QObject *parent,const char *name)
  : QObject(parent,name)
{
  id_id=id;
  for(int i=0;i<CALLERID_MAX_LINES;i++) {
    id_line_map[i]=i;
  }

  id_device=new MLTTYDevice(this);

  id_timer=new QTimer(this,"id_timer");
  connect(id_timer,SIGNAL(timeout()),this,SLOT(scanTtyData()));

  id_simplify_number=false;
  id_ptr=0;
}


CallerIdTrap::~CallerIdTrap()
{
  delete id_timer;
  delete id_device;
}


int CallerIdTrap::id() const
{
  return id_id;
}


void CallerIdTrap::setDeviceName(QString name)
{
  id_device->setName(name);
}


bool CallerIdTrap::simplifyNumber() const
{
  return id_simplify_number;
}


void CallerIdTrap::setSimplifyNumber(bool state)
{
  id_simplify_number=state;
}


int CallerIdTrap::lineMap(int line) const
{
  return id_line_map[line-1]+1;
}


void CallerIdTrap::setLineMap(int line,int dest_line)
{
  id_line_map[line-1]=dest_line-1;
}


bool CallerIdTrap::open()
{
  if(id_device->open(QIODevice::ReadOnly)) {
    id_timer->start(CALLERIDTRAP_SCAN_INTERVAL);
    return true;
  }
  return false;
}


void CallerIdTrap::close()
{
  if(id_device->isOpen()) {
    id_timer->stop();
    id_device->close();
  }
}


void CallerIdTrap::scanTtyData()
{
  bool cr_detected=false;

  int n=id_device->readBlock(id_buffer+id_ptr,255-id_ptr);
  //  printf("n: %d\n",n);
  for(int i=id_ptr;i<(id_ptr+n);i++) {
    if(id_buffer[i]==0x0a) {
      id_buffer[i-1]=0;
      ParseBuffer(id_buffer);
      cr_detected=true;
      id_ptr=0;
    }
  }
  if(!cr_detected) {
    id_ptr+=n;
  }
}


void CallerIdTrap::ParseBuffer(char *buf)
{
  int line;
  CallerIdDetail::CallDirection dir;
  CallerIdDetail::CallState state;
  int length;
  char ring_type;
  int ring_quantity;
  int month;
  int day;
  int hour;
  int minute;
  QDateTime datetime;
  char number[15];
  char name[16];

#ifndef WIN32
  syslog(LOG_DAEMON|LOG_NOTICE,"CallerID String: \"%s\"",buf);
#endif  // WIN32

  int l=strlen(buf);
  if(l<30) {
    fprintf(stderr,"Calleridtrap: invalid CallerID string\n");
#ifndef WIN32
    syslog(LOG_DAEMON|LOG_WARNING,"CallerID string was invalid");
#endif  // WIN32
    return;
  }

  //
  // Process Checksum
  //
  if(buf[12]!='G') {  // Bad Checksum!
    fprintf(stderr,"Calleridtrap: CallerID string has bad checksum\n");
#ifndef WIN32
    syslog(LOG_DAEMON|LOG_WARNING,"CallerID string had bad checksum");
#endif  // WIN32
    return;
  }

  //
  // Line Number
  //
  buf[2]=0;
  sscanf(buf,"%d",&line);

  //
  // Call Direction
  //
  dir=(CallerIdDetail::CallDirection)buf[3];

  //
  // Call State
  //
  state=(CallerIdDetail::CallState)buf[5];

  //
  // Call Length
  //
  buf[11]=0;
  sscanf(buf+7,"%d",&length);

  //
  // Ring Type
  //
  ring_type=buf[14];

  //
  // Ring Quantity
  //
  ring_quantity=buf[15]-'0';

  //
  // Date & Time
  //
  buf[19]=0;
  sscanf(buf+17,"%d",&month);
  buf[22]=0;
  sscanf(buf+20,"%d",&day);
  buf[25]=0;
  sscanf(buf+23,"%d",&hour);
  buf[28]=0;
  sscanf(buf+26,"%d",&minute);
  if(buf[29]=='P') {
    if(hour!=12) {
      hour+=12;
    }
  }
  else {
    if(hour==12) {
      hour=0;
    }
  }
  datetime=QDateTime(QDate(QDate::currentDate().year(),month,day),
		     QTime(hour,minute,0));

  //
  // Caller Number
  //
  if(l>32) {
    buf[46]=0;
    strcpy(number,buf+32);
    if(id_simplify_number) {
      int offset=0;
      for(unsigned i=0;i<strlen(number);i++) {
	if(isxdigit(number[i])) {
	  number[i-offset]=number[i];
	}
	else {
	  offset++;
	}
      }
      number[strlen(number)-offset]=0;
    }
  }

  //
  // Caller Name
  //
  if(l>47) {
    strcpy(name,buf+47);
  }
  CallerIdDetail detail(id_line_map[line-1]+1,dir,state,length,ring_type,
			 ring_quantity,datetime,number,name);
  emit idReceived(id_id,&detail);
}


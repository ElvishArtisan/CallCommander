// message_data.cpp
//
// A Container Class for CallCommander MessageBox Events.
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

#include "message_data.h"
#include "ml.h"
#include "mlconf.h"

MessageData::MessageData()
{
  clear();
}


QString MessageData::nick() const
{
  return msg_nick;
}


void MessageData::setNick(const QString &nick)
{
  msg_nick=nick;
}


QString MessageData::message() const
{
  return msg_message;
}


void MessageData::setMessage(const QString msg)
{
  msg_message=msg;
}


bool MessageData::update(BusDriver::Element elem,const QString &packet)
{
  if(elem!=BusDriver::MessageGlobalElement) {
    return false;
  }
  for(unsigned i=0;i<packet.length();i++) {
    if(packet.at(i)=='|') {
      msg_nick=packet.left(i);
      msg_message=packet.right(packet.length()-i-1);
      return true;
    }
  }
  return false;
}


void MessageData::sendData(BusDriver *driver) const
{
  for(unsigned i=0;i<msg_message.length();i+=ML_MAX_MESSAGE_LENGTH) {
    if(i==0) {
      driver->sendString(BusDriver::MessageGlobalElement,0,
			 QString().sprintf("%s|%s",
		     (const char *)msg_nick,
		     (const char *)msg_message.mid(i,ML_MAX_MESSAGE_LENGTH)));
    }
    else {
      driver->sendString(BusDriver::MessageGlobalElement,0,
			 QString().sprintf("%s|%s",
		     ML_CONTINUATION_NICK,
		     (const char *)msg_message.mid(i,ML_MAX_MESSAGE_LENGTH)));
    }
  }

  /*
  QStringList sl=MLKernString(msg_message,ML_MAX_MESSAGE_LENGTH);
  if(sl.size()>0) {
    driver->sendString(BusDriver::MessageGlobalElement,0,
		       QString().sprintf("%s|%s",
					 (const char *)msg_nick,
					 (const char *)sl[0]));
  }
  for(unsigned i=1;i<sl.size();i++) {
    driver->sendString(BusDriver::MessageGlobalElement,0,
		       QString().sprintf("%s|%s",
					 ML_CONTINUATION_NICK,
					 (const char *)sl[i]));
  }
  */
}


void MessageData::clear()
{
  msg_nick="";
  msg_message="";
}

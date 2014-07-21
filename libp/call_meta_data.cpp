// call_meta_data.cpp
//
// A Container Class for CallCommander Call Metadata.
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

#include "call_meta_data.h"

CallMetaData::CallMetaData()
{
  clear(true);
}


int CallMetaData::version(BusDriver::Element elem) const
{
  if(!ElementValid(elem)) {
    return -1;
  }
  return meta_version[ElementOffset(elem)];
}


QVariant CallMetaData::element(BusDriver::Element elem) const
{
  if(!ElementValid(elem)) {
    return QVariant();
  }
  return meta_element[ElementOffset(elem)];
}


int CallMetaData::priorityOffset(const QString &packet)
{
  for(int i=0;i<packet.length();i++) {
    if(packet.at(i)=='|') {
      return packet.left(i).toInt();
    }
  }
  return -1;
}


void CallMetaData::setElement(BusDriver::Element elem,int value)
{
  if(!ElementValid(elem)) {
    return;
  }
  if(value!=meta_element[ElementOffset(elem)].toInt()) {
    meta_version[ElementOffset(elem)]++;
    meta_element[ElementOffset(elem)]=QVariant(value);
  }
}


void CallMetaData::setElement(BusDriver::Element elem,unsigned value)
{
  if(!ElementValid(elem)) {
    return;
  }
  if(value!=meta_element[ElementOffset(elem)].toUInt()) {
    meta_version[ElementOffset(elem)]++;
    meta_element[ElementOffset(elem)]=QVariant(value);
  }
}


void CallMetaData::setElement(BusDriver::Element elem,bool value)
{
  if(!ElementValid(elem)) {
    return;
  }
  if(value!=meta_element[ElementOffset(elem)].toBool()) {
    meta_version[ElementOffset(elem)]++;
    meta_element[ElementOffset(elem)]=QVariant(value);
  }
}


void CallMetaData::setElement(BusDriver::Element elem,const QString &value)
{
  if(!ElementValid(elem)) {
    return;
  }
  if(value!=meta_element[ElementOffset(elem)].toString()) {
    meta_version[ElementOffset(elem)]++;
    meta_element[ElementOffset(elem)]=QVariant(value);
  }
}


void CallMetaData::setElement(BusDriver::Element elem,const char *value)
{
  if(!ElementValid(elem)) {
    return;
  }
  if(value!=meta_element[ElementOffset(elem)].toString()) {
    meta_version[ElementOffset(elem)]++;
    meta_element[ElementOffset(elem)]=QVariant(value);
  }
}


bool CallMetaData::update(BusDriver::Element elem,const QString &packet)
{
  if(!ElementValid(elem)) {
    return false;
  }


  int pipes=0;
  int version_start=0;
  int payload_start=0;
  int version=0;
  bool ok=false;

  for(int i=0;i<packet.length();i++) {
    if(packet.at(i)=='|') {
      pipes++;
      switch(pipes) {
	  case 1:
	    version_start=i+1;
	    break;

	  case 2:
	    payload_start=i+1;
	    break;

	  default:  // Corrupt packet, discard
	    return false;
      }
    }
  }
  version=packet.mid(version_start,payload_start-version_start-1).toInt(&ok);
  if(!ok) {  // Corrupt packet, discard
    return false;
  }
/*
 * FIXME: The current versioning scheme is flawed and hence disabled,
 *        we need to find something better!
 *
  if(version<=meta_version[ElementOffset(elem)]) {  // Dated packet, discard
    return false;
  }
*/

  meta_element[ElementOffset(elem)]=
    QVariant(packet.right(packet.length()-payload_start));
  meta_version[ElementOffset(elem)]=version;

  return true;
}


void CallMetaData::sendData(BusDriver *driver,unsigned line,
			    int priority_offset) const
{
  QString packet;

  for(unsigned i=0;i<(BusDriver::FirstNameAwardElement-BusDriver::NameElement);
      i++) {
    packet=QString().sprintf("%d|%d|%s",
			     priority_offset,
			     meta_version[i],
			     (const char *)meta_element[i].toString());
    driver->
      sendString((BusDriver::Element)(i+BusDriver::NameElement),line,packet);
  }
}


void CallMetaData::clear(bool reset_version)
{
  for(unsigned i=0;i<(BusDriver::FirstNameAwardElement-BusDriver::NameElement);
      i++) {
    if(reset_version) {
      meta_version[i]=0;
    }
    else {
      if(!meta_element[i].isNull()) {
	meta_version[i]++;
      }
    }
    meta_element[i].clear();
  }
}


bool CallMetaData::ElementValid(BusDriver::Element elem) const
{
  if((elem<BusDriver::NameElement)||(elem>=BusDriver::FirstNameAwardElement)) {
    return false;
  }
  return true;
}


unsigned CallMetaData::ElementOffset(BusDriver::Element elem) const
{
  return elem-BusDriver::NameElement;
}

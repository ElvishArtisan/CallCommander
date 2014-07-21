// mlasteriskconfig.cpp
//
// A container class for CallCommander Asterisk Configuration
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

#include "mlasteriskconfig.h"
#include "mlprofile.h"

MLAsteriskConfig::MLAsteriskConfig()
{
  clear();
}


int MLAsteriskConfig::asterisk2Line(const QString &chan) const
{
  for(unsigned i=0;i<MAX_LINES;i++) {
    if(chan.lower()==ask_lines[i]) {
      return i;
    }
  }
  return -1;
}


QString MLAsteriskConfig::line2Asterisk(int line) const
{
  return ask_lines[line];
}


int MLAsteriskConfig::asterisk2Bank(const QString &chan) const
{
  for(unsigned i=0;i<MAX_BANKS;i++) {
    if(chan.lower()==ask_banks[i]) {
      return i;
    }
  }
  return -1;
}


QString MLAsteriskConfig::bank2Asterisk(int bank) const
{
  return ask_banks[bank];
}


int MLAsteriskConfig::asterisk2Console(const QString &chan) const
{
  for(unsigned i=0;i<ML_MAX_CONSOLES;i++) {
    if(chan.lower()==ask_consoles[i]) {
      return i;
    }
  }
  return -1;
}


QString MLAsteriskConfig::console2Asterisk(int console) const
{
  return ask_consoles[console];
}


QString MLAsteriskConfig::extension(int bank,int console) const
{
  if(console==ML_MAX_CONSOLES) {
    return ask_bank_extensions[bank];
  }
  return ask_console_extensions[console];
}


QString MLAsteriskConfig::context(int bank,int console) const
{
  if(console==ML_MAX_CONSOLES) {
    return ask_bank_contexts[bank];
  }
  return ask_console_contexts[console];
}


QString MLAsteriskConfig:: parkingExtension() const
{
  return ask_parking_extension;
}


QString MLAsteriskConfig::parkingContext() const
{
  return ask_parking_context;
}


void MLAsteriskConfig::load(std::vector<QString> *values)
{
  QString label;

  MLProfile *p=new MLProfile();
  p->setSource(values);

  for(unsigned i=0;i<MAX_LINES;i++) {
    ask_lines[i]=
      p->stringValue("Asterisk",QString().sprintf("Line%uChannel",i)).lower();
  }
  for(unsigned i=0;i<MAX_BANKS;i++) {
    ask_banks[i]=
      p->stringValue("Asterisk",QString().sprintf("Hybrid%uChannel",i)).lower();
    ask_bank_extensions[i]=
      p->stringValue("Asterisk",QString().sprintf("Hybrid%uExtension",i));
    ask_bank_contexts[i]=
      p->stringValue("Asterisk",QString().sprintf("Hybrid%uContext",i));
  }
  for(unsigned i=0;i<ML_MAX_CONSOLES;i++) {
    ask_consoles[i]=
      p->stringValue("Asterisk",QString().sprintf("Screener%uChannel",i)).
      lower();
    ask_console_extensions[i]=
      p->stringValue("Asterisk",QString().sprintf("Screener%uExtension",i));
    ask_console_contexts[i]=
      p->stringValue("Asterisk",QString().sprintf("Screener%uContext",i));
  }
  ask_parking_extension=p->stringValue("Asterisk","ParkingExtension");
  ask_parking_context=p->stringValue("Asterisk","ParkingContext");

  delete p;
}


void MLAsteriskConfig::clear()
{
  for(unsigned i=0;i<MAX_LINES;i++) {
    ask_lines[i]="";
  }
  for(unsigned i=0;i<MAX_BANKS;i++) {
    ask_banks[i]="";
    ask_bank_contexts[i]="";
    ask_bank_extensions[i]="";
  }
  for(unsigned i=0;i<ML_MAX_CONSOLES;i++) {
    ask_consoles[i]="";
    ask_console_contexts[i]="";
    ask_console_extensions[i]="";
  }
  ask_parking_extension="";
  ask_parking_context="";
}

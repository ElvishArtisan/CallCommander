// askdata.cpp
//
// Startup Dialog Data Container Class
//
//   (C) Copyright 2002 Fred Gleason <fredg@paravelsystems.com>
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

#include <stdio.h>
#include <stdlib.h>

#include "askdata.h"

AskData::AskData()
{
  clear();
}


void AskData::clear()
{
  ask_hostname="";
  ask_port=0;
  ask_console=0;
  ask_mode=MldConfig::Talent;
}


QString AskData::hostname() const
{
  return ask_hostname;
}


void AskData::setHostname(QString string)
{
  ask_hostname=string;
}


unsigned short AskData::port() const
{
  return ask_port;
}


void AskData::setPort(unsigned short port)
{
  ask_port=port;
}


int AskData::console() const
{
  return ask_console;
}


void AskData::setConsole(int console)
{
  ask_console=console;
}


QString AskData::password() const
{
  return ask_password;
}


void AskData::setPassword(QString password)
{
  ask_password=password;
}


int AskData::group() const
{
  return ask_group;
}


void AskData::setGroup(int group)
{
  ask_group=group;
}




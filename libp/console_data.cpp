// console_data.cpp
//
// A Container Class for CallCommander Console Entries.
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

#include "console_data.h"

ConsoleData::ConsoleData(const QString &console_name,int console,
			 ConsoleData::Mode mode,bool free)
{
  data_console_name=console_name;
  data_console_console=console;
  data_console_mode=mode;
  data_console_free=free;
}


QString ConsoleData::name() const
{
  return data_console_name;
}


int ConsoleData::console() const
{
  return data_console_console;
}


ConsoleData::Mode ConsoleData::mode() const
{
  return data_console_mode;
}


bool ConsoleData::isFree() const
{
  return data_console_free;
}


QString ConsoleData::modeText(ConsoleData::Mode mode)
{
  QString ret;
  switch(mode) {
      case ConsoleData::Producer:
	ret="Producer";
	break;

      case ConsoleData::Talent:
	ret="Talent";
	break;

      case ConsoleData::Unavailable:
	ret="Unknown";
	break;
  }
  return ret;
}

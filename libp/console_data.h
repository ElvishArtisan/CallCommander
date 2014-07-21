// console_data.h
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

#ifndef CONSOLE_DATA_H
#define CONSOLE_DATA_H

#include <QtCore/QString>
#include <QtCore/QDateTime>

class ConsoleData
{
  public:
   enum Mode {Producer=0,Talent=1,Unavailable=2};
   ConsoleData(const QString &console_name,int console,
	       ConsoleData::Mode mode,bool free);
   QString name() const;
   int console() const;
   ConsoleData::Mode mode() const;
   bool isFree() const;
   static QString modeText(ConsoleData::Mode mode);

  private:
   QString data_console_name;
   int data_console_console;
   ConsoleData::Mode data_console_mode;
   bool data_console_free;
};

#endif  // CONSOLE_DATA_H

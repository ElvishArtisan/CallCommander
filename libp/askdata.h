// askdata.h
//
// Start Dialog Data Container Class
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

#ifndef ASKDATA_H
#define ASKDATA_H

#include <QtCore/QString>

#include <mlconfig.h>

class AskData
{
  public:
   AskData();
   void clear();
   QString hostname() const;
   void setHostname(QString string);
   unsigned short port() const;
   void setPort(unsigned short port);
   int console() const;
   void setConsole(int console);
   QString password() const;
   void setPassword(QString password);
   int group() const;
   void setGroup(int group);

  private:
   QString ask_hostname;
   unsigned short ask_port;
   int ask_console;
   MldConfig::DirectorMode ask_mode;
   QString ask_password;
   int ask_group;
};


#endif

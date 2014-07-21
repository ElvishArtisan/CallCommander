// show_data.h
//
// A Container Class for CallCommander Show Entries.
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

#ifndef SHOW_DATA_H
#define SHOW_DATA_H

#include <QtCore/QString>
#include <QtCore/QDateTime>

class ShowData
{
  public:
   ShowData(const QString &show_name,const QString &host_name,
	    bool is_active,bool passwd_reqd);
   QString name() const;
   QString hostName() const;
   bool isActive() const;
   bool passwordRequired() const;

  private:
   QString data_show_name;
   QString data_host_name;
   bool data_is_active;
   bool data_passwd_reqd;
};

#endif  // SHOW_DATA_H

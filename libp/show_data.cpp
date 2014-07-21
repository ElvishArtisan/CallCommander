// show_data.cpp
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

#include "show_data.h"

ShowData::ShowData(const QString &show_name,const QString &host_name,
		   bool is_active,bool passwd_reqd)
{
  data_show_name=show_name;
  data_host_name=host_name;
  data_is_active=is_active;
  data_passwd_reqd=passwd_reqd;
}


QString ShowData::name() const
{
  return data_show_name;
}


QString ShowData::hostName() const
{
  return data_host_name;
}


bool ShowData::isActive() const
{
  return data_is_active;
}


bool ShowData::passwordRequired() const
{
  return data_passwd_reqd;
}

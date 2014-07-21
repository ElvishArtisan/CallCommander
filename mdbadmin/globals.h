// globals.h
//
// Global Definitions for MDBAdmin
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


#ifndef GLOBALS_H
#define GLOBALS_H

#include <Qt3Support/q3socketdevice.h>

#include <mldconfig.h>

//
// Global Classes
//
extern MldConfig *admin_config;
extern QString admin_loginname;
extern bool exiting;
extern Q3SocketDevice *admin_reload_socket;
extern QString admin_admin_username;
extern QString admin_admin_password;
extern bool found_check_db;

#endif  // GLOBALS_H

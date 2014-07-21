// globals.h
//
// Global Definitions for MCallMan
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

#include <QtCore/QDateTime>

#include <mldconfig.h>

//
// Global Classes
//
extern MldConfig *man_config;
extern QString man_loginname;
extern bool exiting;
extern QDateTime start_datetime;
extern QDateTime end_datetime;
extern int base_year;

#endif  // GLOBALS_H

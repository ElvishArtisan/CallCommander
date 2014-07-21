# mdbadmin.pro
#
# The QMake project file for mdbadmin(1).
#
# (C) Copyright 2003-2006 Fred Gleason <fredg@paravelsystems.com>
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as
#   published by the Free Software Foundation; either version 2 of
#   the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

TEMPLATE = app

win32 {
  DEFINES += WIN32
  DEFINES += VERSION=\\\"$$[VERSION]\\\"
  DEFINES += PACKAGE=\\\"callcommander\\\" 
  DEFINES += PACKAGE_VERSION=\\\"$$[VERSION]\\\" 
  DEFINES += PACKAGE_NAME=\\\"callcommander\\\"
  DEFINES += PACKAGE_BUGREPORT=\\\"fredg@paravelsystems.com\\\"
}

SOURCES += add_callerid_source.cpp
SOURCES += add_directory.cpp
SOURCES += add_map_line.cpp
SOURCES += add_show.cpp
SOURCES += add_system.cpp
SOURCES += add_user.cpp
SOURCES += createdb.cpp
SOURCES += edit_callerid_source.cpp
SOURCES += edit_directory.cpp
SOURCES += edit_filter.cpp
SOURCES += edit_map.cpp
SOURCES += edit_module.cpp
SOURCES += edit_show.cpp
SOURCES += edit_show_slot.cpp
SOURCES += edit_system.cpp
SOURCES += edit_user.cpp
SOURCES += list_callerid_sources.cpp
SOURCES += list_directory.cpp
SOURCES += list_modules.cpp
SOURCES += list_show_configs.cpp
SOURCES += list_systems.cpp
SOURCES += list_users.cpp
SOURCES += mdbadmin.cpp
SOURCES += mysql_login.cpp
SOURCES += name_sub.cpp
SOURCES += opendb.cpp
SOURCES += purge_directory.cpp
SOURCES += select_callerid_source.cpp
SOURCES += system_info.cpp

HEADERS += add_callerid_source.h
HEADERS += add_directory.h
HEADERS += add_map_line.h
HEADERS += add_show.h
HEADERS += add_system.h
HEADERS += add_user.h
HEADERS += createdb.h
HEADERS += edit_callerid_source.h
HEADERS += edit_directory.h
HEADERS += edit_filter.h
HEADERS += edit_map.h
HEADERS += edit_module.h
HEADERS += edit_show.h
HEADERS += edit_show_slot.h
HEADERS += edit_system.h
HEADERS += edit_user.h
HEADERS += globals.h
HEADERS += list_callerid_sources.h
HEADERS += list_directory.h
HEADERS += list_modules.h
HEADERS += list_show_configs.h
HEADERS += list_systems.h
HEADERS += list_users.h
HEADERS += mdbadmin.h
HEADERS += mysql_login.h
HEADERS += name_sub.h
HEADERS += opendb.h
HEADERS += purge_directory.h
HEADERS += select_callerid_source.h
HEADERS += system_info.h


RC_FILE += ../icons/callcommander.rc

INCLUDEPATH += ../libp
INCLUDEPATH += ../mlhpi

LIBS += -LC:/OpenSSL/lib/VC -llibeay32MT
LIBS += -lwsock32 -ladvapi32
LIBS += -L../libp/release -lcallcmdr

CONFIG += qt
CONFIG += warn_on
CONFIG += release
CONFIG += windows

QT += qt3support
QT += network
QT += sql

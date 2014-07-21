# mcallman.pro
#
# The QMake project file for mcallman(1).
#
# (C) Copyright 2003-2005 Fred Gleason <fredg@paravelsystems.com>
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

SOURCES += add_action.cpp
SOURCES += add_report.cpp
SOURCES += area_code_report.cpp
SOURCES += detail_listview.cpp
SOURCES += edit_action.cpp
SOURCES += edit_report.cpp
SOURCES += filter_dialog.cpp
SOURCES += list_actions.cpp
SOURCES += list_details.cpp
SOURCES += list_reports.cpp
SOURCES += list_winners.cpp
SOURCES += mcallman.cpp
SOURCES += peg_counter.cpp
SOURCES += print_report.cpp
SOURCES += select_operation.cpp
SOURCES += show_detail.cpp
SOURCES += standard_age_report.cpp
SOURCES += winner_listview.cpp
SOURCES += winner_listviewitem.cpp

HEADERS += add_action.h
HEADERS += add_report.h
HEADERS += detail_listview.h
HEADERS += edit_action.h
HEADERS += edit_report.h
HEADERS += filter_dialog.h
HEADERS += globals.h
HEADERS += list_actions.h
HEADERS += list_details.h
HEADERS += list_reports.h
HEADERS += list_winners.h
HEADERS += mcallman.h
HEADERS += peg_counter.h
HEADERS += print_report.h
HEADERS += select_operation.h
HEADERS += show_detail.h
HEADERS += winner_listview.h
HEADERS += winner_listviewitem.h

RC_FILE += ../icons/callcommander.rc

INCLUDEPATH += ../libp
INCLUDEPATH += ../mlhpi

LIBS += -LC:/OpenSSL/lib/VC -llibeay32MT
LIBS += -lwsock32 -ladvapi32
LIBS += -L../libp/release -lcallcmdr

CONFIG += qt
CONFIG += release
CONFIG += warn_on
CONFIG += windows

QT += network
QT += qt3support
QT += sql

# ml.pro
#
# The QMake project file for ml(1).
#
# (C) Copyright 2003 Fred Gleason <fredg@paravelsystems.com>
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

SOURCES += callcomment.cpp
SOURCES += callers.cpp
SOURCES += callinfo.cpp
SOURCES += change_nickname.cpp
SOURCES += client_listview.cpp
SOURCES += client_listviewitem.cpp
SOURCES += comment.cpp
SOURCES += config.cpp
SOURCES += drawcaps.cpp
SOURCES += keybox.cpp
SOURCES += line_button.cpp
SOURCES += line_warning.cpp
SOURCES += list_clients.cpp
SOURCES += message.cpp
SOURCES += mithlond.cpp
SOURCES += msg_textedit.cpp
SOURCES += redial_dialog.cpp
SOURCES += sendmsg_textedit.cpp

HEADERS += callcomment.h
HEADERS += callinfo.h
HEADERS += callers.h
HEADERS += change_nickname.h
HEADERS += client_listview.h
HEADERS += client_listviewitem.h
HEADERS += comment.h
HEADERS += keybox.h
HEADERS += line_button.h
HEADERS += line_warning.h
HEADERS += list_clients.h
HEADERS += message.h
HEADERS += mithlond.h
HEADERS += msg_textedit.h
HEADERS += redial_dialog.h
HEADERS += sendmsg_textedit.h

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

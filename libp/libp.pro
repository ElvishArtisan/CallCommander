# libp.pro
#
# The libp/ QMake project file for CallCommander.
#
# (C) Copyright 2003-2007 Fred Gleason <fredg@paravelsystems.com>
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

TEMPLATE = lib
TARGET = callcmdr
win32 {
  DEFINES += WIN32
  DEFINES += VERSION=\\\"$$[VERSION]\\\"
  DEFINES += PACKAGE=\\\"callcommander\\\" 
  DEFINES += PACKAGE_VERSION=\\\"$$[VERSION]\\\" 
  DEFINES += PACKAGE_NAME=\\\"callcommander\\\"
  DEFINES += PACKAGE_BUGREPORT=\\\"fredg@paravelsystems.com\\\"
}

SOURCES += askbox.cpp
SOURCES += askdata.cpp
SOURCES += asterisk_driver.cpp
SOURCES += bus_driver.cpp
SOURCES += call_meta_data.cpp
SOURCES += calleriddetail.cpp
SOURCES += calleridtrap.cpp
SOURCES += change_password.cpp
SOURCES += client_data.cpp
SOURCES += cmdswitch.cpp
SOURCES += conn_view_item.cpp
SOURCES += connection_data.cpp
SOURCES += console_data.cpp
SOURCES += dbdialog.cpp
SOURCES += edit_connection.cpp
SOURCES += edit_connection_data.cpp
SOURCES += edit_ttysettings.cpp
SOURCES += edit_winner.cpp
SOURCES += escapestring.cpp
SOURCES += list_connections.cpp
SOURCES += list_consoles.cpp
SOURCES += list_devices.cpp
SOURCES += list_shows.cpp
SOURCES += login.cpp
SOURCES += message_data.cpp
SOURCES += ml_listviewitem.cpp
SOURCES += ml_reload_packet.cpp
SOURCES += ml_timeslot.cpp
SOURCES += ml_timeslotlist.cpp
SOURCES += mlami.cpp
SOURCES += mlasteriskconfig.cpp
SOURCES += mlconf.cpp
SOURCES += mlconfig.cpp
SOURCES += mldatedialog.cpp
SOURCES += mldatepicker.cpp
SOURCES += mldconfig.cpp
SOURCES += mldshow.cpp
SOURCES += mllabel.cpp
SOURCES += mllicense.cpp
SOURCES += mllistselector.cpp
SOURCES += mlplaymeter.cpp
SOURCES += mlprofile.cpp
SOURCES += mlprofilesection.cpp
SOURCES += mlprofileline.cpp
SOURCES += mlpushbutton.cpp
SOURCES += mlsegclock.cpp
SOURCES += mlseglist.cpp
SOURCES += mlsegment.cpp
SOURCES += mlsegmeter.cpp
SOURCES += mltransportbutton.cpp
SOURCES += number_list.cpp
SOURCES += save_connection.cpp
SOURCES += shows.cpp
SOURCES += show_data.cpp
SOURCES += telos100_driver.cpp
SOURCES += telos100_states.cpp
SOURCES += telos2101_callbacks.cpp
SOURCES += telos2101_driver.cpp
SOURCES += tlsaplib.cpp
SOURCES += ttysettings.cpp
SOURCES += ts612_driver.cpp
SOURCES += virtual_driver.cpp
x11 {
  SOURCES += html_gpl2.cpp
  SOURCES += mlttydevice.cpp
  SOURCES += mlwavefile.cpp
}
win32 {
  SOURCES += html_gpl2_win32.cpp
  SOURCES += mlttydevice_win32.cpp
}

HEADERS += askbox.h
HEADERS += askdata.h
HEADERS += asterisk_driver.h
HEADERS += bus_driver.h
HEADERS += calleriddetail.h
HEADERS += calleridtrap.h
HEADERS += call_meta_data.h
HEADERS += change_password.h
HEADERS += client_data.h
HEADERS += cmdswitch.h
HEADERS += conn_view_item.h
HEADERS += connection_data.h
HEADERS += console_data.h
HEADERS += dbdialog.h
HEADERS += edit_connection.h
HEADERS += edit_connection_data.h
HEADERS += edit_ttysettings.h
HEADERS += edit_winner.h
HEADERS += escapestring.h
HEADERS += list_connections.h
HEADERS += list_consoles.h
HEADERS += list_devices.h
HEADERS += list_shows.h
HEADERS += login.h
HEADERS += message_data.h
HEADERS += ml.h
HEADERS += ml_listviewitem.h
HEADERS += ml_reload_packet.h
HEADERS += ml_timeslot.h
HEADERS += ml_timeslotlist.h
HEADERS += mlami.h
HEADERS += mlasteriskconfig.h
HEADERS += mlconf.h
HEADERS += mlconfig.h
HEADERS += mldatedialog.h
HEADERS += mldatepicker.h
HEADERS += mldconfig.h
HEADERS += mldshow.h
HEADERS += mllabel.h
HEADERS += mllicense.h
HEADERS += mllistselector.h
HEADERS += mlplaymeter.h
HEADERS += mlprofile.h
HEADERS += mlprofilesection.h
HEADERS += mlprofileline.h
HEADERS += mlpushbutton.h
HEADERS += mlsegclock.h
HEADERS += mlseglist.h
HEADERS += mlsegment.h
HEADERS += mlsegmeter.h
HEADERS += mlttydevice.h
HEADERS += mltransportbutton.h
HEADERS += number_list.h
HEADERS += save_connection.h
HEADERS += shows.h
HEADERS += show_data.h
HEADERS += telos100_driver.h
HEADERS += telos100_states.h
HEADERS += telos2101_driver.h
HEADERS += tlsaplib.h
HEADERS += ts612_driver.h
HEADERS += ttysettings.h
HEADERS += virtual_driver.h

INCLUDEPATH += ../mlhpi
INCLUDEPATH += C:/OpenSSL/include
INCLUDEPATH += ../libp

LIBS += -LC:/OpenSSL/lib/VC -llibeay32MT -lwsock32

CONFIG += qt
CONFIG += staticlib
CONFIG += release
CONFIG += warn_on

QT += network
QT += qt3support
QT += sql

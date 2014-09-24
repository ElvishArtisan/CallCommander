## automake.am
##
## mld/Makefile.am for CallCommander
##
##   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as
##   published by the Free Software Foundation; either version 2 of
##   the License, or (at your option) any later version.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -DQT3_SUPPORT -I../mlhpi -I$(top_srcdir)/libp @LOCAL_CPPFLAGS@
LIBS = -L$(top_srcdir)/libp
MOC = @MOC@

# QT's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

sbin_PROGRAMS = mld
dist_mld_SOURCES = globals.h\
                   mld.cpp mld.h\
                   mld_connection.cpp mld_connection.h\
                   mld_socket.cpp mld_socket.h\
                   mld_system.cpp mld_system.h

nodist_mld_SOURCES = moc_mld.cpp\
                     moc_mld_socket.cpp

mld_LDADD = @QTLIBS@ -lml -lssl
mld_LDFLAGS = @QTFLAGS@

CLEANFILES = *~\
             moc_*

DISTCLEANFILES = moc_*

MAINTAINERCLEANFILES = *~\
                       Makefile.in\
                       moc_*

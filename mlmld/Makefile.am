## automake.am
##
## mlmld/Makefile.am for CallCommander
##
## Use automake to process this into a Makefile.in
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

sbin_PROGRAMS = mlmld
dist_mlmld_SOURCES = mlmld.cpp mlmld.h

nodist_mlmld_SOURCES = moc_mlmld.cpp

mlmld_LDADD = @QTLIBS@ -lml -lssl
mlmld_LDFLAGS = @QTFLAGS@

CLEANFILES = *~\
             moc_*

DISTCLEANFILES = moc_*

MAINTAINERCLEANFILES = *~\
                       Makefile.in\
                       moc_*

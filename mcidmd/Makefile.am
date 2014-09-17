## automake.am
##
## mcidmd/Makefile.am for CallCommander
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

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -DQT3_SUPPORT -I$(top_srcdir)/libp
LIBS = -L$(top_srcdir)/libp
MOC = @MOC@

# QT's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

sbin_PROGRAMS = mcidmd
dist_mcidmd_SOURCES = award_data.cpp award_data.h\
                      cid_monitor.cpp cid_monitor.h\
                      mcidmd.cpp mcidmd.h\
                      process_id.cpp

nodist_mcidmd_SOURCES = moc_mcidmd.cpp

mcidmd_LDADD = @QTLIBS@ -lml -lssl
mcidmd_LDFLAGS = @QTFLAGS@

CLEANFILES = *~\
             moc_*

DISTCLEANFILES = moc_*

MAINTAINERCLEANFILES = *~\
                       Makefile.in\
                       moc_*

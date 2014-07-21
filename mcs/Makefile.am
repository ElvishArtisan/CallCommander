## automake.am
##
## mcs/ Makefile.am for mithlond
##
## (C) Copyright 2004-2006 Fred Gleason <fredg@paravelsystems.com>
##
## Use automake to process this into a Makefile.in
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

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -DQT3_SUPPORT
INCLUDES = -I$(top_srcdir)/libp -I$(top_srcdir)/mlhpi
LIBS = -L$(top_srcdir)/libp -L$(top_srcdir)/mlhpi
MOC = @MOC@

# QT's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

bin_PROGRAMS = mskimmer
dist_mskimmer_SOURCES = mskimmer.cpp mskimmer.h globals.h\
                        mskimmer_widget.cpp mskimmer_widget.h

nodist_mskimmer_SOURCES = moc_mskimmer.cpp\
                          moc_mskimmer_widget.cpp

mskimmer_LDADD = @QTLIBS@ -lml -lssl -lmlhpi -lhpi -lid3 @LIBVORBIS@
mskimmer_LDFLAGS = @QTFLAGS@

CLEANFILES = *~\
             moc_*
DISTCLEANFILES = moc_*
MAINTAINERCLEANFILES = *~\
                       Makefile.in\
                       moc_*

## automake.am
##
## helpers/ Automake.am for CallCommander
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
##
##    $Id: Makefile.am,v 1.3 2007/09/07 01:01:14 fredg Exp $
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU Library General Public License 
##   version 2 as published by the Free Software Foundation.
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

AM_CPPFLAGS = -Wall

noinst_PROGRAMS = cwrap

dist_cwrap_SOURCES = cwrap.cpp cwrap.h

EXTRA_DIST = win32_frag1.txt win32_frag2.txt

CLEANFILES = *~ moc_* *.tar.gz *.bat
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 moc_* *.tar.gz

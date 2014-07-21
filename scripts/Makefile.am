## automake.am
##
## scripts/ Makefile.am for Call Commander
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2002-2008 Fred Gleason <fredg@paravelsystems.com>
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

CFLAGS = -Wall
INCLUDES = -I$(QTDIR)/include
LIBS = -L$(QTDIR)/lib
MOC = $(QTDIR)/bin/moc

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@


dist_sbin_SCRIPTS = twinkle_answer.sh\
                    twinkle_call.sh
nodist_sbin_SCRIPTS = twinkle_hybrid.sh

configdir = $(datadir)/callcommander
dist_config_DATA = twinkle.sys
nodist_config_DATA = twinkle.cfg

EXTRA_DIST = twinkle.cfg.in\
             twinkle_hybrid.sh.in

CLEANFILES = *~

DISTCLEANFILES = twinkle.cfg\
                 twinkle_hybrid.sh

MAINTAINERCLEANFILES = *~\
                       Makefile.in

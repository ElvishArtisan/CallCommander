## automake.am
##
## Automake.am for mlm/
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
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

AM_CFLAGS = -fPIC -Wall

MLM_MODULES = mlm_airtools.mlm\
              mlm_cunctator.mlm\
              mlm_dump_calls.mlm\
              mlm_remora.mlm\
              mlm_test.mlm\
              mlm_udpdelay.mlm

all:	$(MLM_MODULES)

%.mlm:	%.c
	$(CC) $(AM_CFLAGS) -I$(top_srcdir) -shared $< -o $@

install:	all
	mkdir -p $(DESTDIR)$(prefix)/@CALL_LIB_PATH@/mlm
	cp $(MLM_MODULES) $(DESTDIR)$(prefix)/@CALL_LIB_PATH@/mlm/
	mkdir -p $(includedir)/mlm
	cp mlm.h $(includedir)/mlm/

headerdir = $(includedir)/mlm
header_HEADERS = mlm.h

EXTRA_DIST = Makefile-example\
             mlm_airtools.c\
             mlm_cunctator.c\
             mlm_dump_calls.c\
             mlm_remora.c\
             mlm_test.c\
             mlm_udpdelay.c

CLEANFILES = *~\
             *.mlm\
             *.obj\
             *.qm\
             html_gpl2.cpp\
             moc_*

MAINTAINERCLEANFILES = *~\
                       Makefile.in\
                       moc_*

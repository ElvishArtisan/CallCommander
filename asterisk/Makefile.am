## automake.am
##
## Automake.am for callcommander/asterisk
##
## by Fred Gleason <fredg@paravelsystems.com>
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
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

AM_CFLAGS = -fPIC -Wall -I $(top_srcdir)/libp

%.so:	%.c
	$(CC) $(AM_CFLAGS) -I$(top_srcdir) -shared $< -o $@

EXTRA_DIST = pbx_callcommander.c callcommander.conf-sample\
             extensions.conf-sample zapata.conf-sample\
             features.conf-sample manager.conf-sample\
             sip.conf-sample

CLEANFILES = *~ moc_* html_gpl2.cpp *.tar.gz *.qm\
             *.obj *.lib *.exe *.idb *.pdb *.ilk
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 moc_* *.tar.gz

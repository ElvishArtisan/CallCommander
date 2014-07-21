## automake.am
##
## Automake.am for icons/
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2002-2005 Fred Gleason <fredg@paravelsystems.com>
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

install-exec-am:	
	mkdir -p @prefix@/share/icons/hicolor/16x16/apps
	cp callcommander-16x16.png @prefix@/share/icons/hicolor/16x16/apps/callcommander.png
	mkdir -p @prefix@/share/icons/hicolor/22x22/apps
	cp callcommander-22x22.png @prefix@/share/icons/hicolor/22x22/apps/callcommander.png
	mkdir -p @prefix@/share/icons/hicolor/32x32/apps
	cp callcommander-32x32.png @prefix@/share/icons/hicolor/32x32/apps/callcommander.png
	mkdir -p @prefix@/share/icons/hicolor/48x48/apps
	cp callcommander-48x48.png @prefix@/share/icons/hicolor/48x48/apps/callcommander.png

uninstall:	
	rm -f @prefix@/share/icons/hicolor/16x16/apps/callcommander.png
	rm -f @prefix@/share/icons/hicolor/22x22/apps/callcommander.png
	rm -f @prefix@/share/icons/hicolor/32x32/apps/callcommander.png
	rm -f @prefix@/share/icons/hicolor/48x48/apps/callcommander.png

EXTRA_DIST = callcommander-16x16.png\
             callcommander-16x16.xpm\
             callcommander-22x22.png\
             callcommander-22x22.xpm\
             callcommander-32x32.png\
             callcommander-32x32.xpm\
             callcommander-48x48.png\
             callcommander-48x48.xpm\
             callcommander.rc\
             callcommander.ico\
             CallCommander.icns\
             checkmark.xpm\
             client.xpm\
             client_segment.xpm\
             ex.xpm\
             logic.xpm\
             monitor.xpm\
             recorder.xpm

CLEANFILES = *~
DISTCLEANFILES = moc_*
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 moc_* *.tar.gz

## automake.am
##
## xdg/automake.am for CallCommander
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
	mkdir -p @prefix@/share/applications
	cp callcommander-mcallman.desktop @prefix@/share/applications/
	cp callcommander-mdbadmin.desktop @prefix@/share/applications/
	cp callcommander-ml.desktop @prefix@/share/applications/
	cp callcommander-mskimmer.desktop @prefix@/share/applications/
	mkdir -p @prefix@/share/desktop-directories
	cp *.directory @prefix@/share/desktop-directories/
	mkdir -p /etc/xdg/menus/applications-merged
	cp *.menu /etc/xdg/menus/applications-merged/

uninstall:	
	rm -f @prefix@/share/applications/callcommander-*.desktop
	rm -f @prefix@/share/desktop-directories/callcommander-*.directory
	rm -f /etc/xdg/menus/applications-merged/callcommander-*.menu

EXTRA_DIST = callcommander-callcommander.directory\
             callcommander-callcommander.menu\
             callcommander-mcallman.desktop\
             callcommander-mdbadmin.desktop\
             callcommander-ml.desktop\
             callcommander-mskimmer.desktop

CLEANFILES = *~
MAINTAINERCLEANFILES = *~\
                       Makefile.in

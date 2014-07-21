## automake.am
##
## Top-level Makefile.am for Call Commander
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
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

if BUILD_SOUND
SUBDIRS = helpers samples docs icons xdg libp mlhpi ml mld mcidmd scripts\
          mdbadmin mcallman mlm mlmh mlmld mcs asterisk
else
SUBDIRS = helpers samples docs icons xdg libp ml mld mcidmd scripts mdbadmin\
          mcallman asterisk
endif

install-exec-local:	
	cp rc.mld /etc/init.d/mld
	cp rc.mcidmd /etc/init.d/mcidmd
	chmod 755 /etc/init.d/mcidmd
	cp rc.mlmld /etc/init.d/mlmld
	chmod 755 /etc/init.d/mlmld

uninstall-local:	
	rm /etc/init.d/mld
	rm /etc/init.d/mcidmd
	rm /etc/init.d/mlmld

dmg:	
	./make_bundle.sh

rpm:	dist
	cp callcommander-$(VERSION).tar.gz $(RPM_ROOT)/SOURCES/
	$(RPMBUILD) -ba callcommander.spec
	mv $(RPM_ROOT)/RPMS/$(ARCH)/callcommander-$(VERSION)-@RPM_RELEASE@.$(ARCH).rpm $(top_srcdir)/
	mv $(RPM_ROOT)/RPMS/$(ARCH)/callcommander-audio-$(VERSION)-@RPM_RELEASE@.$(ARCH).rpm $(top_srcdir)/
	mv $(RPM_ROOT)/RPMS/$(ARCH)/callcommander-devel-$(VERSION)-@RPM_RELEASE@.$(ARCH).rpm $(top_srcdir)/
	mv $(RPM_ROOT)/SRPMS/callcommander-$(VERSION)-@RPM_RELEASE@.src.rpm $(top_srcdir)/
	rm $(RPM_ROOT)/SOURCES/callcommander-$(VERSION).tar.gz
	rm -rf $(RPM_ROOT)/BUILD/callcommander-$(VERSION)

EXTRA_DIST = callcommander.spec.in callcommander.pro autogen.sh\
             slack_doinst.sh make_slack.in slack-desc.in rc.mcidmd.in\
             rc.mld.in callcommander.ism build_win32.bat VERSION\
             get_target.sh get_distro.sh ltmain.sh make_bundle.sh.in\
             callcommander-screener.ism

slack:	all
	./make_slack

slax:	all
	rm -f callcommander-$(VERSION)-$(ARCH)-$(RPM_RELEASE).mo
	./make_slack
	tgz2mo callcommander-$(VERSION)-$(ARCH)-$(RPM_RELEASE).tgz callcommander-$(VERSION)-$(ARCH)-$(RPM_RELEASE).mo

zip:	dist
	tar -zxf callcommander-@VERSION@.tar.gz
	zip -r callcommander-@VERSION@.zip callcommander-@VERSION@
	rm -rf callcommander-@VERSION@

CLEANFILES = *~ moc_* *.tgz *.mo *.rpm *.zip Makefile.Release Makefile.Debug\
             *.dmg
DISTCLEANFILES = *~ moc_* *.tar.gz *.spec make_slack slack-desc rc.mld\
                 rc.mcidmd rc.mlmld make_bundle.sh
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 moc_* *.tar.gz\
                       depcomp install-sh missing mkinstalldirs\
                       config.guess config.sub

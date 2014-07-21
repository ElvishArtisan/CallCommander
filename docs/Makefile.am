## automake.am
##
## docs/ Automake.am for mithlond
##
## by Fred Gleason <fredg@paravelsystems.com>
##
## Use automake to process this into a Makefile.in

SUBDIRS = tables

man_MANS = ml.1 mskimmer.1 ml.conf.5 callcommander.1 mcidmd.8
EXTRA_DIST = $(man_MANS) mld.conf.5 mld.8 virtual_protocol.odt\
             ASTERISK.txt win32-license.rtf


CLEANFILES = *~
DISTCLEANFILES = moc_*
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 moc_* *.tar.gz

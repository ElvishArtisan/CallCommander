## automake.am
##
## Automake.am for callcommander/mcallman
##
## (C) Copyright 2005-2006 Fred Gleason <fredg@paravelsystems.com>
##
##  $Id: Makefile.am,v 1.22 2013/06/06 03:55:46 cvs Exp $
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

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -DQT3_SUPPORT -I$(top_srcdir)/libp @LOCAL_CPPFLAGS@
LIBS = -L$(top_srcdir)/libp
MOC = @MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

clean-local:	
	rm -rf debug release

bin_PROGRAMS = mcallman

dist_mcallman_SOURCES = add_action.cpp add_action.h\
                        add_report.cpp add_report.h\
                        detail_listview.cpp detail_listview.h\
                        edit_action.cpp edit_action.h\
                        edit_report.cpp edit_report.h\
                        filter_dialog.cpp filter_dialog.h\
                        globals.h\
                        list_actions.cpp list_actions.h\
                        list_details.cpp list_details.h\
                        list_reports.cpp list_reports.h\
                        list_winners.cpp list_winners.h\
                        mcallman.cpp mcallman.h\
                        peg_counter.cpp peg_counter.h\
                        print_report.cpp print_report.h\
                        select_operation.cpp select_operation.h\
                        show_detail.cpp show_detail.h\
                        standard_age_report.cpp area_code_report.cpp\
                        winner_listview.cpp winner_listview.h\
                        winner_listviewitem.cpp winner_listviewitem.h

nodist_mcallman_SOURCES = moc_add_action.cpp\
                          moc_add_report.cpp\
                          moc_detail_listview.cpp\
                          moc_edit_action.cpp\
                          moc_edit_report.cpp\
                          moc_filter_dialog.cpp\
                          moc_list_reports.cpp\
                          moc_list_actions.cpp\
                          moc_list_details.cpp\
                          moc_list_winners.cpp\
                          moc_mcallman.cpp\
                          moc_print_report.cpp\
                          moc_select_operation.cpp\
                          moc_show_detail.cpp\
                          moc_winner_listview.cpp

mcallman_LDADD = @QTLIBS@ -lml -lssl
mcallman_LDFLAGS = @QTFLAGS@

EXTRA_DIST = CallManager-Info.plist.in\
             mcallman.pro\
             mcallman.res

CLEANFILES = *~\
             debug\
             Makefile.Debug\
             Makefile.Release\
             moc_*\
             release

DISTCLEANFILES = CallManager-Info.plist

MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
                       configure\
                       Makefile.in\
                       moc_*

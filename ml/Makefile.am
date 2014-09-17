## automake.am
##
## ml/ Makefile.am for CallCommander
##
## by Fred Gleason <fredg@paravelsystems.com>
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

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -DQT3_SUPPORT -I../mlhpi -I$(top_srcdir)/libp
LIBS = -L$(top_srcdir)/libp
MOC = @MOC@

# QT's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

clean-local:	
	rm -rf debug release

bin_PROGRAMS = ml
dist_ml_SOURCES = callers.cpp callers.h\
                  callcomment.cpp callcomment.h\
                  callinfo.cpp callinfo.h\
                  change_nickname.cpp change_nickname.h\
                  client_listview.cpp client_listview.h\
                  client_listviewitem.cpp client_listviewitem.h\
                  comment.cpp comment.h\
                  config.cpp defaults.h drawcaps.cpp\
                  keybox.cpp keybox.h\
                  line_button.cpp line_button.h\
                  line_warning.cpp line_warning.h globals.h\
                  list_clients.cpp list_clients.h\
                  message.cpp message.h\
                  mithlond.cpp mithlond.h\
                  msg_textedit.cpp msg_textedit.h\
                  redial_dialog.cpp redial_dialog.h\
                  sendmsg_textedit.cpp sendmsg_textedit.h

nodist_ml_SOURCES = moc_callcomment.cpp\
                    moc_callinfo.cpp\
                    moc_change_nickname.cpp\
                    moc_client_listview.cpp\
                    moc_comment.cpp\
                    moc_keybox.cpp\
                    moc_line_button.cpp\
                    moc_line_warning.cpp\
                    moc_list_clients.cpp\
                    moc_message.cpp\
                    moc_mithlond.cpp moc_callers.cpp\
                    moc_msg_textedit.cpp\
                    moc_redial_dialog.cpp\
                    moc_sendmsg_textedit.cpp

ml_LDADD = @QTLIBS@ -lssl -lml
ml_LDFLAGS = -L/usr/local/mysql/lib @QTFLAGS@

EXTRA_DIST = CallScreener-Info.plist.in\
             ml.pro\
             ml.res

CLEANFILES = *~\
             *.exe\
             *.idb\
             *.ilk \
             *.obj\
             *.pdb\
             debug\
             Makefile.Release\
             Makefile.Debug\
             moc_*\
             release

DISTCLEANFILES = CallScreener-Info.plist\
                 Makefile.Debug\
                 Makefile.Release\
                 moc_*

MAINTAINERCLEANFILES = *~\
                       Makefile.in\
                       moc_*

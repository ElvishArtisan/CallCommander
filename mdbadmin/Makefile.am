## automake.am
##
## Automake.am for callcommander/mdbadmin
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

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -DQT3_SUPPORT -I../mlhpi -I$(top_srcdir)/libp @LOCAL_CPPFLAGS@
LIBS = -L$(top_srcdir)/libp
MOC = @MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

clean-local:	
	rm -rf debug release

bin_PROGRAMS = mdbadmin

dist_mdbadmin_SOURCES = add_callerid_source.cpp add_callerid_source.h\
                        add_directory.cpp add_directory.h\
                        add_map_line.cpp add_map_line.h\
                        add_show.cpp add_show.h\
                        add_system.cpp add_system.h\
                        add_user.cpp add_user.h\
                        createdb.cpp createdb.h\
                        edit_callerid_source.cpp edit_callerid_source.h\
                        edit_directory.cpp edit_directory.h\
                        edit_filter.cpp edit_filter.h\
                        edit_map.cpp edit_map.h\
                        edit_module.cpp edit_module.h\
                        edit_show.cpp edit_show.h\
                        edit_show_slot.cpp edit_show_slot.h\
                        edit_system.cpp edit_system.h\
                        edit_user.cpp edit_user.h\
                        globals.h\
                        list_callerid_sources.cpp list_callerid_sources.h\
                        list_directory.cpp list_directory.h\
                        list_modules.cpp list_modules.h\
                        list_show_configs.cpp list_show_configs.h\
                        list_systems.cpp list_systems.h\
                        list_users.cpp list_users.h\
                        mdbadmin.cpp mdbadmin.h\
                        mysql_login.cpp mysql_login.h\
                        name_sub.cpp name_sub.h\
                        opendb.cpp opendb.h\
                        purge_directory.cpp purge_directory.h\
                        select_callerid_source.cpp select_callerid_source.h\
                        system_info.cpp system_info.h

nodist_mdbadmin_SOURCES = moc_add_callerid_source.cpp\
                          moc_add_directory.cpp\
                          moc_add_map_line.cpp\
                          moc_add_system.cpp\
                          moc_add_user.cpp\
                          moc_edit_directory.cpp\
                          moc_add_show.cpp\
                          moc_edit_callerid_source.cpp\
                          moc_edit_filter.cpp\
                          moc_edit_map.cpp\
                          moc_edit_module.cpp\
                          moc_edit_show.cpp\
                          moc_edit_show_slot.cpp\
                          moc_edit_system.cpp\
                          moc_edit_user.cpp\
                          moc_list_callerid_sources.cpp\
                          moc_list_directory.cpp\
                          moc_list_modules.cpp\
                          moc_list_show_configs.cpp\
                          moc_list_systems.cpp\
                          moc_list_users.cpp\
                          moc_mdbadmin.cpp\
                          moc_mysql_login.cpp\
                          moc_purge_directory.cpp\
                          moc_select_callerid_source.cpp\
                          moc_system_info.cpp

mdbadmin_LDADD = @QTLIBS@ -lml -lssl
mdbadmin_LDFLAGS = @QTFLAGS@

EXTRA_DIST = CallAdministrator-Info.plist.in\
             mdbadmin.pro\
             mdbadmin.res

CLEANFILES = *~\
             *.exe\
             *.obj\
             debug\
             Makefile.Release\
             Makefile.Debug\
             moc_*\
             release

DISTCLEANFILES = CallAdministrator-Info.plist

MAINTAINERCLEANFILES = *~\
                       Makefile.in\
                       moc_*

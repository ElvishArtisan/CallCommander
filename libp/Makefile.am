## automake.am
##
## Automake.am for callcommander/libp
##
## by Fred Gleason <fredg@paravelsystems.com>
##
## Use automake to process this into a Makefile.in

AM_CPPFLAGS = -Wall -I../mlhpi -DQT3_SUPPORT
MOC = @MOC@
CWRAP = ../helpers/cwrap

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

# The cwrap dependency
html_%.cpp:	%.html
	$(CWRAP) -o $@ $<

clean-local:	
	rm -rf debug release

instdir = @LOCAL_PREFIX@/lib

lib_LTLIBRARIES = libml.la
dist_libml_la_SOURCES = askbox.cpp askbox.h\
                        askdata.cpp askdata.h\
                        asterisk_driver.cpp asterisk_driver.h\
                        bus_driver.cpp bus_driver.h\
                        call_meta_data.cpp call_meta_data.h\
                        calleriddetail.cpp calleriddetail.h\
                        calleridtrap.cpp calleridtrap.h\
                        change_password.cpp change_password.h\
                        client_data.cpp client_data.h\
                        cmdswitch.cpp cmdswitch.h\
                        conn_view_item.cpp conn_view_item.h\
                        connection_data.cpp connection_data.h\
                        console_data.cpp console_data.h\
                        dbdialog.cpp dbdialog.h\
                        edit_connection.cpp edit_connection.h\
                        edit_connection_data.cpp edit_connection_data.h\
                        edit_ttysettings.cpp edit_ttysettings.h\
                        edit_winner.cpp edit_winner.h\
                        escapestring.cpp escapestring.h\
                        list_connections.cpp list_connections.h\
                        list_consoles.cpp list_consoles.h\
                        list_devices.cpp list_devices.h\
                        list_shows.cpp list_shows.h\
                        login.cpp login.h\
                        message_data.cpp message_data.h\
                        ml.h\
                        ml_listviewitem.cpp ml_listviewitem.h\
                        ml_reload_packet.cpp ml_reload_packet.h\
                        ml_timeslot.cpp ml_timeslot.h\
                        ml_timeslotlist.cpp ml_timeslotlist.h\
                        mlami.cpp mlami.h\
                        mlasteriskconfig.cpp mlasteriskconfig.h\
                        mlconf.cpp mlconf.h\
                        mlconfig.cpp mlconfig.h\
                        mldconfig.cpp mldconfig.h\
                        mldatedialog.cpp mldatedialog.h\
                        mldatepicker.cpp mldatepicker.h\
                        mldshow.cpp mldshow.h\
                        mllabel.cpp mllabel.h\
                        mllicense.cpp mllicense.h\
                        mllistselector.cpp mllistselector.h\
                        mlpaths.h\
                        mlplaymeter.cpp mlplaymeter.h\
                        mlprofile.cpp mlprofile.h\
                        mlprofileline.cpp mlprofileline.h\
                        mlprofilesection.cpp mlprofilesection.h\
                        mlpushbutton.cpp mlpushbutton.h\
                        mlsegclock.cpp mlsegclock.h\
                        mlseglist.cpp mlseglist.h\
                        mlsegment.cpp mlsegment.h\
                        mlsegmeter.cpp mlsegmeter.h\
                        mltimeengine.cpp mltimeengine.h\
                        mltimeevent.cpp mltimeevent.h\
                        mltransportbutton.cpp mltransportbutton.h\
                        mlttydevice.cpp mlttydevice.h\
                        number_list.cpp number_list.h\
                        telos100_driver.cpp telos100_driver.h\
                        telos100_states.cpp telos100_states.h\
                        telos2101_callbacks.cpp\
                        telos2101_driver.cpp telos2101_driver.h\
                        tlsaplib.cpp tlsaplib.h\
                        ts612_driver.cpp ts612_driver.h\
                        ttysettings.cpp ttysettings.h\
                        save_connection.cpp save_connection.h\
                        show_data.cpp show_data.h\
                        shows.cpp shows.h\
                        virtual_driver.cpp virtual_driver.h

nodist_libml_la_SOURCES = html_gpl2.cpp\
                          moc_mlami.cpp\
                          moc_askbox.cpp\
                          moc_asterisk_driver.cpp\
                          moc_bus_driver.cpp\
                          moc_calleridtrap.cpp\
                          moc_change_password.cpp\
                          moc_dbdialog.cpp\
                          moc_edit_connection.cpp\
                          moc_edit_connection_data.cpp\
                          moc_edit_ttysettings.cpp\
                          moc_edit_winner.cpp\
                          moc_list_connections.cpp\
                          moc_list_consoles.cpp\
                          moc_list_devices.cpp\
                          moc_list_shows.cpp\
                          moc_login.cpp\
                          moc_mldatedialog.cpp\
                          moc_mldatepicker.cpp\
                          moc_mllabel.cpp\
                          moc_mllicense.cpp\
                          moc_mllistselector.cpp\
                          moc_mlplaymeter.cpp\
                          moc_mlpushbutton.cpp\
                          moc_mlsegclock.cpp\
                          moc_mlsegmeter.cpp\
                          moc_mltimeengine.cpp\
                          moc_mltransportbutton.cpp\
                          moc_mlttydevice.cpp\
                          moc_save_connection.cpp\
                          moc_telos100_driver.cpp\
                          moc_telos2101_driver.cpp\
                          moc_ts612_driver.cpp\
                          moc_virtual_driver.cpp

libml_la_LDFLAGS = -release $(VERSION)

EXTRA_DIST = gpl2.html\
             html_gpl2_win32.cpp\
             libp.pro\
             mlpaths.h.in\
             mlttydevice_win32.cpp

CLEANFILES = *~\
             *.idb\
             *.ilk\
             *.lib\
             *.obj\
             *.pdb\
             moc_*

DISTCLEANFILES = Makefile.Debug\
                 Makefile.Release\
                 mlpaths.h

MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
                       configure\
                       Makefile.in\
                       moc_*

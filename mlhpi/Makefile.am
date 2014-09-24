## automake.am
##
## Automake.am for callcommander/mlhpi
##
## by Fred Gleason <fredg@paravelsystems.com>
##
## Use automake to process this into a Makefile.in

AM_CPPFLAGS = -Wall -DQT3_SUPPORT -I../libp -L../libp @LOCAL_CPPFLAGS@
MOC = @MOC@
CWRAP = ../helpers/cwrap

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

# The cwrap dependency
html_%.cpp:	%.html
	$(CWRAP) -o $@ $<

instdir = @LOCAL_PREFIX@/lib

lib_LTLIBRARIES = libmlhpi.la
dist_libmlhpi_la_SOURCES = mlhpiinformation.cpp mlhpiinformation.h\
                           mlhpiplaystream.cpp mlhpiplaystream.h\
                           mlhpisoundcard.cpp mlhpisoundcard.h\
                           mlhpirecordstream.cpp mlhpirecordstream.h\
                           mlwavedata.cpp mlwavedata.h\
                           mlwavefile.cpp mlwavefile.h

nodist_libmlhpi_la_SOURCES = moc_mlhpiplaystream.cpp\
                             moc_mlhpirecordstream.cpp\
                             moc_mlhpisoundcard.cpp

libmlhpi_la_LDFLAGS = -release $(VERSION)

EXTRA_DIST = mlhpi.pro

CLEANFILES = *~\
             moc_*

MAINTAINERCLEANFILES = *~\
                       Makefile.in\
                       moc_*

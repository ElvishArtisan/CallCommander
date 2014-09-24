dnl configure.ac
dnl
dnl   (C) Copyright 2002-2014 Fred Gleason <fredg@paravelsystems.com>
dnl
dnl   This program is free software; you can redistribute it and/or modify
dnl   it under the terms of the GNU General Public License as
dnl   published by the Free Software Foundation; either version 2 of
dnl   the License, or (at your option) any later version.
dnl
dnl   This program is distributed in the hope that it will be useful,
dnl   but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl   GNU General Public License for more details.
dnl
dnl   You should have received a copy of the GNU General Public
dnl   License along with this program; if not, write to the Free Software
dnl   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
dnl
dnl Use autoconf to process this into a configure script
AC_INIT(callcommander,m4_esyscmd(cat VERSION))
AM_INIT_AUTOMAKE
AC_SUBST(RPM_RELEASE,1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])
AC_CONFIG_MACRO_DIR([m4])

#
# Some Fully Resolved Install Paths
# (for the packaging system and friends)
#
if test ${prefix} = "NONE" ; then
  AC_SUBST(LOCAL_PREFIX,/usr/local)
else
  AC_SUBST(LOCAL_PREFIX,${prefix})
fi

#
# Basic Compiler Checks
#
AC_PROG_CC
AC_PROG_CXX
AC_LANG(C++)
AC_PROG_LIBTOOL

#
# Option Switches
#
AC_ARG_ENABLE(hpi,[  --disable-hpi           disable AudioScience HPI sound support],[HPI_DISABLED=yes],[])
AC_ARG_WITH(debug,[  --with-debug            include debugging symbols in RPMs],[].[STRIP=yes])

#
# Determine the target architecture
#
AR_GCC_TARGET()
AR_GET_DISTRO()
AC_SUBST(VENDOR,$ar_gcc_distro)
AC_SUBST(ARCH,$ar_gcc_arch)
if test $ar_gcc_distro = "apple" ; then
   AC_DEFINE(OS_X)
else
   AC_DEFINE(LINUX)
fi

#
# Check for Qt
#
AC_CHECK_HEADER(QtCore/QtCore,[],[AC_MSG_ERROR([*** Qt4 not found, unable to continue ***])])
AC_CHECK_PROG(MOC_NAME,moc-qt4,[moc-qt4],[moc])
AC_SUBST(MOC,$MOC_NAME)

#
# Debug Build Configuration
#
if test -n $STRIP ; then
  AC_SUBST(STRIP_DEBUG,yes)
else
  AC_SUBST(STRIP_DEBUG,"")
fi

#
# Check for OggVorbis
#
AC_CHECK_HEADER(vorbis/vorbisfile.h,[VORBIS_FOUND=yes],[])
if test "$VORBIS_FOUND" = "yes" ; then
  AC_DEFINE(HAVE_VORBIS)
  AC_SUBST(LIBVORBIS,"-lvorbisfile -lvorbisenc")
else
  AC_SUBST(LIBVORBIS,"")
fi

#
# Sound Drivers
#
AC_CHECK_HEADER(asihpi/hpi.h,[HPI_FOUND=yes],[])

#
# Setup Sound API Dependencies
#
if test $HPI_FOUND ; then
  if test -z $HPI_DISABLED ; then
    AC_DEFINE(SOUND_SUPPORT,"yes")
    AC_DEFINE(HPI,yes)
    AC_SUBST(SOUND_FLAG)
    AC_SUBST(LIBHPI,"-lmlhpi -lhpi")
    USING_HPI=yes
    AM_CONDITIONAL(BUILD_SOUND,true)
  else 
    AC_SUBST(SOUND_FLAG,"--disable-hpi")
    AC_SUBST(LIBHPI,"")
    AM_CONDITIONAL(BUILD_SOUND,false)
  fi
else
  AC_SUBST(SOUND_FLAG)
  AC_SUBST(LIBHPI,"")
  AM_CONDITIONAL(BUILD_SOUND,false)
fi

#
# Configure Linker Flags
#
if test $ar_gcc_distro = "apple" ; then
  AC_SUBST(QTLIBS,"")
  AC_SUBST(QTFLAGS,"-mmacosx-version-min=10.5 -framework QtCore -framework QtGui -framework QtNetwork -framework QtSql -framework Qt3Support")
  AC_SUBST(LOCAL_CPPFLAGS,"-mmacosx-version-min=10.5")
else
  AC_SUBST(QTLIBS,"-lQtCore -lQtGui -lQtNetwork -lQtSql -lQt3Support")
  AC_SUBST(QTFLAGS,"")
  AC_SUBST(LOCAL_CPPFLAGS,"")
fi

#
# Configure RPM Build
#
AC_MSG_CHECKING([for $WIN32_SETUPS/callcommander-$VERSION-$RPM_RELEASE.exe])
if test -f $WIN32_SETUPS/callcommander-$VERSION-$RPM_RELEASE.exe ; then
  AC_MSG_RESULT([yes])
  AC_SUBST(WIN32_SOURCE,"$WIN32_SETUPS/callcommander-$VERSION-$RPM_RELEASE.exe")
  AC_SUBST(WIN32_PATH,"/var/win32/callcommander-$VERSION-$RPM_RELEASE.exe")
else
  AC_MSG_RESULT([no])
  AC_SUBST(WIN32_SOURCE,"")
  AC_SUBST(WIN32_PATH,"")
fi

AC_MSG_CHECKING([for $OSX_SETUPS/callcommander-$VERSION-$RPM_RELEASE.dmg])
if test -f $OSX_SETUPS/callcommander-$VERSION-$RPM_RELEASE.dmg ; then
  AC_MSG_RESULT([yes])
  AC_SUBST(OSX_SOURCE,"$OSX_SETUPS/callcommander-$VERSION-$RPM_RELEASE.dmg")
  AC_SUBST(OSX_PATH,"/var/osx/callcommander-$VERSION-$RPM_RELEASE.dmg")
else
  AC_MSG_RESULT([no])
  AC_SUBST(OSX_SOURCE,"")
  AC_SUBST(OSX_PATH,"")
fi

AC_CHECK_PROG(RPMBUILD_FOUND,rpmbuild,[yes],[])
if test -z $RPMBUILD_FOUND ; then
  AC_SUBST(RPMBUILD,rpm)
else
  AC_SUBST(RPMBUILD,rpmbuild)
fi
if test -d /usr/src/redhat ; then
  AC_SUBST(RPM_ROOT,/usr/src/redhat)
else
  AC_SUBST(RPM_ROOT,/usr/src/packages)
fi 
if test $ar_gcc_arch = x86_64 ; then
  AC_SUBST(CALL_LIB_PATH,lib64)
else
  AC_SUBST(CALL_LIB_PATH,lib)
fi

AC_OUTPUT(Makefile \
          callcommander.spec \
          rc.mld \
          rc.mcidmd \
          rc.mlmld \
          make_slack \
          slack-desc \
          make_bundle.sh \
          helpers/Makefile \
          docs/Makefile \
          docs/tables/Makefile \
          samples/Makefile \
          icons/Makefile \
          xdg/Makefile \
	  libp/Makefile \
          libp/mlpaths.h \
          mlhpi/Makefile \
          ml/Makefile \
          ml/CallScreener-Info.plist \
          mld/Makefile \
          mcidmd/Makefile \
	  scripts/Makefile \
	  scripts/twinkle_hybrid.sh \
	  scripts/twinkle.cfg \
	  mdbadmin/Makefile \
          mdbadmin/CallAdministrator-Info.plist \
	  mcallman/Makefile \
          mcallman/CallManager-Info.plist \
	  mlm/Makefile \
	  mlmh/Makefile \
	  mlmld/Makefile \
	  mcs/Makefile \
	  asterisk/Makefile \
)
chmod 755 rc.mld
chmod 755 rc.mcidmd
chmod 755 make_slack
chmod 755 make_bundle.sh
chmod 755 scripts/twinkle_hybrid.sh

AC_MSG_NOTICE()
if test $WIN32_SOURCE ; then
AC_MSG_NOTICE([Including RPM win32 setup from:])
AC_MSG_NOTICE([  $WIN32_SOURCE])
AC_MSG_NOTICE()
fi
if test $OSX_SOURCE ; then
AC_MSG_NOTICE([Including RPM OS-X setup from:])
AC_MSG_NOTICE([  $OSX_SOURCE])
AC_MSG_NOTICE()
fi
AC_MSG_NOTICE()
AC_MSG_NOTICE(Now enter 'make' to build the software.)
AC_MSG_NOTICE()

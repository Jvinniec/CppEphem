
dnl Initialize the build system
dnl DONT FORGET TO CHANGE THE VERSION!
AC_INIT([CppEphem],[MASTER],[jvinniec <at> gmail.com])
AC_CONFIG_SRCDIR([CppEphem/src/mjd2jd.cpp])

echo ""
echo "================================================="
echo " Configuring ${PACKAGE_NAME} - ${PACKAGE_VERSION}"
echo "================================================="

dnl Run the automake stuff
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror])

dnl Make sure there is a c++ compiler installed
AC_PROG_CC
AC_PROG_CXX([clang++ g++])
AC_LANG_PUSH([C++])

dnl Other required programs
AM_PROG_AR
LT_INIT

dnl This variable will be used to specify special compile options
special_exec=
CppEphem_CPPFLAGS=
curDir=${PWD}
dnl #################################
dnl SOFA PACKAGE OPTIONS
dnl #################################
sofalibs=
sofa_CXXFLAGS=
have_sofa=no

AC_ARG_WITH([sofa],
	AS_HELP_STRING([--with-sofa],
		[directory containing 'sofa.h']))
AC_ARG_WITH([sofa-libdir],
	AS_HELP_STRING([--with-sofa-libdir],
		[directory containing 'libsofa.a']))
AC_ARG_WITH([sofa-incdir],
	AS_HELP_STRING([--with-sofa-incdir],
		[directory containing 'sofa.h']))

dnl The following is true if '--with-sofa' is specified
if (test "x$with_sofa" != x) && (test -d ${with_sofa}); then
   sofalibs="-L$with_sofa -lsofa_c"
   sofa_CXXFLAGS="-I$with_sofa ${sofa_CXXFLAGS}"
fi

dnl The following is true if '--with-wcs-include' is provided
if test "x$with_sofa_incdir" != x; then
   sofa_CXXFLAGS="-I$with_sofa_incdir ${sofa_CXXFLAGS}"
fi

dnl The following is true if '--with-wcs-lib' is provided
if test "x$with_sofa_libdir" != x; then
   wcslibs="-L$with_sofa_libdir -lsofa_c $wcslibs"
fi

dnl The following is true if none of the '--with-sofa...' options
dnl were specified
if test "x$with_sofa$with_sofa_incdir$with_sofa_libdir" = x ; then
   sofalibs="-L${curDir}/sofa/20150209_a/c/src -lsofa_c ${sofalibs}"
   sofa_CXXFLAGS="-I${curDir}/sofa/20150209_a/c/src ${sofa_CXXFLAGS}"
fi

echo "$sofa_CXXFLAGS"
echo "$sofalibs"

dnl Add the include flags and the library search paths
LIBS="${sofalibs} ${LIBS}"

dnl Library checks
have_sofa=yes
# AC_CHECK_LIB([sofa_c],[iauJd2cal],
#	[have_sofa=yes],
#	[AC_MSG_ERROR([sofa library does not contain 'iauJd2cal']) have_sofa=no])
#AC_CHECK_LIB([sofa_c],[iauCal2jd],
#	[have_sofa=yes],
#	[AC_MSG_ERROR([sofa library does not contain 'iauCal2jd'])])

if test "x${have_sofa}" != xyes; then
   echo "------------------------------------"
   echo " ERROR :: -lsofa_c"
   echo " Could not find required methods in"
   echo " sofa library."
   echo "------------------------------------"
   (exit 1); exit 1;
fi

dnl Add the sofa_CXXFLAGS to the variables accessible in Makefiles
AC_SUBST(sofa_CXXFLAGS)


dnl #################################
dnl ADDITIONAL OPTIONS FOR '--enable-'/'--disable-'
dnl #################################


dnl #################################
dnl ADDITIONAL OPTIONS FOR '--with-'/'--without-'
dnl #################################


dnl #################################
dnl CHECK FOR SPECIFIED PROGRAMS
dnl #################################


dnl #################################
dnl CHECK FOR STANDARD STUFF
dnl #################################

dnl Check for standard library functions
AC_CHECK_FUNCS([floor pow sqrt]) 


dnl #################################
dnl Specify the Makefile locations
dnl #################################
AC_CONFIG_FILES([Makefile CppEphem/src/Makefile])

dnl 
AC_OUTPUT

dnl Print some helpful information to the user
echo \
"
=================================================
 ${PACKAGE_NAME} Version ${PACKAGE_VERSION}
=================================================
 Prefix: '${prefix}'
 C Compiler   : ${CC}
 C++ Compiler : ${CXX}
 CFLAGS  : ${CFLAGS}
 CPPFLAGS: ${CPPFLAGS}
 CXXFLAGS: ${CXXFLAGS}

 Additional compiler flags:
 --------------------------
${sofa_CXXFLAGS} ${CppEphem_CPPFLAGS}

 LDFLAGS : ${LDFLAGS}

 Additional linker flags:
 ------------------------
$LIBS

 Package features:
   Special Execution: ${special_exec}

 Now type 'make @<:@<target>@:>@'
   where the optional <target> is:
     all             - build all binaries
     install         - install everything
=================================================
"
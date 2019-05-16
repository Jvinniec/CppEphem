
dnl Initialize the build system
dnl DONT FORGET TO CHANGE THE VERSION!

m4_define([CppEphem_VERSION_MAJOR], [1])
m4_define([CppEphem_VERSION_MINOR], [2])
m4_define([CppEphem_VERSION_PATCH], [99])
m4_define([cppephem_version],
          [CppEphem_VERSION_MAJOR.CppEphem_VERSION_MINOR.CppEphem_VERSION_PATCH])

AC_INIT([CppEphem],[cppephem_version],[jvinniec <at> gmail.com])
AC_CONFIG_SRCDIR([cppephem/src/mjd2jd.cpp],[sofa/src/cal2jd.c])
AC_CONFIG_HEADER([config.h])

echo ""
echo "================================================="
echo " Configuring ${PACKAGE_NAME} - ${PACKAGE_VERSION}"
echo "================================================="

dnl Run the automake stuff
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign])

dnl Make sure there is a c++ compiler installed
AC_PROG_CC([clang gcc])
AC_PROG_CXX([clang++ g++])
AC_LANG_PUSH([C++])

dnl Other required programs
AM_PROG_AR
LT_INIT

dnl This variable will be used to specify special compile options
special_exec=
CppEphem_CPPFLAGS="-DCPPEPHEM_VERSION=\\\""cppephem_version"\\\""

curDir=${PWD}

BUILDSUBDIRS="cppephem/src"

dnl #################################
dnl SOFA PACKAGE OPTIONS
dnl #################################
sofalibs=
sofa_CXXFLAGS=
have_sofa=no
sofa_external=no

# Set the default sofa directory to the included one
SOFADIR="${curDir}/sofa/src/"

AC_ARG_WITH([sofa],
	AS_HELP_STRING([--with-sofa],
		[directory containing 'sofa.h']))

dnl The following is true if '--with-sofa' is specified
if (test "x$with_sofa" != x) && (test -d ${with_sofa}); then
   SOFADIR="${with_sofa}"
   sofalibs="-L$with_sofa -lsofa_c"
   sofa_CXXFLAGS="-I$with_sofa ${sofa_CXXFLAGS}"
   have_sofa=yes
   sofa_external=yes
else
   # Add the default sofa directory to the directories to be built
   BUILDSUBDIRS="${SOFADIR} ${BUILDSUBDIRS}"
   sofalibs="-L${SOFADIR} -lsofa_c ${sofalibs}"
   sofa_CXXFLAGS="-I${SOFADIR} ${sofa_CXXFLAGS}"
   have_sofa=yes
fi


dnl Pass the SOFADIR to the makefiles
AC_SUBST(SOFADIR)
AC_SUBST(sofalibs)

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
AC_SUBST(CppEphem_CPPFLAGS)

dnl Add the BUILDSUBDIRS to the variables accessible in Makefiles
AC_SUBST(BUILDSUBDIRS)

dnl #################################
dnl ADDITIONAL OPTIONS FOR '--enable-'/'--disable-'
dnl #################################


dnl #################################
dnl ADDITIONAL OPTIONS FOR '--with-'/'--without-'
dnl #################################

# Corrections directory
cecorrfilepath="${datarootdir}/cppephem/"
cleanup_corrfilepath="rm -r ${cecorrfilepath}"

AC_ARG_WITH([cecorrfilepath],
	AS_HELP_STRING([--with-cecorrfilepath],
		[directory corrections files for 'UT1-UTC' and 'TT-UT1']))

# The following is true if '--with-cecorrfilepath' is specified
if (test "x$with_cecorrfilepath" != x) && (test -d ${with_cecorrfilepath}); then
   cecorrfilepath="${with_cecorrfilepath}/"
   cleanup_corrfilepath=""
fi
# Add the corrections directory
CppEphem_CPPFLAGS="${CppEphem_CPPFLAGS} -DCECORRFILEPATH=\\\"${cecorrfilepath}\\\""
AC_SUBST(cecorrfilepath)
AC_SUBST(cleanup_corrfilepath)

# Add libcurl support
curllibs="-lcurl"
enable_curl=yes

AC_ARG_ENABLE([curl],
	AS_HELP_STRING([--enable-curl],
		[disable support for downloading corrections files via libcurl]),
  [enable_curl=yes], [enable_curl=no])

dnl The following is true if '--enable-curl' is specified
AS_IF([test "x$enable_curl" = "xno"], [
  dnl Do the stuff needed for enabling the feature
  CppEphem_CPPFLAGS="${CppEphem_CPPFLAGS} -DNOCURL"
  curllibs = ""
])
# Add the corrections directory
CppEphem_CPPFLAGS="${CppEphem_CPPFLAGS} -DCECORRFILEPATH=\\\"${cecorrfilepath}\\\""
AC_SUBST(curllibs)


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

AC_CONFIG_FILES([Makefile sofa/src/Makefile cppephem/src/Makefile])

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
 SOFADIR : ${SOFADIR}
 Corrections Dir : '${cecorrfilepath}'

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

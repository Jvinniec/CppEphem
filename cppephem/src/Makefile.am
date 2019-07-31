## Process this file with automake to produce Makefile.in
#
# Author: Josh V. Cardenzana, jvinniec <at> gmail.com
# Date: April, 2016
#

AM_CXXFLAGS = -std=c++11 $(sofa_CXXFLAGS) $(CppEphem_CPPFLAGS)
AM_CPPFLAGS = -I$(top_srcdir)/cppephem/include \
              -I$(top_srcdir)/cppephem/support \
              -I$(SOFADIR) \
              $(CppEphem_CPPFLAGS)

lib_LTLIBRARIES = libcppephem.la

libcppephem_la_SOURCES = CENamespace.cpp \
                         CEBody.cpp \
                         CECoordinates.cpp \
                         CECorrections.cpp \
                         CEDate.cpp \
                         CEException.cpp \
                         CEObservation.cpp \
                         CEObserver.cpp \
                         CEPlanet.cpp \
                         CERunningDate.cpp \
                         CESkyCoord.cpp \
                         CETime.cpp

headers = ../include/CppEphem.h \
                  ../include/CENamespace.h \
                  ../include/CEBody.h \
                  ../include/CECoordinates.h \
                  ../include/CECorrections.h \
                  ../include/CEDate.h \
                  ../include/CEException.h \
                  ../include/CEObservation.h \
                  ../include/CEObserver.h \
                  ../include/CEPlanet.h \
                  ../include/CERunningDate.h \
                  ../include/CESkyCoord.h \
                  ../include/CETime.h

include_HEADERS = $(headers)

#libcppephem_la_CPPFLAGS = $(CppEphem_CPPFLAGS) $(AM_CPPFLAGS)
#libcppephem_la_CXXFLAGS = $(CppEphem_CPPFLAGS)
libcppephem_la_LIBADD = $(LIBS) $(sofalibs) $(curllibs)
libcppephem_ladir = $(libdir)

bin_PROGRAMS = cal2jd cal2mjd jd2cal jd2mjd mjd2jd mjd2cal cirs2icrs cirs2gal cirs2obs icrs2cirs icrs2gal icrs2obs gal2cirs gal2icrs gal2obs obs2cirs obs2icrs obs2gal angsep planetephem

##########################################
# DATE CONVERSION EXECUTABLES
##########################################

# CALENDAR to JULIAN DATE
cal2jd_SOURCES = cal2jd.cpp
cal2jd_LDADD = libcppephem.la

# CALENDAR to MODIFIED JULIAN DATE
cal2mjd_SOURCES = cal2mjd.cpp
cal2mjd_LDADD = libcppephem.la

# JULIAN DATE to CALENDAR
jd2cal_SOURCES = jd2cal.cpp
jd2cal_LDADD = libcppephem.la

# JULIAN DATE to MODIFIED JULIAN DATE
jd2mjd_SOURCES = jd2mjd.cpp
jd2mjd_LDADD = libcppephem.la

# MODIFIED JULIAN DATE to JULIAN DATE
mjd2jd_SOURCES = mjd2jd.cpp 
mjd2jd_LDADD = libcppephem.la

# MODIFIED JULIAN DATE to CALENDAR
mjd2cal_SOURCES = mjd2cal.cpp
mjd2cal_LDADD = libcppephem.la

##########################################
# TIME CONVERSION EXECUTABLES
##########################################


##########################################
# COORDINATE CONVERSION EXECUTABLES
##########################################

# CIRS to ICRS coordinates
cirs2icrs_SOURCES = cirs2icrs.cpp
cirs2icrs_LDADD = libcppephem.la

# CIRS to GALACTIC coordinates
cirs2gal_SOURCES = cirs2gal.cpp
cirs2gal_LDADD = libcppephem.la

# CIRS to LOCAL coordinates
cirs2obs_SOURCES = cirs2obs.cpp
cirs2obs_LDADD = libcppephem.la

# CIRS to LOCAL coordinates
icrs2obs_SOURCES = icrs2obs.cpp
icrs2obs_LDADD = libcppephem.la

# CIRS to GALACTIC coordinates
icrs2gal_SOURCES = icrs2gal.cpp
icrs2gal_LDADD = libcppephem.la

# CIRS to GALACTIC coordinates
icrs2cirs_SOURCES = icrs2cirs.cpp
icrs2cirs_LDADD = libcppephem.la

# GALACTIC to CIRS coordinates
gal2cirs_SOURCES = gal2cirs.cpp
gal2cirs_LDADD = libcppephem.la

# GALACTIC to ICRS coordinates
gal2icrs_SOURCES = gal2icrs.cpp
gal2icrs_LDADD = libcppephem.la

# GALACTIC to LOCAL coordinates
gal2obs_SOURCES = gal2obs.cpp
gal2obs_LDADD = libcppephem.la

# LOCAL to CIRS coordinates
obs2cirs_SOURCES = obs2cirs.cpp
obs2cirs_LDADD = libcppephem.la

# LOCAL to ICRS coordinates
obs2icrs_SOURCES = obs2icrs.cpp
obs2icrs_LDADD = libcppephem.la

# LOCAL to GALACTIC coordinates
obs2gal_SOURCES = obs2gal.cpp
obs2gal_LDADD = libcppephem.la

##########################################
# ANGULAR SEPARATION
##########################################

angsep_SOURCES = angsep.cpp
angsep_LDADD = libcppephem.la

##########################################
# PLANET EPHEMERIS EXECUTABLES
##########################################

planetephem_SOURCES = planetephem.cpp
planetephem_LDADD = libcppephem.la

CLEANFILES=

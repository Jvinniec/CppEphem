## Process this file with automake to produce Makefile.in
#
# Author: Josh V. Cardenzana, jvinniec <at> gmail.com
# Date: April, 2016
#

AM_CXXFLAGS = -std=c++11 -I../include -I./ $(sofa_CXXFLAGS)
AM_CFLAGS = 

lib_LTLIBRARIES = libcppephem.la
libcppephem_la_SOURCES = CENamespace.cpp \
                        CEBody.cpp \
                        CECoordinates.cpp \
                        CEDate.cpp \
                        CEObservation.cpp \
                        CEObserver.cpp \
                        CEPlanet.cpp \
                        CERunningDate.cpp \
                        CETime.cpp
libcppephem_la_HEADERS = ../include/CppEphem.h \
                        ../include/CENamespace.h \
                        ../include/CEBody.h \
                        ../include/CECoordinates.h \
                        ../include/CEDate.h \
                        ../include/CEObservation.h \
                        ../include/CEObserver.h \
                        ../include/CEPlanet.h \
                        ../include/CERunningDate.h \
                        ../include/CETime.h
libcppephem_la_CPPFLAGS = $(CppEphem_CPPFLAGS)
libcppephem_la_LIBADD=$(LIBS)
libcppephem_ladir=$(libdir)

bin_PROGRAMS = cal2jd cal2mjd jd2cal jd2mjd mjd2jd mjd2cal cirs2obs cirs2gal gal2cirs gal2obs

##########################################
# DATE CONVERSION EXECUTABLES
##########################################

# CALENDAR to JULIAN DATE
cal2jd_SOURCES = cal2jd.cpp
cal2jd_LDADD = libcppephem.la
cal2jd_CPPFLAGS = $(CppEphem_CPPFLAGS)

# CALENDAR to MODIFIED JULIAN DATE
cal2mjd_SOURCES = cal2mjd.cpp
cal2mjd_LDADD = libcppephem.la
cal2mjd_CPPFLAGS = $(CppEphem_CPPFLAGS)

# JULIAN DATE to CALENDAR
jd2cal_SOURCES = jd2cal.cpp
jd2cal_LDADD = libcppephem.la
jd2cal_CPPFLAGS=$(CppEphem_CPPFLAGS)

# JULIAN DATE to MODIFIED JULIAN DATE
jd2mjd_SOURCES = jd2mjd.cpp
jd2mjd_LDADD = libcppephem.la
jd2mjd_CPPFLAGS=$(CppEphem_CPPFLAGS)

# MODIFIED JULIAN DATE to JULIAN DATE
mjd2jd_SOURCES = mjd2jd.cpp 
mjd2jd_LDADD = libcppephem.la
mjd2jd_CPPFLAGS=$(CppEphem_CPPFLAGS)

# MODIFIED JULIAN DATE to CALENDAR
mjd2cal_SOURCES = mjd2cal.cpp
mjd2cal_LDADD = libcppephem.la
mjd2cal_CPPFLAGS = $(CppEphem_CPPFLAGS)

##########################################
# TIME CONVERSION EXECUTABLES
##########################################


##########################################
# COORDINATE CONVERSION EXECUTABLES
##########################################

# CIRS to LOCAL coordinates
cirs2obs_SOURCES = cirs2obs.cpp
cirs2obs_LDADD = libcppephem.la
cirs2obs_CPPFLAGS = $(CppEphem_CPPFLAGS)

# CIRS to GALACTIC coordinates
cirs2gal_SOURCES = cirs2gal.cpp
cirs2gal_LDADD = libcppephem.la
cirs2gal_CPPFLAGS = $(CppEphem_CPPFLAGS)

# GALACTIC to CIRS coordinates
gal2cirs_SOURCES = gal2cirs.cpp
gal2cirs_LDADD = libcppephem.la
gal2cirs_CPPFLAGS = $(CppEphem_CPPFLAGS)

# GALACTIC to LOCAL coordinates
gal2obs_SOURCES = gal2obs.cpp
gal2obs_LDADD = libcppephem.la
gal2obs_CPPFLAGS = $(CppEphem_CPPFLAGS)

# LOCAL to CIRS coordinates
#obs2cirs_SOURCES = obs2cirs.cpp
#obs2cirs_LDADD = libcppephem.la
#obs2cirs_CPPFLAGS = $(CppEphem_CPPFLAGS)

##########################################
# PLANET EPHEMERIS EXECUTABLES
##########################################


CLEANFILES=
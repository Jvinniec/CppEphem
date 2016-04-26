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
                        CEObserver.cpp \
                        CETime.cpp
libcppephem_la_HEADERS = ../include/CppEphem.h \
                        ../include/CENamespace.h \
                        ../include/CEBody.h \
                        ../include/CECoordinates.h \
                        ../include/CEDate.h \
                        ../include/CEObserver.h \
                        ../include/CETime.h
libcppephem_la_CPPFLAGS = $(CppEphem_CPPFLAGS)
libcppephem_la_LIBADD=$(LIBS)
libcppephem_ladir=$(libdir)

bin_PROGRAMS = cal2jd cal2mjd jd2cal jd2mjd mjd2jd mjd2cal cirs2obs

##########################################
# DATE CONVERSION EXECUTABLES
##########################################

cal2jd_SOURCES = cal2jd.cpp
cal2jd_LDADD = libcppephem.la
cal2jd_CPPFLAGS = $(CppEphem_CPPFLAGS)

cal2mjd_SOURCES = cal2mjd.cpp
cal2mjd_LDADD = libcppephem.la
cal2mjd_CPPFLAGS = $(CppEphem_CPPFLAGS)

jd2cal_SOURCES = jd2cal.cpp
jd2cal_LDADD = libcppephem.la
jd2cal_CPPFLAGS=$(CppEphem_CPPFLAGS)

jd2mjd_SOURCES = jd2mjd.cpp
jd2mjd_LDADD = libcppephem.la
jd2mjd_CPPFLAGS=$(CppEphem_CPPFLAGS)

mjd2jd_SOURCES = mjd2jd.cpp 
mjd2jd_LDADD = libcppephem.la
mjd2jd_CPPFLAGS=$(CppEphem_CPPFLAGS)

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
cirs2obs_CPPFLAGS = $(CppEphem_CPPFLAGS)

# CIRS to GALACTIC coordinates
#cirs2gal_SOURCES = cirs2gal.cpp
#cirs2gal_LDADD = libcppephem.la
#cirs2gal_CPPFLAGS = $(CppEphem_CPPFLAGS)


##########################################
# PLANET EPHEMERIS EXECUTABLES
##########################################


CLEANFILES=
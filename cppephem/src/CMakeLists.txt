
#------------------------------------------
# Define include directories
#------------------------------------------
#include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../include ${SOFA_DIRECTORY})

#------------------------------------------
# Executables
#------------------------------------------
# Date converters
cppephem_executable (cal2jd  cal2jd.cpp)
cppephem_executable (cal2mjd cal2mjd.cpp)
cppephem_executable (jd2mjd  jd2mjd.cpp)
cppephem_executable (jd2cal  jd2cal.cpp)
cppephem_executable (mjd2jd  mjd2jd.cpp)
cppephem_executable (mjd2cal mjd2cal.cpp)

# Coordinate converters
cppephem_executable (cirs2gal cirs2gal.cpp)
cppephem_executable (cirs2icrs cirs2icrs.cpp)
cppephem_executable (cirs2obs cirs2obs.cpp)
cppephem_executable (icrs2cirs icrs2cirs.cpp)
cppephem_executable (icrs2gal icrs2gal.cpp)
cppephem_executable (icrs2obs icrs2obs.cpp)
cppephem_executable (gal2cirs gal2cirs.cpp)
cppephem_executable (gal2icrs gal2icrs.cpp)
cppephem_executable (gal2obs  gal2obs.cpp)
cppephem_executable (obs2cirs obs2cirs.cpp)
cppephem_executable (obs2icrs obs2icrs.cpp)
cppephem_executable (obs2gal  obs2gal.cpp)

# Angular separation
cppephem_executable (angsep   angsep.cpp)

#------------------------------------------
# Executable Tests
#------------------------------------------

# Note that the following tests do not test if the outputs are correct. Tests
# of whether the classes are correct are handled elsewhere.

# Angular separation
add_test(NAME test_angsep COMMAND ${CMAKE_BINARY_DIR}/build/bin/angsep --xcoord1=0.0 --ycoord1=0.0 --xcoord2=0.0 --ycoord2=1.0)

# Date conversion tests
add_test(NAME test_cal2jd  COMMAND ${CMAKE_BINARY_DIR}/build/bin/cal2jd  20000101.5)
add_test(NAME test_cal2mjd COMMAND ${CMAKE_BINARY_DIR}/build/bin/cal2mjd 20000101.5)
add_test(NAME test_jd2cal  COMMAND ${CMAKE_BINARY_DIR}/build/bin/jd2cal  2451545.0)
add_test(NAME test_jd2mjd  COMMAND ${CMAKE_BINARY_DIR}/build/bin/jd2mjd  2451545.0)
add_test(NAME test_mjd2cal COMMAND ${CMAKE_BINARY_DIR}/build/bin/mjd2cal 51544.5)
add_test(NAME test_mjd2jd  COMMAND ${CMAKE_BINARY_DIR}/build/bin/mjd2jd  51544.5)

# Coordinate conversion tests
cppephem_test (test_cirs2gal  cirs2gal.cpp    --ra=83.633 --dec=22.0145 --juliandate=2451545.0)
cppephem_test (test_cirs2icrs cirs2icrs.cpp   --ra=83.633 --dec=22.0145 --juliandate=2451545.0)
cppephem_test (test_cirs2obs  cirs2obs.cpp    --ra=83.633 --dec=22.0145 --longitude=0.0 --latitude=0.0 --juliandate=2451545.0)
cppephem_test (test_icrs2cirs icrs2cirs.cpp   --ra=83.633 --dec=22.0145 --juliandate=2451545.0)
cppephem_test (test_icrs2gal  icrs2gal.cpp    --ra=83.633 --dec=22.0145)
cppephem_test (test_icrs2obs  icrs2obs.cpp    --ra=83.633 --dec=22.0145 --juliandate=2451545.0)
cppephem_test (test_gal2cirs  gal2cirs.cpp    --glon=184.553 --glat=-5.788 --juliandate=2451545.0)
cppephem_test (test_gal2icrs  gal2icrs.cpp    --glon=184.553 --glat=-5.788)
cppephem_test (test_gal2obs   gal2obs.cpp     --glon=184.553 --glat=-5.788 --longitude=0.0 --latitude=0.0  --juliandate=2451545.0)
cppephem_test (test_obs2cirs  obs2cirs.cpp    --longitude=0.0 --latitude=0.0 --azimuth=180 --zenith=20 --juliandate=2451545.0)
cppephem_test (test_obs2icrs  obs2icrs.cpp    --longitude=0.0 --latitude=0.0 --azimuth=180 --zenith=20 --juliandate=2451545.0)
cppephem_test (test_obs2gal   obs2gal.cpp     --longitude=0.0 --latitude=0.0 --azimuth=180 --zenith=20 --juliandate=2451545.0)

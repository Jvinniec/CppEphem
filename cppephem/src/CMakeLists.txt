
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
cppephem_executable (cirs2obs cirs2obs.cpp)
cppephem_executable (cirs2gal cirs2gal.cpp)
cppephem_executable (icrs2cirs icrs2cirs.cpp)
cppephem_executable (icrs2gal icrs2gal.cpp)
cppephem_executable (icrs2obs icrs2obs.cpp)
cppephem_executable (gal2cirs gal2cirs.cpp)
cppephem_executable (gal2obs  gal2obs.cpp)
cppephem_executable (obs2cirs obs2cirs.cpp)
cppephem_executable (obs2icrs obs2icrs.cpp)
cppephem_executable (obs2gal  obs2gal.cpp)

# Angular separation
cppephem_executable (angsep   angsep.cpp)

# Planet positions
cppephem_executable (planetephem planetephem.cpp)


# Note that the following tests do not test if the outputs are correct. Tests
# of whether the classes are correct are handled elsewhere.
cppephem_test (test_angsep      angsep.cpp      --xcoord1=0.0 --ycoord1=0.0 --xcoord2=0.0 --ycoord2=1.0)

# Date conversion tests
cppephem_test (test_cal2jd      cal2jd.cpp      20190201.5)
cppephem_test (test_cal2mjd     cal2mjd.cpp     20190201.5)
cppephem_test (test_jd2cal      jd2cal.cpp      2451545.0)
cppephem_test (test_jd2mjd      jd2mjd.cpp      2451545.0)
cppephem_test (test_mjd2cal     mjd2cal.cpp     51544.5)
cppephem_test (test_mjd2jd      mjd2jd.cpp      51544.5)

# Coordinate conversion tests
cppephem_test (test_cirs2gal    cirs2gal.cpp    --ra=83.633 --dec=22.0145)
cppephem_test (test_cirs2icrs   cirs2gal.cpp    --ra=83.633 --dec=22.0145)
cppephem_test (test_icrs2gal    icrs2gal.cpp    --ra=83.633 --dec=22.0145)

# planetephem test
cppephem_test (test_planetephem planetephem.cpp --planet=4 --longitude=0 --latitude=42 --elevation=0 --startJD=2451545.0)

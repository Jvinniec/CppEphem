
#------------------------------------------
# Define include directories
#------------------------------------------
#include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../include ${SOFA_DIRECTORY})

#------------------------------------------
# Executables
#------------------------------------------
# Date converters
# cppephem_executable (cal2jd  cal2jd.cpp)
# cppephem_executable (cal2mjd cal2mjd.cpp)
# cppephem_executable (jd2mjd  jd2mjd.cpp)
# cppephem_executable (jd2cal  jd2cal.cpp)
# cppephem_executable (mjd2jd  mjd2jd.cpp)
# cppephem_executable (mjd2cal mjd2cal.cpp)

# Coordinate converters
cppephem_executable (convcoord convcoord.cpp)

# Angular separation
# cppephem_executable (angsep   angsep.cpp)

# Planet positions
cppephem_executable (planetephem planetephem.cpp)

#------------------------------------------
# Executable Tests
#------------------------------------------

set(CORRFILES 
    --nutation=${CMAKE_BINARY_DIR}/build/share/cppephem/nutation.txt 
    --ttut1hist=${CMAKE_BINARY_DIR}/build/share/cppephem/ttut1_historic.txt 
    --ttut1pred=${CMAKE_BINARY_DIR}/build/share/cppephem/ttut1_predicted.txt)

# Note that the following tests do not test if the outputs are correct. Tests
# of whether the classes are correct are handled elsewhere.

# Angular separation
# cppephem_test (test_angsep      angsep.cpp      --xcoord1=0.0 --ycoord1=0.0 --xcoord2=0.0 --ycoord2=1.0)

# # Date conversion tests
# cppephem_test (test_cal2jd    cal2jd.cpp      20000101.5)
# cppephem_test (test_cal2mjd   cal2mjd.cpp     20000101.5)
# cppephem_test (test_jd2cal    jd2cal.cpp      2451545.0)
# cppephem_test (test_jd2mjd    jd2mjd.cpp      2451545.0)
# cppephem_test (test_mjd2cal   mjd2cal.cpp     51544.5)
# cppephem_test (test_mjd2jd    mjd2jd.cpp      51544.5)

# Coordinate conversion tests
add_test(NAME test_cirs2icrs2 COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --cirs=83.633,22.0145 --to=icrs --juliandate=2451545.0 > test_cirs2icrs2.txt)
add_test(NAME test_cirs2gal2  COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --cirs=83.633,22.0145 --to=galactic --juliandate=2451545.0)
add_test(NAME test_cirs2obs2  COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --cirs=83.633,22.0145 --to=observed --longitude=0.0 --latitude=0.0 --juliandate=2451545.0)
add_test(NAME test_cirs2ecl2  COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --cirs=83.633,22.0145 --to=ecliptic --juliandate=2451545.0)
add_test(NAME test_icrs2cirs2 COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --icrs=83.633,22.0145     --to=cirs --juliandate=2451545.0)
add_test(NAME test_icrs2gal2  COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --icrs=83.633,22.0145     --to=galactic)
add_test(NAME test_icrs2obs2  COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --icrs=83.633,22.0145     --to=observed --juliandate=2451545.0 --longitude=0.0 --latitude=0.0)
add_test(NAME test_icrs2ecl2  COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --icrs=83.633,22.0145     --to=ecliptic --juliandate=2451545.0)
add_test(NAME test_gal2cirs2  COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --galactic=184.553,-5.788 --to=cirs --juliandate=2451545.0)
add_test(NAME test_gal2icrs2  COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --galactic=184.553,-5.788 --to=icrs)
add_test(NAME test_gal2obs2   COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --galactic=184.553,-5.788 --to=observed --longitude=0.0 --latitude=0.0  --juliandate=2451545.0)
add_test(NAME test_gal2ecl2   COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --galactic=184.553,-5.788 --to=ecliptic --juliandate=2451545.0)
add_test(NAME test_obs2cirs2  COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --observed=180,20         --to=cirs --longitude=0.0 --latitude=0.0 --juliandate=2451545.0)
add_test(NAME test_obs2icrs2  COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --observed=180,20         --to=icrs --longitude=0.0 --latitude=0.0 --juliandate=2451545.0)
add_test(NAME test_obs2gal2   COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --observed=180,20         --to=galactic --longitude=0.0 --latitude=0.0 --juliandate=2451545.0)
add_test(NAME test_obs2ecl2   COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --observed=180,20         --to=ecliptic --longitude=0.0 --latitude=0.0 --juliandate=2451545.0)
add_test(NAME test_ecl2cirs2  COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --ecliptic=184.553,-5.788 --to=cirs --juliandate=2451545.0)
add_test(NAME test_ecl2icrs2  COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --ecliptic=184.553,-5.788 --to=icrs --juliandate=2451545.0)
add_test(NAME test_ecl2gal2   COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --ecliptic=184.553,-5.788 --to=galactic --juliandate=2451545.0)
add_test(NAME test_ecl2obs2   COMMAND ${CMAKE_BINARY_DIR}/build/bin/convcoord ${CORRFILES} --ecliptic=184.553,-5.788 --to=observed --longitude=0.0 --latitude=0.0  --juliandate=2451545.0)

# planetephem test
# cppephem_test (test_planetephem planetephem.cpp --planet=4 --longitude=0 --latitude=42 --elevation=0 --startJD=2451545.0)
add_test(NAME test_planetephem COMMAND ${CMAKE_BINARY_DIR}/build/bin/planetephem ${CORRFILES} --planet=4 --longitude=0 --latitude=42 --elevation=0 --startJD=2451545.0)


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
# cppephem_executable (planetephem planetephem.cpp)

#------------------------------------------
# Executable Tests
#------------------------------------------

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
cppephem_test (test_cirs2gal2  convcoord.cpp --cirs=83.633,22.0145 --to=galactic --juliandate=2451545.0)
cppephem_test (test_cirs2icrs2 convcoord.cpp --cirs=83.633,22.0145 --to=icrs --juliandate=2451545.0)
cppephem_test (test_cirs2obs2  convcoord.cpp --cirs=83.633,22.0145 --to=observed --longitude=0.0 --latitude=0.0 --juliandate=2451545.0)
cppephem_test (test_cirs2ecl2  convcoord.cpp --cirs=83.633,22.0145 --to=ecliptic --juliandate=2451545.0)
cppephem_test (test_icrs2cirs2 convcoord.cpp --icrs=83.633,22.0145 --to=cirs --juliandate=2451545.0)
cppephem_test (test_icrs2gal2  convcoord.cpp --icrs=83.633,22.0145 --to=galactic)
cppephem_test (test_icrs2obs2  convcoord.cpp --icrs=83.633,22.0145 --to=obs --juliandate=2451545.0 --longitude=0.0 --latitude=0.0)
cppephem_test (test_icrs2ecl2  convcoord.cpp --icrs=83.633,22.0145 --to=ecliptic --juliandate=2451545.0)
cppephem_test (test_gal2cirs2  convcoord.cpp --galactic=184.553,-5.788 --to=cirs --juliandate=2451545.0)
cppephem_test (test_gal2icrs2  convcoord.cpp --galactic=184.553,-5.788 --to=icrs)
cppephem_test (test_gal2obs2   convcoord.cpp --galactic=184.553,-5.788 --to=observed --longitude=0.0 --latitude=0.0  --juliandate=2451545.0)
cppephem_test (test_gal2ecl2   convcoord.cpp --galactic=184.553,-5.788 --to=ecliptic --juliandate=2451545.0)
cppephem_test (test_obs2cirs2  convcoord.cpp --observed=180,20         --to=cirs --longitude=0.0 --latitude=0.0 --juliandate=2451545.0)
cppephem_test (test_obs2icrs2  convcoord.cpp --observed=180,20         --to=icrs --longitude=0.0 --latitude=0.0 --juliandate=2451545.0)
cppephem_test (test_obs2gal2   convcoord.cpp --observed=180,20         --to=galactic --longitude=0.0 --latitude=0.0 --juliandate=2451545.0)
cppephem_test (test_obs2ecl2   convcoord.cpp --observed=180,20         --to=ecliptic --longitude=0.0 --latitude=0.0 --juliandate=2451545.0)
cppephem_test (test_ecl2cirs2  convcoord.cpp --ecliptic=184.553,-5.788 --to=cirs --juliandate=2451545.0)
cppephem_test (test_ecl2icrs2  convcoord.cpp --ecliptic=184.553,-5.788 --to=icrs --juliandate=2451545.0)
cppephem_test (test_ecl2gal2   convcoord.cpp --ecliptic=184.553,-5.788 --to=galactic --juliandate=2451545.0)
cppephem_test (test_ecl2obs2   convcoord.cpp --ecliptic=184.553,-5.788 --to=observed --longitude=0.0 --latitude=0.0  --juliandate=2451545.0)

# planetephem test
# cppephem_test (test_planetephem planetephem.cpp --planet=4 --longitude=0 --latitude=42 --elevation=0 --startJD=2451545.0)

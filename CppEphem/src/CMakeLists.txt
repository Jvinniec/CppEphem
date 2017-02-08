
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../include ${SOFA_DIRECTORY})

add_library (cppephem SHARED
        CENamespace.cpp
        CEBody.cpp
        CECoordinates.cpp
        CEDate.cpp
        CEObservation.cpp
        CEObserver.cpp
        CEPlanet.cpp
        CERunningDate.cpp
        CETime.cpp
        )

#--------------------------------------------------
# Executables
#--------------------------------------------------
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
cppephem_executable (gal2cirs gal2cirs.cpp)
cppephem_executable (gal2obs  gal2obs.cpp)
cppephem_executable (obs2cirs obs2cirs.cpp)
cppephem_executable (obs2icrs obs2icrs.cpp)
cppephem_executable (obs2gal  obs2gal.cpp)

# Create the cppephem shared library


target_link_libraries(cppephem sofa_c)


#------------------------------------------
# Define include directories
#------------------------------------------
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../include ${SOFA_DIRECTORY})

#------------------------------------------
# Define the source and header files needed
# by the CppEphem library
#------------------------------------------
set (cppephem_SOURCES
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

set (cppephem_HEADERS
    ../include/CppEphem.h
    ../include/CENamespace.h
    ../include/CEBody.h
    ../include/CECoordinates.h
    ../include/CEDate.h
    ../include/CEObservation.h
    ../include/CEObserver.h
    ../include/CEPlanet.h
    ../include/CERunningDate.h
    ../include/CETime.h
    )

#------------------------------------------
# Create both a static and shared version of the library
#------------------------------------------
add_library (cppephem SHARED ${cppephem_SOURCES} ${cppephem_HEADERS})
add_library (cppephem_static STATIC ${cppephem_SOURCES} ${cppephem_HEADERS})

# Make sure the static version has the same name
set_target_properties(cppephem_static PROPERTIES OUTPUT_NAME cppephem)

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
cppephem_executable (icrs2cirs icrs2cirs.cpp)
cppephem_executable (icrs2gal icrs2gal.cpp)
cppephem_executable (icrs2obs icrs2obs.cpp)
cppephem_executable (cirs2obs cirs2obs.cpp)
cppephem_executable (cirs2gal cirs2gal.cpp)
cppephem_executable (gal2cirs gal2cirs.cpp)
cppephem_executable (gal2obs  gal2obs.cpp)
cppephem_executable (obs2cirs obs2cirs.cpp)
cppephem_executable (obs2icrs obs2icrs.cpp)
cppephem_executable (obs2gal  obs2gal.cpp)

# Angular separation
cppephem_executable (angsep   angsep.cpp)

# Planet positions
cppephem_executable (planetephem planetephem.cpp)

target_link_libraries(cppephem sofa_c)

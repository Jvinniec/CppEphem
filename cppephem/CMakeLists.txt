
#------------------------------------------
# Define include directories
#------------------------------------------
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include 
                     ${CMAKE_SOURCE_DIR}/test/include 
                     ${SOFA_DIRECTORY})

#------------------------------------------
# Define the source and header files needed
# by the CppEphem library
#------------------------------------------
set (cppephem_SOURCES
    src/CENamespace.cpp
    src/CEBody.cpp
    src/CECoordinates.cpp
    src/CEDate.cpp
    src/CEException.cpp
    src/CEObservation.cpp
    src/CEObserver.cpp
    src/CEPlanet.cpp
    src/CERunningDate.cpp
    src/CETime.cpp
    )

set (cppephem_HEADERS
    include/CppEphem.h
    include/CENamespace.h
    include/CEBody.h
    include/CECoordinates.h
    include/CEDate.h
    include/CEException.h
    include/CEObservation.h
    include/CEObserver.h
    include/CEPlanet.h
    include/CERunningDate.h
    include/CETime.h
    )

#------------------------------------------
# Create both a static and shared version of the library
#------------------------------------------
add_library (cppephem SHARED ${cppephem_SOURCES} ${cppephem_HEADERS})
add_library (cppephem_static STATIC ${cppephem_SOURCES} ${cppephem_HEADERS})

# Make sure the static version has the same name
set_target_properties(cppephem_static PROPERTIES OUTPUT_NAME cppephem)

target_link_libraries(cppephem sofa_c)

# Make sure all of the executables are compiled
add_subdirectory (src)

set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CPPEPHEM_TEST_CXX_FLAGS}")

# Define the tests
cppephem_test(test_CECoordinates test/test_CECoordinates.cpp test/test_CECoordinates.h ${cppephem_SOURCES} ${cppephem_HEADERS})
cppephem_test(test_CEDate test/test_CEDate.cpp test/test_CEDate.h ${cppephem_SOURCES} ${cppephem_HEADERS})

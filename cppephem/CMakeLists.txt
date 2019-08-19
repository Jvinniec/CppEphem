
#------------------------------------------
# Define include directories
#------------------------------------------
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include
                     ${CMAKE_CURRENT_SOURCE_DIR}/support
                     ${CMAKE_SOURCE_DIR}/test/include
                     ${SOFA_DIRECTORY})

#------------------------------------------
# Define the source and header files needed
# by the CppEphem library
#------------------------------------------
set (cppephem_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CENamespace.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CEAngle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CEBody.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CECoordinates.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CECorrections.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CEDate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CEException.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CEObservation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CEObserver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CEPlanet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CERunningDate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CESkyCoord.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CETime.cpp
    )

set (cppephem_HEADERS
    include/CppEphem.h
    include/CENamespace.h
    include/CEAngle.h
    include/CEBody.h
    include/CECoordinates.h
    include/CECorrections.h
    include/CEDate.h
    include/CEException.h
    include/CEObservation.h
    include/CEObserver.h
    include/CEPlanet.h
    include/CERunningDate.h
    include/CESkyCoord.h
    include/CETime.h
    )

#------------------------------------------
# Create both a static and shared version of the library
#------------------------------------------
add_library (cppephem SHARED ${cppephem_SOURCES} ${cppephem_HEADERS})
add_library (cppephem_static STATIC ${cppephem_SOURCES} ${cppephem_HEADERS})
target_compile_definitions(cppephem PUBLIC -DCECORRFILEPATH=\"${CPPEPHEM_CORRFILEPATH}\")
target_compile_definitions(cppephem_static PUBLIC -DCECORRFILEPATH=\"${CPPEPHEM_CORRFILEPATH}\")

# Make sure the static version has the same name
set_target_properties(cppephem_static PROPERTIES OUTPUT_NAME cppephem)

# Link against the sofa and curl (optional) libraries
target_link_libraries(cppephem sofa_c)
if (NOT nocurl)
    target_link_libraries(cppephem curl)
endif()

#------------------------------------------
# Create both a static and shared version of the library
#------------------------------------------

set (testcppephem_HEADERS ${cppephem_HEADERS} PARENT_SCOPE)
set (testcppephem_SOURCES ${cppephem_SOURCES} PARENT_SCOPE)

# Make sure all of the executables are compiled
add_subdirectory (test)
add_subdirectory (src)
add_subdirectory (exec)
add_subdirectory (swig)

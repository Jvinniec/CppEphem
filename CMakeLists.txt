cmake_minimum_required (VERSION 3.0)
project (CppEphem)
set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# Add Version number
set (CppEphem_VERSION_MAJOR 1)
set (CppEphem_VERSION_MINOR 2)
set (CppEphem_VERSION_PATCH 0)
set (cppephem_version "${CppEphem_VERSION_MAJOR}.${CppEphem_VERSION_MINOR}.${CppEphem_VERSION_PATCH}")
message ("\nCOMPILING CPPEPHEM VERSION ${cppephem_version}\n")

# Set some flags necessary for the MAC OS builds
if (CMAKE_SYSTEM_NAME MATCHES Darwin)
   set(CMAKE_MACOSX_RPATH 1)
endif()

# If the user supplied a prefix, set it
if (prefix)
   set (CMAKE_INSTALL_PREFIX ${prefix})
endif()

# User defined corrections file location
set (CPPEPHEM_CORRFILEPATH "${CMAKE_INSTALL_PREFIX}/share/cppephem/corrections.txt")
if (CORRFILEPATH)
    set (CPPEPHEM_CORRFILEPATH ${CORRFILEPATH})
endif()

# Defined when user does not want to download the corrections file
if (nocurl)
    message(STATUS "NO CURL WAS SPECIFIED")
    add_compile_definitions(NOCURL)
endif()

# Define where we want to put things
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
set (CPPEPHEM_SHARE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/share/cppephem)
set (HEADER_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build/include)

# Define some of the default compiler flags
#set (CMAKE_CXX_FLAGS "-std=c++11 ${CMake_CXX_FLAGS}")

# Define the sofa directory
set (SOFA_DIRECTORY ${CMAKE_SOURCE_DIR}/sofa/20170420/c/src)
if (sofa)
   set (SOFA_DIRECTORY ${sofa})
endif()
link_directories (${SOFA_DIRECTORY})

# Define the default compile flags
set (CPPEPHEM_TEST_CXX_FLAGS "")
set (CODECOV "none")
if (coverage)
    include(CodeCoverage.cmake)
    message (STATUS "Testing flags set to ${CPPEPHEM_TEST_CXX_FLAGS} (coverage = ${CODECOV})")
endif()

set (CPPEPHEM_CXX_FLAGS "${CPPEPHEM_CXX_FLAGS} -std=c++11 -Wall")
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CPPEPHEM_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CPPEPHEM_TEST_CXX_FLAGS}")


#-------------------------------------------

# Define a macro for cppephem executable compilation
# so that I dont have to specify that it depends on
# the cppephem library EVERY SINGLE TIME!
function (cppephem_executable _name _sources)
    # invoke built-in add_executable
    add_executable(${_name} ${_sources})
    target_link_libraries(${_name} cppephem)
    target_compile_definitions(${_name} PUBLIC -DCPPEPHEM_VERSION=\"${cppephem_version}\")
endfunction(cppephem_executable)

#-------------------------------------------

# Define a macro for cppephem test compilation
# so that we dont have to specify multiple lines
# EVERY SINGLE TIME!
function (cppephem_test _name _source)
    add_executable(${_name} ${_source} ${cppephem_SOURCES})
    target_compile_definitions(${_name} PUBLIC -DCECORRFILEPATH=\"${CMAKE_BINARY_DIR}/build/share/cppephem/corrections.txt\"
                                               -DCPPEPHEM_VERSION=\"${cppephem_version}-TEST\"
                                               -DNOCURL)
    target_link_libraries(${_name} testcesuite sofa_c)
    add_test(NAME ${_name} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_name} ${ARGN})
endfunction (cppephem_test)

#-------------------------------------------

# Define the sub-directories for building in
add_subdirectory (${SOFA_DIRECTORY})
add_subdirectory (share)
enable_testing ()
add_subdirectory (cppephem)
add_subdirectory (test)


#-------------------------------------------

# Define installation directories
install(DIRECTORY ${CMAKE_BINARY_DIR}include/
    USE_SOURCE_PERMISSIONS
	DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    FILES_MATCHING PATTERN "C*.h")

install(DIRECTORY ${SOFA_DIRECTORY}/
	USE_SOURCE_PERMISSIONS
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    FILES_MATCHING PATTERN "sofa*h")

install(DIRECTORY ${CPPEPHEM_SHARE_OUTPUT_DIRECTORY}
    USE_SOURCE_PERMISSIONS
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share
    PATTERN "corrections.txt" EXCLUDE)

install(DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
	USE_SOURCE_PERMISSIONS
	DESTINATION ${CMAKE_INSTALL_PREFIX}
    PATTERN "libtest*" EXCLUDE)

install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	USE_SOURCE_PERMISSIONS
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    PATTERN "test*" EXCLUDE)

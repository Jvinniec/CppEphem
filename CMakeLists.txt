cmake_minimum_required (VERSION 2.6)
project (CppEphem)
set(CMAKE_BUILD_TYPE Release)

# Add Version number
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# Define where we want to put things
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(HEADER_OUTPUT_PATH ${CMAKE_BINARY_DIR}/include)

# If the user supplied a prefix, set it
if (prefix)
   set (CMAKE_INSTALL_PREFIX prefix)
endif()

# Define some of the default compiler flags
set (CMAKE_CXX_FLAGS "-std=c++11 ${CMake_CXX_FLAGS}")

# Define the sofa directory
set (SOFA_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sofa/20160503/c/src)
link_directories (${SOFA_DIRECTORY} /Users/joshcardenzana/gitRepos/CppEphem/cmake_test/lib)

# Define the default compile flags
set (CPPEPHEM_CXX_FLAGS "-std=c++11")
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CPPEPHEM_CXX_FLAGS}")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -c")


# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#  "${PROJECT_SOURCE_DIR}/CppEphemConfig.h.in"
#  "${PROJECT_BINARY_DIR}/CppEphemConfig.h"
#  )
 
#-------------------------------------------

# Define a macro for cppephem executable compilation
# so that I dont have to specify that it depends on
# the cppephem library EVERY SINGLE TIME!
function (cppephem_executable _name _sources)
    # invoke built-in add_executable
    add_executable(${_name} ${_sources})
    target_link_libraries(${_name} cppephem)
endfunction(cppephem_executable)

#-------------------------------------------

# Define the sub-directories for building in
add_subdirectory (${SOFA_DIRECTORY})
add_subdirectory (CppEphem/src)

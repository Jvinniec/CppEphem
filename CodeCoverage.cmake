
# Make sure that we can find a code coverage tool
find_program( GCOV_PATH gcov )
find_program( LLVMCOV_PATH  NAMES llvm-cov)

# If no code coverage tool was found, throw an error
if(NOT GCOV_PATH AND NOT LLVMCOV_PATH)
    message(FATAL_ERROR "No coverage tool found! Aborting...")
endif()

# Check if we're compiling on OSX
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3)
        message(FATAL_ERROR "Clang version must be 3.0.0 or greater! Aborting...")
    endif()
elseif(NOT CMAKE_COMPILER_IS_GNUCXX)
    message(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
endif()

# Set the compile and linker parameters
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CPPEPHEM_CXX_FLAGS "-g -O0 --coverage ${CPPEPHEM_CXX_FLAGS}")
    link_libraries(gcov)
else()
    set(CPPEPHEM_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage ${CPPEPHEM_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()


# Make sure that we can find a code coverage tool
find_program ( GCOV_PATH gcov )
find_program ( LLVMCOV_PATH  NAMES llvm-cov)

# If no code coverage tool was found, throw an error
if (NOT GCOV_PATH AND NOT LLVMCOV_PATH)
    message (FATAL_ERROR "No coverage tool found! Aborting...")
endif ()

# Check if we're compiling on OSX
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    if ("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3)
        message (FATAL_ERROR "Clang version must be 3.0.0 or greater! Aborting...")
    else ()
        message (STATUS "Compiling with Clang... will use llvm-cov for coverage")
        set (CODECOV "llvm")
    endif ()
elseif (NOT CMAKE_COMPILER_IS_GNUCXX)
    message (FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
else()
    message (STATUS "Compiling with gcc... will use gcov for coverage")
    set (CODECOV "gcov")
endif ()

# Set the compile and linker parameters
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set (CPPEPHEM_TEST_CXX_FLAGS "--coverage ${CPPEPHEM_TEST_CXX_FLAGS}")
    link_libraries (gcov)
else ()
    set (CPPEPHEM_TEST_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping ${CPPEPHEM_TEST_CXX_FLAGS}")
endif ()

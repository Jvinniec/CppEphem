
#------------------------------------------
# Define include directories
#------------------------------------------
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include
                     ${CMAKE_SOURCE_DIR}/cppephem/include
                     ${CMAKE_SOURCE_DIR}/cppephem/support
                     ${SOFA_DIRECTORY})

#------------------------------------------
# Define the source and header files needed
# by the testing library
#------------------------------------------
set (test_ce_SOURCES
    src/CETestSuite.cpp
    )
    
set (test_ce_HEADERS
     include/CETestSuite.h
    )

#------------------------------------------
# Add test library
#------------------------------------------
add_library (testcesuite SHARED ${test_ce_SOURCES} ${test_ce_HEADERS})
target_link_libraries(testcesuite)

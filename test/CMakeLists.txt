
#------------------------------------------
# Define include directories
#------------------------------------------
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include 
                     ${SOFA_DIRECTORY}
                     ${CMAKE_SOURCE_DIR}/include
                     )

#------------------------------------------
# Define the source and header files needed
# by the CppEphem library
#------------------------------------------
set (test_ce_SOURCES
    src/CETestSuite.cpp
    src/test_CEDate.cpp
    )
    
set (test_ce_HEADERS
    include/CETestSuite.h
    include/test_CEDate.h
    )

#------------------------------------------
# Add test library
#------------------------------------------
add_library (testcppephem SHARED ${test_ce_SOURCES} ${test_ce_HEADERS})

#------------------------------------------
# Test Executables
#------------------------------------------
#cppephem_executable(test_cppephem src/test_cppephem.cpp)
add_executable(test_cppephem src/test_cppephem.cpp)
target_link_libraries(test_cppephem testcppephem)


target_link_libraries(testcppephem
                      cppephem
                      )
                    
add_test(NAME test_ce COMMAND test_cppephem)

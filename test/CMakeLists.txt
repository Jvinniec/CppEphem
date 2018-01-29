

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include 
                     ${SOFA_DIRECTORY}
                     ${CMAKE_SOURCE_DIR}/include
                     )

set (test_ce_SOURCES
    src/CETestSuite.cpp
    src/test_CEDate.cpp
    )
    
set (test_ce_HEADERS
    include/CETestSuite.h
    include/test_CEDate.h
    )

add_library (testcppephem ${test_ce_SOURCES} ${test_ce_HEADERS})

cppephem_executable(test_cppephem src/test_cppephem.cpp)

target_link_libraries(test_cppephem
                      cppephem
                      testcppephem
                      )
                    
add_test(NAME test_ce COMMAND test_cppephem)

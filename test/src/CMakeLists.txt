

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../include 
                     ${SOFA_DIRECTORY}
                     ${}
                     )

set (test_ce_SOURCES
     CETestSuite.cpp)

target_link_libraries(cppephem sofa_c)
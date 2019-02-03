
#------------------------------------------
# Define include directories
#------------------------------------------
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

#------------------------------------------
# Define the source and header files needed
# by the CppEphem library
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
add_library (testcppephem SHARED ${test_ce_SOURCES} ${test_ce_HEADERS})
#target_link_libraries(testcppephem sofa_c)

#------------------------------------------
# Test Executables
#------------------------------------------
#add_executable(test_cppephem src/test_cppephem.cpp ${test_ce_SOURCES} ${test_ce_HEADERS} ${cppephem_SOURCES})
#target_link_libraries(test_cppephem test_cppephem)


#target_link_libraries(testcppephem
#                      cppephem
#                      )
#set (ENV{LLVM_PROFILE_FILE} "test_cppephem.profraw")
#add_test(NAME test_ce COMMAND test_cppephem)

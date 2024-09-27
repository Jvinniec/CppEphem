#**************************************************************************
#  test/CMakeLists.txt: CppEphem                                          *
# ----------------------------------------------------------------------- *
#  Copyright © 2019 JCardenzana                                           *
# ----------------------------------------------------------------------- *
#                                                                         *
#  This program is free software: you can redistribute it and/or modify   *
#  it under the terms of the GNU General Public License as published by   *
#  the Free Software Foundation, either version 3 of the License, or      *
#  (at your option) any later version.                                    *
#                                                                         *
#  This program is distributed in the hope that it will be useful,        *
#  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
#  GNU General Public License for more details.                           *
#                                                                         *
#  You should have received a copy of the GNU General Public License      *
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
#                                                                         *
#**************************************************************************

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

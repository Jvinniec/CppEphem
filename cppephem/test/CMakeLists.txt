#**************************************************************************
#  cppephem/test/CMakeLists.txt: CppEphem                                 *
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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include
                    ${CMAKE_SOURCE_DIR}/test/include
                    )

# Define the tests
cppephem_test(test_CEAngle       test_CEAngle.cpp)
cppephem_test(test_CEBody        test_CEBody.cpp)
cppephem_test(test_CECoordinates test_CECoordinates.cpp)
cppephem_test(test_CEDate        test_CEDate.cpp)
cppephem_test(test_CEException   test_CEException.cpp)
cppephem_test(test_CENamespace   test_CENamespace.cpp)
cppephem_test(test_CEObservation test_CEObservation.cpp)
cppephem_test(test_CEObserver    test_CEObserver.cpp)
cppephem_test(test_CEPlanet      test_CEPlanet.cpp)
cppephem_test(test_CERunningDate test_CERunningDate.cpp)
cppephem_test(test_CESkyCoord    test_CESkyCoord.cpp)
cppephem_test(test_CETime        test_CETime.cpp)

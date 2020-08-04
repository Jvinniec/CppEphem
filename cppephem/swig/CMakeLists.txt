#**************************************************************************
#  cppephem/swig/CMakeLists.txt: CppEphem                                 *
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

if (python-bindings OR javascript-bindings)

    FIND_PACKAGE(SWIG REQUIRED)
    INCLUDE(${SWIG_USE_FILE})

    # Set the include directories
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
                        ${CMAKE_SOURCE_DIR}/cppephem/include)

    SET(swig_files
        cppephem.i
        CEAngle.i
        CEBody.i
        CECorrections.i
        CEDate.i
        CEException.i
        CENamespace.i
        CEObservation.i
        CEObserver.i
        CERunningDate.i
        CESkyCoord.i
        CETime.i)

    add_custom_target(swig-target ALL DEPENDS ${swig_files})

    foreach(item IN LISTS swig_files)
        add_custom_command(
            TARGET swig-target PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${item}" "${CMAKE_CURRENT_BINARY_DIR}/${item}"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
        )
    endforeach()

    # Python wrapper
    if (python-bindings)
        add_subdirectory (pyext)
    endif()

endif()
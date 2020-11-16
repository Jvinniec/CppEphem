#**************************************************************************
#  cppephem/swig/pyext/CMakeLists.txt: CppEphem                           *
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

######################################################
# This builds the Python wrappers for the code
######################################################

FIND_PACKAGE(PythonInterp)
if(PYTHONINTERP_FOUND)
  FIND_PACKAGE(PythonLibs)
endif()

# Ensure SWIG is installed
FIND_PACKAGE(SWIG 4.0 COMPONENTS python)
if(SWIG_FOUND)
  MESSAGE("SWIG found: ${SWIG_EXECUTABLE} ${SWIG_VERSION}")
endif()

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

# Pre-configure the python files
configure_file(cppephem/__init__.py.in 
               ${CMAKE_CURRENT_BINARY_DIR}/cppephem/__init__.py)
configure_file(setup.py.in 
               ${CMAKE_CURRENT_BINARY_DIR}/setup.py)

set_property(SOURCE ../cppephem.i PROPERTY CPLUSPLUS ON)

# Add special doxygen formatting if SWIG version supports it
if (${SWIG_VERSION} GREATER_EQUAL 4)
    set_property(SOURCE ../cppephem.i PROPERTY SWIG_FLAGS -doxygen)# -debug-doxygen-translator)
endif()

SWIG_ADD_LIBRARY(cppephem_pywrap
                 LANGUAGE    python
                 OUTPUT_DIR  cppephem
                 SOURCES     ../cppephem.i)

SWIG_LINK_LIBRARIES(cppephem_pywrap cppephem ${PYTHON_LIBRARIES})

add_custom_command(TARGET cppephem_pywrap 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/_cppephem_pywrap.so" "${CMAKE_CURRENT_BINARY_DIR}/cppephem/_cppephem_pywrap.so")

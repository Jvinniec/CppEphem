/***************************************************************************
 *  CEBase.i: CppEphem                                                     *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2019 JCardenzana                                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/

/**
 * @file CEBase.i
 * @brief SWIG file for CEBase class
 * @author JCardenzana
 */
%{
/* Put headers and other declarations here that are needed for compilation */
// CppEphem HEADERS
#include "CEBase.h"
%}


/***********************************************************************//**
 * @class CEBase
 *
 * @brief CEBase class SWIG interface definition
 ***************************************************************************/
class CEBase {
public:
    // Constructors
    CEBase();
    virtual ~CEBase();

    virtual const std::string ClassName(void) const = 0;
    virtual const std::string describe(void) const;

};


/***********************************************************************//**
 * Extend CEBase class
 ***************************************************************************/
%extend CEBase {

    %pythoncode {
        def __str__(self):
            """
            Returns a description of this object
            """
            return (self.describe())
    }

};
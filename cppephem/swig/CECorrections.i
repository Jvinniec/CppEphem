/***************************************************************************
 *  CECorrections.i: CppEphem                                              *
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
 * @file CECorrections.i
 * @brief SWIG file for CECorrections class
 * @author JCardenzana
 */
%{
/* Put headers and other declarations here that are needed for compilation */
// CppEphem HEADERS
#include "CECorrections.h"
%}


/***********************************************************************//**
 * @class CECorrections
 *
 * @brief CECorrections class SWIG interface definition
 ***************************************************************************/
class CECorrections {
public:
    CECorrections();
    CECorrections(const CECorrections& other);
    virtual ~CECorrections() {}

    double      dut1(const double& mjd) const;
    double      xpolar(const double& mjd) const;
    double      ypolar(const double& mjd) const;
    double      deps(const double& mjd) const;
    double      dpsi(const double& mjd) const;
    double      ttut1(const double& mjd) const;
    std::string NutationFile(void) const;
    std::string TtUt1HistFile(void) const;
    std::string TtUt1PredFile(void) const;
    void        SetNutationFile(const std::string& filename);
    void        SetTtUt1HistFile(const std::string& filename);
    void        SetTtUt1PredFile(const std::string& filename);
    void        SetInterp(bool set_interp);

    // Necessary methods
    const std::string ClassName(void) const;
    const std::string describe(void) const;
};

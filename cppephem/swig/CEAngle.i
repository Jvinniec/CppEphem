/***************************************************************************
 *  CEAngle.i: CppEphem                                                    *
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
 * @file CEAngle.i
 * @brief SWIG file for CEAngle class
 * @author JCardenzana
 */
%{
/* Put headers and other declarations here that are needed for compilation */
// CppEphem HEADERS
#include "CEAngle.h"
%}

/***********************************************************************//**
 * @enum CEAngleType
 *
 * @brief CEAngleType enum class interface definition
 ***************************************************************************/
enum class CEAngleType : unsigned int {
    DEGREES=0,      ///< Degrees
    RADIANS=1,      ///< Radians
    HMS=2,          ///< Hours, minutes, seconds
    DMS=3           ///< Degrees, arcmin, arcsec
};

/***********************************************************************//**
 * @class CEAngle
 *
 * @brief CEAngle class interface definition
 ***************************************************************************/
class CEAngle {
public:
    // Constructors
    CEAngle();
    CEAngle(const double& angle);
    explicit CEAngle(const CEAngle& other);
    virtual ~CEAngle();

    static CEAngle Hms(const char* angle_str, 
                      const char& delim=0);
    static CEAngle Hms(const std::vector<double>& angle_vec);
    static CEAngle Dms(const char* angle_str, 
                      const char& delim=0);
    static CEAngle Dms(const std::vector<double>& angle_vec);

    // Create from an angle value
    static CEAngle Deg(const double& angle);
    static CEAngle Rad(const double& angle);

    // Methods to return a formatted value for the angle
    std::string         HmsStr(const char& delim=':') const;
    std::vector<double> HmsVect(void) const;
    std::string         DmsStr(const char& delim=':') const;
    std::vector<double> DmsVect(void) const;
    double              Deg(void) const;
    double              Rad(void) const;

    // Generic methods for setting the angle
    void SetAngle(const double&      angle,
                  const CEAngleType& angle_type=CEAngleType::RADIANS);
    void SetAngle(const char*        angle_str,
                  const CEAngleType& angle_type,
                  const char&        delim=0);
    void SetAngle(const std::vector<double>& angle_vec,
                  const CEAngleType&         angle_type);

    // Necessary methods
    const std::string ClassName(void) const;
    const std::string describe(void) const;

};


/***********************************************************************//**
 * @brief CEAngle class extension
 ***************************************************************************/
%extend CEAngle {

    // Add python specific functions
    %pythoncode {
        def __float__(self):
            """
            Return the angle in radians
            """
            return self.Rad()
    }
    
};
/***************************************************************************
 *  CEAngle.h: CppEphem                                                    *
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

#ifndef CEAngle_h
#define CEAngle_h

#include <string> 
#include <vector>

#include "CENamespace.h"

/**********************************************************************//**
 * Angle type enum
 *************************************************************************/
enum class CEAngleType : unsigned int {
    DEGREES=0,      ///< Degrees
    RADIANS=1,      ///< Radians
    HMS=2,          ///< Hours, minutes, seconds
    DMS=3           ///< Degrees, arcmin, arcsec
};

class CEAngle {
public:
    // Constructors
    CEAngle();
    CEAngle(const double& angle);
    explicit CEAngle(const CEAngle& other);
    virtual ~CEAngle();

    // Copy-assignment operators
    CEAngle& operator=(const CEAngle& other);
    CEAngle& operator=(const double&  other);

    // Make sure the angle object can be interpreted as a double
    operator double();
    operator double() const;

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

private:

    // Methods
    void copy_members(const CEAngle& other);
    void init_members(void);
    void free_members(void);

    void SetAngle_HmsVect(const std::vector<double>& angle);
    void SetAngle_DmsVect(const std::vector<double>& angle);

    // Variables
    mutable double angle_;      ///< Angle stored in radians

};

#endif /* CEAngle_h */
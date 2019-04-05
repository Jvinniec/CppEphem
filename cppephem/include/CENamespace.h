/***************************************************************************
 *  CENamespace.h: CppEphem                                                *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016-2019 JCardenzana                                      *
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

#ifndef CENamespace_h
#define CENamespace_h

#include <cmath>
#include <map>
#include <sofa.h>
#include <memory>
#include "CECorrections.h"

#define MINMJDFORDUT1 43144.0
#define MAXMJDFORDUT1 57448.0

enum class CEAngleType{DEGREES, RADIANS} ;

// Create a namespace with some useful stuff
namespace CppEphem {
    // Define an angle type so that we can differentiate between degrees or radians

    // Approximate temperature at sea-level in Kelvin
    inline double SeaLevelTemp_K() {return 288.2 ;}
    inline double SeaLevelTemp_C() {return SeaLevelTemp_K()-273.15 ;}
    inline double SeaLevelTemp_F() {return ((9.0/5.0)*SeaLevelTemp_C())+32.0 ;}
    
    // Some constants taken from the sofa dictionary, but with some more discriptive names
    inline double julian_date_J2000() {return DJ00 ;}           ///< Julian Date corresponding to J2000
    inline double c() {return DC * DAU / DAYSEC ;}              ///< speed of light (meters/second)
    inline double c_au_per_day() {return DC ;}                  ///< speed of light (astronomical units)/day
    inline double m_per_au() {return DAU ;}                     ///< meters per astronomical unit
    inline double sec_per_day() {return DAYSEC;}                ///< Seconds per day
    
    static CECorrections corrections;

    /*********************************************
     * Return the 'dut1' value which represents 
     * 'UT1-UTC' for a given MJD or it's error
     *********************************************/
    double dut1(const double& mjd) ;
    double dut1Error(const double& mjd=51544.5) ;
    double dut1Calc(const double& mjd) ;
    
    /*********************************************
     * Polar Motion methods
     * For the moment, 0.0 should be sufficient. This
     * assumption may need to be revisited
     *********************************************/
    double xp(const double& mjd);
    double yp(const double& mjd);
    
    /** Method for estimating altitude (in meters) from atmospheric pressure (in hPa) */
    inline double EstimateAltitude_m(double pressure_hPa)
        {return -29.3 * SeaLevelTemp_K() * std::log(pressure_hPa/1013.25) ;}
    /** Method for estimating atmospheric pressure (in hPa) from altitude (in meters) */
    inline double EstimatePressure_hPa(double elevation_m)
        {return 1013.25 * std::exp(-elevation_m / (29.3*SeaLevelTemp_K() )) ;}
}

#endif /* CENamespace_h */

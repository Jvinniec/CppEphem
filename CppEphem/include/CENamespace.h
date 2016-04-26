//
//  CENamespace.h
//  CppEphem
//
//  Created by Josh Cardenzana on 4/6/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#ifndef CENamespace_h
#define CENamespace_h

#include <cmath>
#include <map>
#include <sofa.h>

#define MINMJDFORDUT1 43144.0
#define MAXMJDFORDUT1 57448.0

// Create a namespace with some useful stuff
namespace CppEphem {
    // Define an angle type so that we can differentiate between degrees or radians
    enum class CEAngleType{DEGREES, RADIANS} ;

    // Approximate temperature at sea-level in Kelvin
    inline double SeaLevelTemp_K() {return 288.2 ;}
    
    // Some constants taken from the sofa dictionary, but with some more discriptive names
    inline double julian_date_J2000() {return DJ00 ;}           // Julian Date corresponding to J2000
    inline double c() {return DC * DAU / DAYSEC ;}              // speed of light (meters/second)
    inline double c_au_per_day() {return DC ;}                  // speed of light (astronomical units)/day
    inline double m_per_au() {return DAU ;}                     // meters per astronomical unit
    
    /*********************************************
     * Return the 'dut1' value which represents 
     * 'UT1-UTC' for a given MJD or it's error
     *********************************************/
    double dut1(double mjd=51544.5) ;
    double dut1Error(double mjd=51544.5) ;
    inline std::map<int, double> dut1_list() {return {{51544.5,0.0}};}
    inline std::map<int, double> dut1Error_list() {return {{51544.5,0.0}};}
    double dut1Calc(double mjd) ;
    
    /*********************************************
     * Polar Motion methods
     * For the moment, 0.0 should be sufficient. This
     * assumption may need to be revisited
     *********************************************/
    inline double xp(double mjd=51544.5) {return 0.0 ;}
    inline double yp(double mjd=51544.5) {return 0.0 ;}
    
    // Methods for estimating altitude from atmospheric pressure (in hPa)
    inline double EstimateAltitude_m(double pressure_hPa)
        {return -29.3 * SeaLevelTemp_K() * std::log(pressure_hPa/1013.25) ;}
    inline double EstimatePressure_hPa(double elevation_m)
        {return 1013.25 * std::exp(-elevation_m / (29.3*SeaLevelTemp_K() )) ;}
}

#endif /* CENamespace_h */

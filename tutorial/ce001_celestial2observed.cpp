//
//  ce001_celestial2observed.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 1/16/17.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

/** \file
 * This is a tutorial on how to use the CEObserver class and the CECoordinates
 * class to obtain local sky coordinates for a given observer on a given date
 */

#include "CppEphem.h"

int main(int argc, char** argv)
{
    // Create a default object to observe. Here I'm using the Crab nebula
    //      Arg1 - Right Ascension
    //      Arg2 - Declination
    //      Arg3 - CIRS coordinates (Earth centric RA,Dec)
    //      Arg4 - Tells CEBody that the RA and Dec are in degrees (default is radians)
    CEBody crab_nebula(83.6331, 22.0145, CECoordinateType::ICRS, CEAngleType::DEGREES) ;
    
    // Set the time of the observation. I'll choose new years day, 2017 at 11 pm UTC
    //      Element 0 - Year
    //      Element 1 - Month
    //      Element 2 - Day
    //      Element 3 - Day fraction
    CEDate date( {2017, 01, 01, 23.0/24.0} ) ;
    
    // Alternatively, the date can also be specified in the following way:
    //      CEDate date( (20160101+(23.0/24.0)), CEDateType::GREGORIAN) ;
    
    // Create a default observer longitude and latitude in degrees. I'm using Ames, IA, USA.
    // Note that longitude is "east-positive" from Greenwich.
    //      Arg1 - Longitude (east-positive)
    //      Arg2 - Latitude
    //      Arg3 - Elevation (meters above sea level)
    //      Arg4 - Tells CEObserver that longitude and latitude are in degrees
    //      Arg5 - Date for the observation
    CEObserver observer(-93.62, 42.0347, 287.0, CEAngleType::DEGREES) ;
    
    // Now get the coordinates of the object
    CECoordinates observed_coords = observer.ObservedPosition(crab_nebula, date) ;
    
    /*****************************************************************
     * At this point we're done. Everything below here is just to
     * view the results of the above inputs.
     *****************************************************************/
    
    // Print this information to the screen
    std::printf("\n") ;
    std::printf("OBSERVER INFORMATION\n") ;
    std::printf("   Longitude: %+f\n", observer.Longitude_Deg()) ;
    std::printf("   Latitude : %+f\n", observer.Latitude_Deg()) ;
    std::printf("\nDATE INFORMATION\n") ;
    std::printf("   Julian Date: %+f\n", date.JD());
    std::printf("   Year : %+d\n", date.Year()) ;
    std::printf("   Month: %+d\n", date.Month()) ;
    std::printf("   Day  : %+d\n", date.Day()) ;
    std::printf("   DayFrac: %+f\n", date.DayFraction()) ;
    std::printf("\nOBJECT INFORMATION\n") ;
    std::printf("   RA : %+f\n", crab_nebula.XCoordinate_Deg()) ;
    std::printf("   Dec: %+f\n", crab_nebula.YCoordinate_Deg()) ;
    std::printf("   Azimuth : %+06.3f\n", observed_coords.XCoordinate_Deg()) ;
    std::printf("   Zenith  : %+05.3f\n", observed_coords.YCoordinate_Deg()) ;
    std::printf("   Altitude: %+05.3f\n", 90.0-observed_coords.YCoordinate_Deg()) ;
    std::printf("\n") ;
    
    return 0 ;
}

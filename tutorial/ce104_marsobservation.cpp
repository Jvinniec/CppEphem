//
//  ce104_marsobservation.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 1/30/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

/** \file
 * This is a tutorial on how to use the CEObservation class combined
 * with the CEPlanet class to observe a planet in the solar system.
 */

#include <stdio.h>
#include <unistd.h>

#include "CppEphem.h"

int main(int argc, char** argv)
{
    // Create the object representing Mercury
    CEPlanet mars = CEPlanet::Mars() ;
    
    // Create an object that always represents the current instantaneous time
    CERunningDate date ;
    
    // Create a default observer, longitude and latitude in degrees. I'm using Ames, IA, USA.
    // Note that longitude is "east-positive" from Greenwich.
    CEObserver observer(-93.62, 42.0347, 287.0, CEAngleType::DEGREES, &date) ;
    
    // Now get the coordinates of the object as a constantly updating 'CEObservation' object
    CEObservation observed_coords(&observer, &mars) ;
    
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
    
    // Print the object information, updated every ~1 second
    
    std::printf("\nOBJECT INFORMATION\n") ;
    std::printf("    JD    | Time UTC |   RA   |  DEC   | Azimuth| Zenith | Altitude\n") ;
    std::printf("----------+----------+--------+--------+--------+--------+----------\n") ;
    double ra, dec, azimuth, zenith ;
    while (true) {
        usleep(1000000) ;
        observed_coords.GetAzimuthZenith_Deg(&azimuth, &zenith) ;
//        observed_coords.GetApparentXYCoordinate_Deg(&ra, &dec) ;

        ra = mars.XCoordinate_Deg() ;
        dec = mars.YCoordinate_Deg() ;
//        CECoordinates::ICRS2CIRS(ra, dec, &ra, &dec,date, CEAngleType::DEGREES) ;
        std::printf("\r %8d | %8.1f |%8.3f|%+8.3f|%8.3f|%8.3f|%8.3f",
                    int(date), date.GetTime(),
                    ra, dec, azimuth, zenith, 90.0-zenith) ;
        std::cout.flush() ;
    }
    
    return 0 ;
}
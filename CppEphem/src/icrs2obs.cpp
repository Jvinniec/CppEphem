//
//  icrs2obs.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 5/12/16.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

#include <stdio.h>
#include <getopt.h>
#include <map>
#include <string>
#include <time.h>

// CppEphem HEADERS
#include "CppEphem.h"
#include "CLOptions.h"

/**********************************************************************//**
 *************************************************************************/
CLOptions DefineOpts()
{
    // Create a default observer
    CEObserver obs;
    
    // Create the
    CLOptions opts;
    opts.AddDoubleParam("X,longitude", "Observer longitude (degrees)",0.0);
    opts.AddDoubleParam("Y,latitude", "Observer latitude (degrees)",0.0);
    opts.AddDoubleParam("R,ra", "Right Ascension (degrees)", 0.0);
    opts.AddDoubleParam("D,dec", "Declination (degrees)", 0.0);
    opts.AddDoubleParam("j,juliandate", "Julian Date for query", CEDate::CurrentJD());
    opts.AddDoubleParam("e,elevation", "Observer elevation (meters above sea-level)", obs.Elevation_m());
    opts.AddDoubleParam("h,humidity", "Observer's relative humidity (0-1)", obs.RelativeHumidity());
    opts.AddDoubleParam("p,pressure", "Observer's atmospheric pressure (hPa)", obs.Pressure_hPa());
    opts.AddDoubleParam("t,temperature", "Observer's temperature (degrees Celsius)", obs.Temperature_C());
    opts.AddDoubleParam("w,wavelength", "Wavelength of light being observed (micrometers)", 0.5);
    opts.AddDoubleParam("d,dut1", "UT1-UTC", 0.0);
    opts.AddDoubleParam("x,xpolar", "x-polar motion (best to leave alone)", 0.0);
    opts.AddDoubleParam("y,ypolar", "y-polar motion (best to leave alone)", 0.0);
    
    return opts;
}


/**********************************************************************//**
 *************************************************************************/
void PrintResults(CECoordinates& input,
                  CECoordinates& output,
                  double jd)
{
    std::printf("\n") ;
    std::printf("******************************************\n") ;
    std::printf("* Results of CIRS -> Observed conversion *\n") ;
    std::printf("******************************************\n") ;
    std::printf("Observed Coordinates (output)\n") ;
    std::printf("    Azimuth        : %f degrees\n", output.XCoordinate_Deg()*DR2D) ;
    std::printf("    Zenith         : %+f degrees\n", output.YCoordinate_Deg()*DR2D) ;
    std::printf("    Altitude       : %+f degrees\n", 90.0-output.YCoordinate_Deg()*DR2D) ;
    std::printf("ICRS Coordinates (input)\n") ;
    std::printf("    Right Ascension: %f degrees\n", input.XCoordinate_Deg()) ;
    std::printf("    Declination    : %+f degrees\n", input.YCoordinate_Deg()) ;
    std::printf("    Julian Date    : %f\n", jd) ;
    std::printf("\n") ;
}

/**********************************************************************//**
 * Main
 *************************************************************************/
int main(int argc, char** argv)
{    
    CLOptions opts = DefineOpts();
    if (opts.ParseCommandLine(argc, argv)) return 0;
    
    // Create the input and output coordinates
    CECoordinates input(opts.AsDouble("ra"), opts.AsDouble("dec"),
                        CECoordinateType::ICRS, CEAngleType::DEGREES);
    
    // Convert the coordinates
    CECoordinates output = input.ConvertToObserved(opts.AsDouble("juliandate"),
                                                   opts.AsDouble("longitude"),
                                                   opts.AsDouble("latitude"),
                                                   opts.AsDouble("elevation"),
                                                   opts.AsDouble("pressure"),
                                                   opts.AsDouble("temperature"),
                                                   opts.AsDouble("humidity"),
                                                   opts.AsDouble("dut1"),
                                                   opts.AsDouble("xpolar"),
                                                   opts.AsDouble("ypolar"),
                                                   opts.AsDouble("wavelength"));
    
    // Print the results
    PrintResults(input, output, opts.AsDouble("juliandate")) ;
    
    return 0 ;
}

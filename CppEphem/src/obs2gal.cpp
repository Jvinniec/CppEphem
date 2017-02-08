//
//  obs2gal.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 2/3/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

#include <stdio.h>
#include <getopt.h>
#include <map>
#include <string>
#include <time.h>
//#include <vector>

// CppEphem HEADERS
#include "CppEphem.h"
#include "CLOptions.h"

//_________________________________________________________
CLOptions DefineOpts()
{
    CLOptions opts ;
    opts.AddProgramDescription("Takes observed positions (azimuth,zenith angle) and computes the Galactic longitude,latitude positions based on the observer location and atmospheric properties. The only values necessary to get rough coordinates are 'longitude', 'latitude', 'azimuth', 'zenith', and 'juliandate'.") ;
    
    // Setup the defaults
    CEObserver obs ;
    opts.AddDoubleParam("X,longitude","Observer longitude (degrees, East-positive)",obs.Longitude_Deg()) ;
    opts.AddDoubleParam("Y,latitude", "Observer latitude (degrees)", obs.Latitude_Deg()) ;
    opts.AddDoubleParam("A,azimuth", "Azimuth (degrees, positive North=0, East=90)", 0.0) ;
    opts.AddDoubleParam("Z,zenith", "Zenith angle (degrees)", 0.0) ;
    opts.AddDoubleParam("j,juliandate", "Julian date for query (default is current time)", CEDate::CurrentJD()) ;
    opts.AddDoubleParam("e,elevation", "Observer elevation (meters above sea level)", obs.Elevation_m()) ;
    opts.AddDoubleParam("r,humidity", "Observer's relative humidity (0-1)", obs.RelativeHumidity()) ;
    opts.AddDoubleParam("p,pressure","Observer's atmospheric pressure (hPa)",obs.Pressure_hPa()) ;
    opts.AddDoubleParam("t,temperature","Observer's atmospheric temperature (degrees Celsius)", obs.Temperature_C()) ;
    opts.AddDoubleParam("w,wavelength","Wavelength of light being observed (micrometers)",obs.Wavelength_um()) ;
    opts.AddDoubleParam("d,dut1","UT1-UTC (not necessary for rough positions)",CEDate::dut1(CEDate::CurrentJD())) ;
    opts.AddDoubleParam("x,xpolar","x-polar motion (not necessary for rough positions)",CEDate::xpolar(CEDate::CurrentJD())) ;
    opts.AddDoubleParam("y,ypolar","y-polar motion (not necessary for rough positions)",CEDate::ypolar(CEDate::CurrentJD())) ;
    
    return opts ;
}

//_________________________________________________________
void PrintResults(CLOptions& inputs, std::map<std::string, double> results)
{
    std::printf("\n") ;
    std::printf("******************************************\n") ;
    std::printf("* Results of Observed -> CIRS conversion *\n") ;
    std::printf("******************************************\n") ;
    std::printf("Observed Coordinates (output)\n") ;
    std::printf("    Azimuth        : %f degrees\n", inputs.AsDouble("azimuth")*DR2D) ;
    std::printf("    Zenith         : %+f degrees\n", inputs.AsDouble("zenith")*DR2D) ;
    std::printf("    Altitude       : %+f degrees\n", 90.0-inputs.AsDouble("zenith")*DR2D) ;
    std::printf("Galactic Coordinates (input)\n") ;
    std::printf("    G. Longitude   : %f degrees\n", results["ra"]) ;
    std::printf("    G. Latitude    : %+f degrees\n", results["dec"]) ;
    std::printf("Observer Info\n") ;
    std::printf("    Julian Date    : %f\n", inputs.AsDouble("juliandate")) ;
    std::printf("    Longitude      : %f deg\n", inputs.AsDouble("longitude")) ;
    std::printf("    Latitude       : %+f deg\n", inputs.AsDouble("latitude")) ;
    std::printf("    Elevation      : %f meters\n", inputs.AsDouble("elevation")) ;
    std::printf("    Pressure       : %f hPa\n", inputs.AsDouble("pressure")) ;
    std::printf("    Temperature    : %f Celsius\n", inputs.AsDouble("temperature")) ;
    std::printf("    Relative Humid.: %f\n", inputs.AsDouble("humidity")) ;
    std::printf("\n") ;
}

//_________________________________________________________
int main(int argc, char** argv) {
    
    // Parse the command line options
    CLOptions opts = DefineOpts() ;
    if (opts.ParseCommandLine(argc, argv)) return 0 ;
    
    // Create a map to store the results
    std::map<std::string, double> results ;
    results["glon"] = 0.0 ;
    results["glat"]  = 0.0 ;
    
    // Convert the coordinates
    int errcode = CECoordinates::Observed2CIRS(opts.AsDouble("azimuth")*DD2R,
                                               opts.AsDouble("zenith")*DD2R,
                                               &results["glon"], &results["glat"],
                                               opts.AsDouble("juliandate"),
                                               opts.AsDouble("longitude")*DD2R,
                                               opts.AsDouble("latitude")*DD2R,
                                               opts.AsDouble("elevation"),
                                               opts.AsDouble("pressure"),
                                               opts.AsDouble("temperature"),
                                               opts.AsDouble("humidity"),
                                               opts.AsDouble("dut1"),
                                               opts.AsDouble("xpolar"),
                                               opts.AsDouble("ypolar"),
                                               opts.AsDouble("wavelength")) ;
    // Print the results
    PrintResults(opts, results) ;
    
    return errcode ;
}
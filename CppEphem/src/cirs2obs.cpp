//
//  cirs2obs.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/26/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include <stdio.h>
#include <getopt.h>
#include <map>
#include <string>

// CppEphem HEADERS
#include "CECoordinates.h"

//_________________________________________________________
void Print_Help()
{
    // This is the help text to be printed if no command line options are provide
    // or if the '--help' or '-h' options are given
    std::printf("USAGE: cirs2obs [options]\n\n") ;
    std::printf("REQURED OPTIONS:\n") ;
    std::printf("  --longitude,   -X      Observer longitude (degrees)\n") ;
    std::printf("  --latitude,    -Y      Observer longitude (degrees)\n") ;
    std::printf("  --elevation    -e      Observer elevation (meters above sea-level)\n") ;
    std::printf("  --ra,          -R      Right Ascension (degrees)\n") ;
    std::printf("  --dec,         -D      Declination\n") ;
    std::printf("  --juliandate,  -j      Julian Date for query\n") ;
                
    std::printf("ADDITIONAL OPTIONS:\n") ;
    std::printf("  --help,        -h      Prints help text\n") ;
    std::printf("  --pressure,    -p      Observer's atmospheric pressure (hPa)\n") ;
    std::printf("  --temperature, -t      Observer's temperature (degrees Celsius)\n") ;
}

//_________________________________________________________
std::map<std::string, double> defaultoptions()
{
    // Define the default values of some of the unnecessary parameters
    std::map<std::string, double> options ;
    options["xpolar"] = 0.0 ;
    options["ypolar"] = 0.0 ;
    
    return options ;
}

//_________________________________________________________
const struct option* getoptions()
{
    const struct option longopts[] =
    {
        {"help",        no_argument,       0, 'h'},   // Print help information
        {"longitude",   required_argument, 0, 'X'},   // Observer longitude
        {"latitude",    required_argument, 0, 'Y'},   // Observer latitude
        {"elevation",   optional_argument, 0, 'e'},   // Observer elevation
        {"ra",          required_argument, 0, 'R'},   // CIRS right ascension
        {"dec",         required_argument, 0, 'D'},   // CIRS declination
        {"juliandate",  required_argument, 0, 'j'},   // Julian date of observation
        {"pressure",    optional_argument, 0, 'p'},   // pressure (hPa)
        {"temperature", optional_argument, 0, 't'},   // Temperature (degrees C)
        {"humidity",    optional_argument, 0, 'r'},   // Relative humidity (0-1)
        {"wavelength",  optional_argument, 0, 'w'},   // Wavelength of light (micrometers)
        {"xpolar",      optional_argument, 0, 'x'},   // x-component of polar motion (can be 0)
        {"ypolar",      optional_argument, 0, 'y'},   // y-component of polar motion (can be 0)
        {0,0,0,0},
    };
    
    return longopts ;
}

//_________________________________________________________
std::map<std::string, double> parseoptions(int argc, char** argv, const struct option* longopts)
{
    // Setup the default parameters
    std::map<std::string, double> options = defaultoptions() ;
    
    int c;
    
    while (1)
    {
        /* getopt_long stores the option index here. */
        int option_index = 0;
        
        c = getopt_long (argc, argv, "hX:Y:e:R:D:j:p:t:r:w:x:y:",
                         longopts, &option_index);
        
        /* Detect the end of the options. */
        if (c == -1) break;
        
        switch (c)
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (longopts[option_index].flag != 0)
                    break;
                
                // Set the option
                options[longopts[option_index].name] = std::stod(optarg) ;
                break;
                
            case 'h':
                // Print the help message and quit
                Print_Help() ;
                return std::map<std::string, double>() ;
                
            case 'X':
                options["longitude"] = std::stod(optarg) ;
                break;
                
            case 'Y':
                options["latitude"] = std::stod(optarg) ;
                break;
                
            case 'e':
                options["elevation"] = std::stod(optarg) ;
                break;
                
            case 'R':
                options["ra"] = std::stod(optarg) ;
                break;
                
            case 'D':
                options["dec"] = std::stod(optarg) ;
                break;
                
            case 'j':
                options["juliandate"] = std::stod(optarg) ;
                break;
                
            case 'p':
                options["pressure"] = std::stod(optarg) ;
                break;
                
            case 't':
                options["temperature"] = std::stod(optarg) ;
                break;
                
            case 'r':
                options["humidity"] = std::stod(optarg) ;
                break;
                
            case 'w':
                options["wavelength"] = std::stod(optarg) ;
                break;
                
            case 'x':
                options["xpolar"] = std::stod(optarg) ;
                break;
                
            case 'y':
                options["ypolar"] = std::stod(optarg) ;
                break;
                
            case '?':
                /* getopt_long already printed an error message. */
                break;
                
            default:
                break ;
        }
    }

    
    return options ;
}

//_________________________________________________________
void PrintResults(std::map<std::string, double> inputs, std::map<std::string, double> results)
{
    std::printf("\n") ;
    std::printf("******************************************\n") ;
    std::printf("* Results of CIRS -> Observed conversion *\n") ;
    std::printf("******************************************\n") ;
    
    std::printf("CIRS Coordinates (input)\n") ;
    std::printf("    Right Ascension: %f degrees\n", inputs["ra"]) ;
    std::printf("    Declination    : %+f degrees\n", inputs["dec"]) ;
    std::printf("Observed Coordinates (output)\n") ;
    std::printf("    Azimuth        : %f degrees\n", results["azimuth"]*DR2D) ;
    std::printf("    Zenith         : %+f degrees\n", results["zenith"]*DR2D) ;
    std::printf("Apparent CIRS Coordinates\n") ;
    std::printf("    Right Ascension: %f\n", results["observed_ra"]*DR2D) ;
    std::printf("    Declination    : %f\n", results["observed_dec"]*DR2D) ;
    std::printf("    Hour Angle     : %+f\n", results["hour_angle"]*DR2D) ;
    std::printf("\n ...done\n") ;
}

//_________________________________________________________
int main(int argc, char** argv) {
    
    // Define the optional arguments
    const struct option* longopts = getoptions() ;
    
    // Parse the options
    std::map<std::string, double> options = parseoptions(argc, argv, longopts) ;
    
    // Create a map to store the results
    std::map<std::string, double> results ;
    results["azimuth"] = 0.0 ;
    results["zenith"]  = 0.0 ;
    results["observed_ra"] = 0.0 ;
    results["observed_dec"] = 0.0 ;
    results["hour_angle"] = 0.0 ;
    
    // Convert the coordinates
    int errcode = CECoordinates::CIRS2Observed(options["ra"]*DD2R, options["dec"]*DD2R,
                                               &results["azimuth"], &results["zenith"],
                                               options["juliandate"],
                                               options["longitude"]*DD2R,
                                               options["latitude"]*DD2R,
                                               options["elevation"],
                                               options["pressure"],
                                               options["temperature"],
                                               options["humidity"],
                                               options["dut1"],
                                               options["xpolar"], options["ypolar"],
                                               options["wavelength"],
                                               &results["observed_ra"],
                                               &results["observed_dec"],
                                               &results["hour_angle"]) ;
    
    // Handle the error code
    
    
    // Print the results
    PrintResults(options, results) ;
    
    return errcode ;
}

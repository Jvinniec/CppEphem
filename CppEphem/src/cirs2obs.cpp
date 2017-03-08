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
#include <time.h>

// CppEphem HEADERS
#include "CppEphem.h"

//_________________________________________________________
void Print_Help()
{
    // This is the help text to be printed if no command line options are provide
    // or if the '--help' or '-h' options are given
    std::printf("\nUSAGE: cirs2obs [options]\n") ;
    
    std::printf("\nREQURED OPTIONS:\n") ;
    std::printf("  --longitude,   -X      Observer longitude (degrees, East-positive)\n") ;
    std::printf("  --latitude,    -Y      Observer latitude (degrees)\n") ;
    std::printf("  --ra,          -R      Right Ascension (degrees)\n") ;
    std::printf("  --dec,         -D      Declination (degrees)\n") ;

    // Create an observer with default values so we know what the defaults will be
    CEObserver obs ;
    
    std::printf("\nADDITIONAL OPTIONS:\n") ;
    std::printf("  --help,        -h      Prints help text\n") ;
    std::printf("  --juliandate,  -j      Julian Date for query (default is current time)\n") ;
    std::printf("  --elevation    -e      Observer elevation (meters above sea-level, default=%f)\n", obs.Elevation_m()) ;
    std::printf("  --humidity     -r      Observer's relative humidity (0-1, default=%f)\n", obs.RelativeHumidity()) ;
    std::printf("  --pressure,    -p      Observer's atmospheric pressure (hPa, default=%f)\n", obs.Pressure_hPa()) ;
    std::printf("  --temperature, -t      Observer's temperature (degrees Celsius, default=%f)\n", obs.Temperature_C()) ;
    std::printf("  --wavelength,  -w      Wavelength of light being observed (micrometers, default=%2.1f)\n", 0.5) ;
    std::printf("  --dut1         -d      UT1-UTC (default=0)\n") ;
    std::printf("  --xpolar,      -x      x-polar motion (default=0, best to leave alone)\n") ;
    std::printf("  --ypolar,      -y      y-polar motion (default=0, best to leave alone)\n") ;
    
    std::printf("\n") ;
}

//_________________________________________________________
std::map<std::string, double> defaultoptions()
{
    // Define the default values of some of the unnecessary parameters
    std::map<std::string, double> options ;
    options["dut1"] = 0.0 ;
    options["xpolar"] = 0.0 ;
    options["ypolar"] = 0.0 ;
    
    // Set the current time as the default date for query
    time_t current_time = time(NULL) ;
    struct tm * timeinfo = gmtime(&current_time) ;
    double dayfrac = (timeinfo->tm_hour + (timeinfo->tm_min/60.0) + ((timeinfo->tm_sec)/3600.0))/24.0 ;
    CEDate date({timeinfo->tm_year+1900.0, timeinfo->tm_mon+1.0, 1.0*timeinfo->tm_mday, dayfrac}) ;
    
    options["juliandate"] = date.JD() ;
    
    // Set the default observer weather conditions
    CEObserver observer ;
    options["elevation"] = observer.Elevation_m() ;
    options["humidity"] = observer.RelativeHumidity() ;
    options["pressure"] = observer.Pressure_hPa() ;
    options["temperature"] = observer.Temperature_C() ;
    
    return options ;
}

//_________________________________________________________
void getoptions(struct option* longopts)
{
    longopts[0]  = {"help",        no_argument,       0, 'h'};   // Print help information
    longopts[1]  = {"longitude",   required_argument, 0, 'X'};   // Observer longitude
    longopts[2]  = {"latitude",    required_argument, 0, 'Y'};   // Observer latitude
    longopts[3]  = {"elevation",   optional_argument, 0, 'e'};   // Observer elevation
    longopts[4]  = {"ra",          required_argument, 0, 'R'};   // CIRS right ascension
    longopts[5]  = {"dec",         required_argument, 0, 'D'};   // CIRS declination
    longopts[6]  = {"juliandate",  required_argument, 0, 'j'};   // Julian date of observation
    longopts[7]  = {"pressure",    optional_argument, 0, 'p'};   // pressure (hPa)
    longopts[8]  = {"temperature", optional_argument, 0, 't'};   // Temperature (degrees C)
    longopts[9]  = {"humidity",    optional_argument, 0, 'r'};   // Relative humidity (0-1)
    longopts[10] = {"wavelength",  optional_argument, 0, 'w'};   // Wavelength of light (micrometers)
    longopts[11] = {"dut1",        optional_argument, 0, 'd'};   // UT1-UTC
    longopts[12] = {"xpolar",      optional_argument, 0, 'x'};   // x-component of polar motion (can be 0)
    longopts[13] = {"ypolar",      optional_argument, 0, 'y'};   // y-component of polar motion (can be 0)
    longopts[14] = {0,0,0,0} ;
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
            case 'X':   // Assign observer's Longitude
                options["longitude"] = std::stod(optarg) ;
                break;
            case 'Y':   // Assign observer's Latitude
                options["latitude"] = std::stod(optarg) ;
                break;
            case 'e':   // Assign observer's elevation
                options["elevation"] = std::stod(optarg) ;
                break;
            case 'R':   // Assign Right Ascension coordinate
                options["ra"] = std::stod(optarg) ;
                break;
            case 'D':   // Assign Declination coordinate
                options["dec"] = std::stod(optarg) ;
                break;
            case 'j':   // Assign julian date
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
            case 'd':
                options["dut1"] = std::stod(optarg) ;
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
    std::printf("Observed Coordinates (output)\n") ;
    std::printf("    Azimuth        : %f degrees\n", results["azimuth"]*DR2D) ;
    std::printf("    Zenith         : %+f degrees\n", results["zenith"]*DR2D) ;
    std::printf("    Altitude       : %+f degrees\n", 90.0-results["zenith"]*DR2D) ;
    std::printf("CIRS Coordinates (input)\n") ;
    std::printf("    Right Ascension: %f degrees\n", inputs["ra"]) ;
    std::printf("    Declination    : %+f degrees\n", inputs["dec"]) ;
    std::printf("    Julian Date    : %f\n", inputs["juliandate"]) ;
    std::printf("Apparent CIRS Coordinates\n") ;
    std::printf("    Right Ascension: %f\n", results["observed_ra"]*DR2D) ;
    std::printf("    Declination    : %+f\n", results["observed_dec"]*DR2D) ;
    std::printf("    Hour Angle     : %+f\n", results["hour_angle"]*DR2D) ;
    std::printf("Observer Info\n") ;
    std::printf("    Longitude      : %f deg\n", inputs["longitude"]) ;
    std::printf("    Latitude       : %+f deg\n", inputs["latitude"]) ;
    std::printf("    Elevation      : %f meters\n", inputs["elevation"]) ;
    std::printf("    Pressure       : %f hPa\n", inputs["pressure"]) ;
    std::printf("    Temperature    : %f Celsius\n", inputs["temperature"]) ;
    std::printf("    Relative Humid.: %f\n", inputs["humidity"]) ;
    std::printf("\n") ;
}

//_________________________________________________________
int main(int argc, char** argv) {
    
    // If no arguments have been passed, print the help text and quit
    if (argc==1) {
        Print_Help() ;
        return 0 ;
    }
    
    // Define the optional arguments
    struct option longopts[15] ;
    getoptions(longopts) ;
    
    // Parse the options
    std::map<std::string, double> options = parseoptions(argc, argv, longopts) ;
    
    // If no valid options were given, quit
    if (options.empty()) return 0 ;
    
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
    
    // Print the results
    PrintResults(options, results) ;
    
    return errcode ;
}

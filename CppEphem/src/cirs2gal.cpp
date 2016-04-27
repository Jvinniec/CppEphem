//
//  cirs2gal.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/27/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include <stdio.h>
#include <getopt.h>
#include <map>
#include <string>
#include <time.h>

// CppEphem HEADERS
#include "CECoordinates.h"
#include "CEDate.h"

//_________________________________________________________
void Print_Help()
{
    // This is the help text to be printed if no command line options are provide
    // or if the '--help' or '-h' options are given
    std::printf("\nUSAGE: cirs2gal [options]\n") ;
    
    std::printf("\nREQURED OPTIONS:\n") ;
    std::printf("  --ra,          -R      Right Ascension (degrees)\n") ;
    std::printf("  --dec,         -D      Declination (degrees)\n") ;
    
    std::printf("\nADDITIONAL OPTIONS:\n") ;
    std::printf("  --help,        -h      Prints help text\n") ;
    std::printf("  --juliandate,  -j      Julian Date for query (default=%f)\n", CppEphem::julian_date_J2000()) ;
    
    std::printf("\n") ;
}

//_________________________________________________________
std::map<std::string, double> defaultoptions()
{
    // Define the default values of some of the unnecessary parameters
    std::map<std::string, double> options ;
    
    // Set January 1, 2000 as the default date
    options["juliandate"] = CppEphem::julian_date_J2000() ;
    
    return options ;
}

//_________________________________________________________
const struct option* getoptions()
{
    const struct option longopts[] =
    {
        {"help",        no_argument,       0, 'h'},   // Print help information
        {"ra",          required_argument, 0, 'R'},   // CIRS right ascension
        {"dec",         required_argument, 0, 'D'},   // CIRS declination
        {"juliandate",  required_argument, 0, 'j'},   // Julian date of observation
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
        
        c = getopt_long (argc, argv, "hR:D:j:",
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
                
            case 'R':
                options["ra"] = std::stod(optarg) ;
                break;
                
            case 'D':
                options["dec"] = std::stod(optarg) ;
                break;
                
            case 'j':
                options["juliandate"] = std::stod(optarg) ;
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
    std::printf("* Results of CIRS -> Galactic conversion *\n") ;
    std::printf("******************************************\n") ;
    std::printf("Galactic Coordinates (output)\n") ;
    std::printf("    Galactic Lon.: %f degrees\n", results["longitude"]*DR2D) ;
    std::printf("    Galactic Lat.: %+f degrees\n", results["latitude"]*DR2D) ;
    std::printf("CIRS Coordinates (input)\n") ;
    std::printf("    Right Ascension: %f degrees\n", inputs["ra"]) ;
    std::printf("    Declination    : %+f degrees\n", inputs["dec"]) ;
    std::printf("    Julian Date    : %f\n", inputs["juliandate"]) ;
    std::printf("\n...done\n") ;
}

//_________________________________________________________
int main(int argc, char** argv) {
    
    // If no arguments have been passed, print the help text and quit
    if (argc==1) {
        Print_Help() ;
        return 0 ;
    }
    
    // Define the optional arguments
    const struct option* longopts = getoptions() ;
    
    // Parse the options
    std::map<std::string, double> options = parseoptions(argc, argv, longopts) ;
    
    // If no valid options were given, quit
    if (options.empty()) return 0 ;
    
    // Create a map to store the results
    std::map<std::string, double> results ;
    results["longitude"] = 0.0 ;
    results["latitude"]  = 0.0 ;
    
    // Convert the coordinates
    CECoordinates::CIRS2Galactic(options["ra"]*DD2R,
                                 options["dec"]*DD2R,
                                 &results["longitude"],
                                 &results["latitude"],
                                 CEDate(options["juliandate"])) ;
    
    // Print the results
    PrintResults(options, results) ;
    
    return 0 ;
}
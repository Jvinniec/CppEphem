/***************************************************************************
 *  gal2cirs.cpp: CppEphem                                                 *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016 JCardenzana                                           *
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

#include <stdio.h>
#include <getopt.h>
#include <map>
#include <string>
#include <time.h>

// CppEphem HEADERS
#include "CppEphem.h"

/**********************************************************************//**
 *************************************************************************/
void Print_Help()
{
    // This is the help text to be printed if no command line options are provide
    // or if the '--help' or '-h' options are given
    std::printf("\nUSAGE: gal2cirs [options]\n") ;
    
    std::printf("\nREQURED OPTIONS:\n") ;
    std::printf("  --glon,        -L      Galactic longitude (degrees)\n") ;
    std::printf("  --glat,        -B      Galactic latitude (degrees)\n") ;
    
    std::printf("\nADDITIONAL OPTIONS:\n") ;
    std::printf("  --help,        -h      Prints help text\n") ;
    std::printf("  --juliandate,  -j      Julian Date for query (default=%f)\n", CppEphem::julian_date_J2000()) ;
    
    std::printf("\n") ;
}

/**********************************************************************//**
 *************************************************************************/
std::map<std::string, double> defaultoptions()
{
    // Define the default values of some of the unnecessary parameters
    std::map<std::string, double> options ;
    
    // Set January 1, 2000 as the default date
    options["juliandate"] = CppEphem::julian_date_J2000() ;
    
    return options ;
}

/**********************************************************************//**
 *************************************************************************/
void getoptions(struct option* longopts)
{
    longopts[0] = {"help",        no_argument,       0, 'h'};   // Print help information
    longopts[1] = {"glon",        required_argument, 0, 'L'};   // CIRS right ascension
    longopts[2] = {"glat",        required_argument, 0, 'B'};   // CIRS declination
    longopts[3] = {"juliandate",  required_argument, 0, 'j'};   // Julian date of observation
    longopts[4] = {0,0,0,0};
}

/**********************************************************************//**
 *************************************************************************/
std::map<std::string, double> parseoptions(int argc, char** argv, const struct option* longopts)
{
    // Setup the default parameters
    std::map<std::string, double> options = defaultoptions() ;
    
    int c;
    
    while (1)
    {
        /* getopt_long stores the option index here. */
        int option_index = 0;
        
        c = getopt_long (argc, argv, "hL:B:j:",
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
                
            case 'L':
                options["glon"] = std::stod(optarg) ;
                break;
                
            case 'B':
                options["glat"] = std::stod(optarg) ;
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

/**********************************************************************//**
 *************************************************************************/
void PrintResults(std::map<std::string, double> inputs, std::map<std::string, double> results)
{
    std::printf("\n") ;
    std::printf("******************************************\n") ;
    std::printf("* Results of Galactic -> CIRS conversion *\n") ;
    std::printf("******************************************\n") ;
    std::printf("CIRS Coordinates (output)\n") ;
    std::printf("    Right Ascension: %f degrees\n", results["ra"]*DR2D) ;
    std::printf("    Declination    : %+f degrees\n", results["dec"]*DR2D) ;
    std::printf("Galactic Coordinates (input)\n") ;
    std::printf("    Galactic Lon.  : %f degrees\n", inputs["glon"]) ;
    std::printf("    Galactic Lat.  : %+f degrees\n", inputs["glat"]) ;
    std::printf("    Julian Date    : %f\n", inputs["juliandate"]) ;
    std::printf("\n") ;
}

/**********************************************************************//**
 *************************************************************************/
int main(int argc, char** argv) {
    
    // If no arguments have been passed, print the help text and quit
    if (argc==1) {
        Print_Help() ;
        return 0 ;
    }
    
    // Define the optional arguments
    struct option longopts[5] ;
    getoptions(longopts) ;
    
    // Parse the options
    std::map<std::string, double> options = parseoptions(argc, argv, longopts) ;
    
    // If no valid options were given, quit
    if (options.empty()) return 0 ;
    
    // Create a map to store the results
    std::map<std::string, double> results ;
    results["ra"] = 0.0 ;
    results["dec"]  = 0.0 ;
    
    // Convert the coordinates
    CECoordinates::Galactic2CIRS(options["glon"]*DD2R,
                                 options["glat"]*DD2R,
                                 &results["ra"],
                                 &results["dec"],
                                 CEDate(options["juliandate"])) ;
    
    // Print the results
    PrintResults(options, results) ;
    
    return 0 ;
}

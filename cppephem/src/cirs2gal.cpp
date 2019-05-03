/***************************************************************************
 *  cirs2gal.cpp: CppEphem                                                 *
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
#include "CLOptions.h"

/**********************************************************************//**
 * Define the command line options for this program
 *************************************************************************/
CLOptions DefineOpts()
{
    CLOptions opts ;
    // Set the options
    opts.AddDoubleParam("r,ra", "CIRS right ascension for the conversion (degrees)",0.0) ;
    opts.AddDoubleParam("d,dec", "CIRS declination for the conversion (degrees)", 0.0) ;
    opts.AddDoubleParam("j,juliandate", "Julian date for query (default is current JD)", 
                        CEDate::CurrentJD());
    
    return opts;
}


/**********************************************************************//**
 *************************************************************************/
void PrintResults(CLOptions& opts, std::map<std::string, double> results)
{
    std::printf("\n") ;
    std::printf("******************************************\n") ;
    std::printf("* Results of CIRS -> Galactic conversion *\n") ;
    std::printf("******************************************\n") ;
    std::printf("Galactic Coordinates (output)\n") ;
    std::printf("    Galactic Lon.: %f degrees\n", results["longitude"]*DR2D) ;
    std::printf("    Galactic Lat.: %+f degrees\n", results["latitude"]*DR2D) ;
    std::printf("CIRS Coordinates (input)\n") ;
    std::printf("    Right Ascension: %f degrees\n", opts.AsDouble("ra")) ;
    std::printf("    Declination    : %+f degrees\n", opts.AsDouble("dec")) ;
    std::printf("    Julian Date    : %f\n", opts.AsDouble("juliandate")) ;
    std::printf("\n") ;
}

/**********************************************************************//**
 *************************************************************************/
int main(int argc, char** argv) 
{    
    // Get the options from the command line
    CLOptions opts = DefineOpts() ;
    if (opts.ParseCommandLine(argc, argv)) return 0 ;
    
    // Create a map to store the results
    std::map<std::string, double> results ;
    results["longitude"] = 0.0 ;
    results["latitude"]  = 0.0 ;
    
    // Convert the coordinates
    CEDate date(opts.AsDouble("juliandate"), CEDateType::JD);
    CECoordinates::CIRS2Galactic(opts.AsDouble("ra")*DD2R,
                                 opts.AsDouble("dec")*DD2R,
                                 &results["longitude"],
                                 &results["latitude"],
                                 date);
    
    // Print the results
    PrintResults(opts, results) ;
    
    return 0 ;
}

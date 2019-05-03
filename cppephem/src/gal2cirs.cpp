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
#include "CLOptions.h"

/**********************************************************************//**
 * Define the command line options for this program
 *************************************************************************/
CLOptions DefineOpts()
{
    CLOptions opts ;
    // Set the options
    opts.AddDoubleParam("l,glon", "Galactic longitude (degrees)",0.0);
    opts.AddDoubleParam("b,glat", "Galactic latitude (degrees)", 0.0);
    opts.AddDoubleParam("j,juliandate", "Julian date for query (default is current JD)", 
                        CEDate::CurrentJD());
    
    return opts;
}


/**********************************************************************//**
 *************************************************************************/
void PrintResults(CLOptions& inputs, std::map<std::string, double> results)
{
    std::printf("\n") ;
    std::printf("******************************************\n") ;
    std::printf("* Results of Galactic -> CIRS conversion *\n") ;
    std::printf("******************************************\n") ;
    std::printf("CIRS Coordinates (output)\n") ;
    std::printf("    Right Ascension: %f degrees\n", results["ra"]*DR2D);
    std::printf("    Declination    : %+f degrees\n", results["dec"]*DR2D);
    std::printf("Galactic Coordinates (input)\n");
    std::printf("    Galactic Lon.  : %f degrees\n", inputs.AsDouble("glon"));
    std::printf("    Galactic Lat.  : %+f degrees\n", inputs.AsDouble("glat"));
    std::printf("    Julian Date    : %f\n", inputs.AsDouble("juliandate"));
    std::printf("\n");
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
    results["ra"]  = 0.0 ;
    results["dec"] = 0.0 ;
    
    // Convert the coordinates
    CEDate date(opts.AsDouble("juliandate"), CEDateType::JD);
    CECoordinates::Galactic2CIRS(opts.AsDouble("glon")*DD2R,
                                 opts.AsDouble("glat")*DD2R,
                                 &results["ra"],
                                 &results["dec"],
                                 date);
    
    // Print the results
    PrintResults(opts, results) ;
    
    return 0 ;
}

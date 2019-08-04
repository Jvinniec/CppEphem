/***************************************************************************
 *  cirs2gal.cpp: CppEphem                                                 *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016-2019 JCardenzana                                      *
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
#include "CEExecOptions.h"

/**********************************************************************//**
 * Define the command line options for this program
 *************************************************************************/
CEExecOptions DefineOpts()
{
    CEExecOptions opts("cirs2gal");

    // Add a program version and description
    opts.AddProgramDescription(std::string() +
        "Converts from CIRS coordinates to Galactic coordinates for a given " +
        "Julian date.");
        
    // Set the options
    opts.AddCirsPars();
    opts.AddJDPar();
    
    return opts;
}


/**********************************************************************//**
 *************************************************************************/
void PrintResults(CEExecOptions&    opts, 
                  const CESkyCoord& results)
{
    std::printf("\n") ;
    std::printf("******************************************\n") ;
    std::printf("* Results of CIRS -> Galactic conversion *\n") ;
    std::printf("******************************************\n") ;
    std::printf("Galactic Coordinates (output)\n") ;
    std::printf("    Galactic Lon.: %f degrees\n", results.XCoord().Deg()) ;
    std::printf("    Galactic Lat.: %+f degrees\n", results.YCoord().Deg()) ;
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
    CEExecOptions opts = DefineOpts();
    if (opts.ParseCommandLine(argc, argv)) return 0;
    
    // Convert the coordinates
    CEDate date(opts.AsDouble("juliandate"), CEDateType::JD);
    CESkyCoord cirs_coord(CEAngle::Deg(opts.AsDouble("ra")),
                          CEAngle::Deg(opts.AsDouble("dec")),
                          CESkyCoordType::CIRS);
    CESkyCoord gal_coord = cirs_coord.ConvertToGalactic(date);
    
    // Print the results
    PrintResults(opts, gal_coord);
    
    return 0 ;
}

/***************************************************************************
 *  icrs2gal.cpp: CppEphem                                                 *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2017-2019 JCardenzana                                      *
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
    CEExecOptions opts("icrs2gal");

    // Add version and description
    opts.AddProgramDescription(std::string() +
        "Converts from ICRS (solar system barycentric) coordinates to " +
        "Galactic coordinates.");

    // Set the options
    opts.AddIcrsPars();
    
    return opts;
}

/**********************************************************************//**
 * Print the results of the analysis
 *************************************************************************/
void PrintResults(const CESkyCoord& input,
                  const CESkyCoord& output)
{
    // Get the representation of the input icrs in hours, minutes, seconds
    std::vector<double> input_hms = input.XCoord().HmsVect();
    
    std::printf("\n") ;
    std::printf("******************************************\n") ;
    std::printf("* Results of ICRS -> Galactic conversion *\n") ;
    std::printf("******************************************\n") ;
    std::printf("Galactic Coordinates (output)\n") ;
    std::printf("    Galactic Lon.: %f degrees\n", output.XCoord().Deg()) ;
    std::printf("    Galactic Lat.: %+f degrees\n", output.YCoord().Deg()) ;
    std::printf("ICRS Coordinates (input)\n") ;
    std::printf("    Right Ascension: %02dh %02dm %04.1fs (%f deg)\n",
                int(input_hms[0]), int(input_hms[1]), input_hms[2],
                input.XCoord().Deg()) ;
    std::printf("    Declination    : %+f degrees\n", input.YCoord().Deg()) ;
    std::printf("\n") ;
}

/**********************************************************************//**
 * Main method
 *************************************************************************/
int main(int argc, char** argv) 
{    
    // Get the options from the command line
    CEExecOptions opts = DefineOpts();
    if (opts.ParseCommandLine(argc, argv)) return 0;
        
    // Create a map to store the results
    CESkyCoord icrs_coords(CEAngle::Deg(opts.AsDouble("ra")),
                          CEAngle::Deg(opts.AsDouble("dec")),
                          CESkyCoordType::ICRS);
    
    // Convert the coordinates
    CESkyCoord gal_coords = icrs_coords.ConvertToGalactic();
    
    // Print the results
    PrintResults(icrs_coords, gal_coords);
    
    return 0 ;
}

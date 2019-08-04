/***************************************************************************
 *  gal2icrs.cpp: CppEphem                                                 *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2019 JCardenzana                                           *
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

/** \file gal2icrs.cpp
 * Executable for converting from Galactic to ICRS coordinates
 */

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
    CEExecOptions opts("gal2icrs");

    // Add version and description
    opts.AddProgramDescription(std::string() +
            "Converts from Galactic longitude,latitude coordinates to ICRS " + 
            "(solar system barycentric) coordinates");

    // Set the options
    opts.AddGalacticPars();
    
    return opts;
}

/**********************************************************************//**
 * Print the results of the analysis
 *************************************************************************/
void PrintResults(const CESkyCoord& input,
                  const CESkyCoord& output)
{
    // Get the representation of the input icrs in hours, minutes, seconds
    std::vector<double> out_hms = output.XCoord().HmsVect();
    std::vector<double> out_dms = output.YCoord().DmsVect();
    
    std::printf("\n");
    std::printf("******************************************\n");
    std::printf("* Results of Galactic -> ICRS conversion *\n");
    std::printf("******************************************\n");
    std::printf("ICRS Coordinates (output)\n");
    std::printf("    Right Ascension: %02dh %02dm %04.1fs (%f deg)\n",
                int(out_hms[0]), int(out_hms[1]), out_hms[2]+out_hms[3],
                output.XCoord().Deg());
    std::printf("    Declination    : %+f degrees\n", input.YCoord().Deg());
    std::printf("Galactic Coordinates (input)\n");
    std::printf("    Galactic Lon.: %f degrees\n", input.XCoord().Deg());
    std::printf("    Galactic Lat.: %+f degrees\n", input.YCoord().Deg());
    std::printf("\n");
}

/**********************************************************************//**
 * Main method
 *************************************************************************/
int main(int argc, char** argv) {
    
    // Get the options from the command line
    CEExecOptions opts = DefineOpts();
    if (opts.ParseCommandLine(argc, argv)) return 0;
        
    // Create a map to store the results
    CESkyCoord gal_coord(CEAngle::Deg(opts.AsDouble("glon")), 
                         CEAngle::Deg(opts.AsDouble("glat")),
                         CESkyCoordType::GALACTIC);
    
    // Convert the coordinates
    CESkyCoord output = gal_coord.ConvertToICRS();
    
    // Print the results
    PrintResults(gal_coord, output);
    
    return 0 ;
}

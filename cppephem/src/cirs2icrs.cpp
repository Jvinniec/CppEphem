/***************************************************************************
 *  cirs2icrs.cpp: CppEphem                                                *
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

/** \file cirs2icrs.cpp
 * Executable for converting from CIRS to ICRS coordinates
 */

#include <stdio.h>
#include "CEExecOptions.h"

#include "CppEphem.h"

/**********************************************************************//**
 * Define the command line options for this program
 *************************************************************************/
CEExecOptions DefineOpts()
{
    CEExecOptions opts("cirs2icrs");

    // Add version and description
    opts.AddProgramDescription(std::string() +
        "Converts from CIRS (Earth centric) coordinates to ICRS (solar " +
        "system barycentric) coordinates for a given Julian date.");

    // Set the options
    opts.AddCirsPars();
    opts.AddJDPar();
    
    return opts;
}

/**********************************************************************//**
 * Print the results of the conversion
 *************************************************************************/
void PrintResults(const  CESkyCoord& input,
                  const  CESkyCoord& output,
                  double jd)
{
    std::vector<double> input_hms = input.XCoord().HmsVect();
    std::vector<double> output_hms = output.XCoord().HmsVect();
    
    std::printf("\n") ;
    std::printf("**********************************************\n") ;
    std::printf("* Results of CIRS -> ICRS                    *\n") ;
    std::printf("**********************************************\n") ;
    std::printf("ICRS Coordinates (output)\n") ;
    std::printf("    Julian Date    : %f\n", jd) ;
    std::printf("    Right Ascension: %02dh %02dm %04.1fs (%f deg)\n",
                int(output_hms[0]), int(output_hms[1]), output_hms[2]+output_hms[3],
                output.XCoord().Deg()) ;
    std::printf("    Declination    : %+f degrees\n", output.YCoord().Deg()) ;
    std::printf("CIRS Coordinates (input)\n") ;
    std::printf("    Right Ascension: %02dh %02dm %04.1fs (%f deg)\n",
                int(input_hms[0]), int(input_hms[1]), input_hms[2]+input_hms[3],
                input.XCoord().Deg()) ;
    std::printf("    Declination    : %+f degrees\n", input.YCoord().Deg()) ;
    std::printf("\n") ;
}


/**********************************************************************//**
 * MAIN
 *************************************************************************/
int main (int argc, char** argv)
{
    // Get the options from the command line
    CEExecOptions opts = DefineOpts() ;
    if (opts.ParseCommandLine(argc, argv)) return 0 ;
    
    // Create a CECoordinates object
    CESkyCoord input(CEAngle::Deg(opts.AsDouble("ra")), 
                     CEAngle::Deg(opts.AsDouble("dec")),
                     CESkyCoordType::CIRS) ;
    
    // Get the coordinates as CIRS
    CESkyCoord output = input.ConvertToICRS(opts.AsDouble("juliandate")) ;
    
    // Print the result
    PrintResults(input, output, opts.AsDouble("juliandate")) ;
}

/***************************************************************************
 *  icrs2cirs.cpp: CppEphem                                                *
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

/** \file icrs2cirs.cpp
 * Executable for converting from ICRS to CIRS coordinates
 */

#include <stdio.h>
#include "CEExecOptions.h"

#include "CppEphem.h"

/**********************************************************************//**
 * Define the command line options for this program
 *************************************************************************/
CEExecOptions DefineOpts()
{
    CEExecOptions opts("icrs2cirs");

    // Add version and description
    opts.AddProgramDescription(std::string() +
        "Converts from ICRS (solar system barycentric) coordinates to CIRS " +
        "(Earth centric) coordinates for a given Julian date.");

    // Set the options
    opts.AddIcrsPars();
    opts.AddJDPar();
    
    return opts;
}

/**********************************************************************//**
 * Print the results of the conversion
 *************************************************************************/
void PrintResults(const  CECoordinates& input,
                  const  CECoordinates& output,
                  double jd)
{
    std::vector<double> input_hms = CECoordinates::GetHMS( input.XCoordinate_Deg() ) ;
    std::vector<double> output_hms = CECoordinates::GetHMS( output.XCoordinate_Deg() ) ;
    
    std::printf("\n") ;
    std::printf("**********************************************\n") ;
    std::printf("* Results of ICRS -> CIRS                    *\n") ;
    std::printf("**********************************************\n") ;
    std::printf("CIRS Coordinates (output)\n") ;
    std::printf("    Julian Date    : %f\n", jd) ;
    std::printf("    Right Ascension: %02dh %02dm %04.1fs (%f deg)\n",
                int(output_hms[0]), int(output_hms[1]), output_hms[2]+output_hms[3],
                output.XCoordinate_Deg()) ;
    std::printf("    Declination    : %+f degrees\n", output.YCoordinate_Deg()) ;
    std::printf("ICRS Coordinates (input)\n") ;
    std::printf("    Right Ascension: %02dh %02dm %04.1fs (%f deg)\n",
                int(input_hms[0]), int(input_hms[1]), input_hms[2]+input_hms[3],
                input.XCoordinate_Deg()) ;
    std::printf("    Declination    : %+f degrees\n", input.YCoordinate_Deg()) ;
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
    CECoordinates input(CEAngle::Deg(opts.AsDouble("ra")), 
                        CEAngle::Deg(opts.AsDouble("dec")),
                        CECoordinateType::ICRS) ;
    
    // Get the coordinates as CIRS
    CECoordinates output = input.ConvertToCIRS(opts.AsDouble("juliandate")) ;
    
    // Print the result
    PrintResults(input, output, opts.AsDouble("juliandate")) ;

    return 0;
}

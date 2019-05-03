/***************************************************************************
 *  icrs2gal.cpp: CppEphem                                                 *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2017 JCardenzana                                           *
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
    opts.AddDoubleParam("r,ra", "ICRS right ascension for the conversion (degrees)",0.0) ;
    opts.AddDoubleParam("d,dec", "ICRS declination for the conversion (degrees)", 0.0) ;
    
    return opts;
}

/**********************************************************************//**
 * Print the results of the analysis
 *************************************************************************/
void PrintResults(CECoordinates& input,
                  CECoordinates& output)
{
    // Get the representation of the input icrs in hours, minutes, seconds
    std::vector<double> input_hms = CECoordinates::GetHMS( input.XCoordinate_Deg() ) ;
    
    std::printf("\n") ;
    std::printf("******************************************\n") ;
    std::printf("* Results of ICRS -> Galactic conversion *\n") ;
    std::printf("******************************************\n") ;
    std::printf("Galactic Coordinates (output)\n") ;
    std::printf("    Galactic Lon.: %f degrees\n", output.XCoordinate_Deg()) ;
    std::printf("    Galactic Lat.: %+f degrees\n", output.YCoordinate_Deg()) ;
    std::printf("ICRS Coordinates (input)\n") ;
    std::printf("    Right Ascension: %02dh %02dm %04.1fs (%f deg)\n",
                int(input_hms[0]), int(input_hms[1]), input_hms[2],
                input.XCoordinate_Deg()) ;
    std::printf("    Declination    : %+f degrees\n", input.YCoordinate_Deg()) ;
    std::printf("\n") ;
}

/**********************************************************************//**
 * Main method
 *************************************************************************/
int main(int argc, char** argv) {
    
    // Get the options from the command line
    CLOptions opts = DefineOpts() ;
    if (opts.ParseCommandLine(argc, argv)) return 0 ;
        
    // Create a map to store the results
    CECoordinates input(opts.AsDouble("ra"), opts.AsDouble("dec"),
                        CECoordinateType::ICRS, CEAngleType::DEGREES) ;
    
    // Convert the coordinates
    CECoordinates output = input.ConvertToGalactic();
    
    // Print the results
    PrintResults(input, output) ;
    
    return 0 ;
}

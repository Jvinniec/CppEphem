//
//  icrs2cirs.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 2/9/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

/** \file icrs2cirs.cpp
 * Executable for converting from ICRS to CIRS coordinates
 */

#include <stdio.h>
#include "CLOptions.h"

#include "CppEphem.h"

/**********************************************************************//**
 * Define the command line options for this program
 *************************************************************************/
CLOptions DefineOpts()
{
    CLOptions opts ;
    // Set the options
    opts.AddDoubleParam("r,RA", "ICRS right ascension for the conversion (degrees)",0.0) ;
    opts.AddDoubleParam("d,Dec", "ICRS declination for the conversion (degrees)", 0.0) ;
    opts.AddDoubleParam("j,JulianDate", "Julian date for the returned coordinates", CEDate::CurrentJD()) ;
    
    return opts;
}

/**********************************************************************//**
 * Print the results of the conversion
 *************************************************************************/
void PrintResults(CECoordinates& input,
                  CECoordinates& output,
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
    CLOptions opts = DefineOpts() ;
    if (opts.ParseCommandLine(argc, argv)) return 0 ;
    
    // Create a CECoordinates object
    CECoordinates input(opts.AsDouble("RA"), opts.AsDouble("Dec"),
                        CECoordinateType::ICRS, CEAngleType::DEGREES) ;
    
    // Get the coordinates as CIRS
    CECoordinates output = input.ConvertToCIRS(opts.AsDouble("JulianDate")) ;
    
    // Print the result
    PrintResults(input, output, opts.AsDouble("JulianDate")) ;
}
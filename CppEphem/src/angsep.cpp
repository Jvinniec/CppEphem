//
//  angsep.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/25/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

#include <stdio.h>

#include "CLOptions.h"
#include "CECoordinates.h"

/*********************************************
 * Forward declarations
 *********************************************/

CLOptions DefineOpts() ;

/*********************************************
 * MAIN
 *********************************************/
int main(int argc, char** argv)
{
    // Create and parse the command line options
    CLOptions opts = DefineOpts();
    if (opts.ParseCommandLine(argc,argv)) return 0;
    
    // Establish the input coordinate type
    CEAngleType in_ang_type  = CEAngleType::DEGREES;
    
    if (!opts.AsBool("InputDegrees")) {
        in_ang_type = CEAngleType::RADIANS;
    }
    
    // Find coordinates for the first object
    double angsep = CECoordinates::AngularSeparation(opts.AsDouble("xcoord1"), opts.AsDouble("ycoord1"),
                                                    opts.AsDouble("xcoord2"), opts.AsDouble("ycoord2"),
                                                    in_ang_type) ;
    
    // Figure out whether we need to convert the output angular separation
    if (opts.AsBool("InputDegrees") && !opts.AsBool("OutputDegrees")) {
        angsep *= DD2R;
    } else if (!opts.AsBool("InputDegrees") && opts.AsBool("OutputDegrees")) {
        angsep *= DR2D;
    }
    
    // Print the result
    std::cout << angsep << std::endl;
    
    return 0;
}

/*********************************************
 * Define the options available from the command line
 *********************************************/
CLOptions DefineOpts()
{
    CLOptions opts;
    
    // Add program description
    opts.AddProgramDescription(std::string("Computes the angular separation between two coordinates in either ICRS or GALACTIC. ") +
                               "Note that the coordinate types need to be the same for both positions.");
    
    // Specify the actual options
    opts.AddBoolParam("InputDegrees",
                      "Specifies whether input angles are in degrees (1) or radians (0)",
                      true);
    opts.AddBoolParam("OutputDegrees",
                      "Specifies whether to output the angular separation in degrees (1) or radians (0)",
                      true);
    opts.AddDoubleParam("xcoord1",
                        "X-coordinate for first position. Should be either 'right ascension' or 'galactic longitude'",
                        0.0);
    opts.AddDoubleParam("ycoord1",
                        "Y-coordinate for first position. Should be either 'declination' or 'galactic latitude'",
                        0.0);
    opts.AddDoubleParam("xcoord2",
                        "X-coordinate for second position. Should be either 'right ascension' or 'galactic longitude'",
                        0.0);
    opts.AddDoubleParam("ycoord2",
                        "Y-coordinate for second position. Should be either 'declination' or 'galactic latitude'",
                        0.0);
    
    return opts;
}

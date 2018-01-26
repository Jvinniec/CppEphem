//
//  jd2mjd.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/7/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include <stdio.h>
#include "CppEphem.h"

/**********************************************************************//**
 *************************************************************************/
int main(int argc, const char * argv[]) {
    // Check that we've been passed an actual argument
    if (argc < 2) {
        // Print some usage information
        std::cout << "\nUSAGE: jd2mjd <julian date>\n" ;
        std::cout << "RETURNED: Modified Julian Date\n\n" ;
        return 0 ;
    }
    
    // Convert the input variable to a double
    double jd = std::stod(std::string(argv[1])) ;
    
    // Print the resulting MJD value
    std::printf("%f\n", CEDate::JD2MJD(jd)) ;
    
    return 0 ;
}
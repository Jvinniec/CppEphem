//
//  mjd2jd.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/6/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include <iostream>
#include "CEDate.h"

int main(int argc, const char * argv[]) {
    // Check that we've been passed an actual argument
    if (argc < 2) {
        // Print some usage information
        std::cout << "\nUSAGE: mjd2jd <modified julian date>\n\n" ;
        return 0 ;
    }

    // Convert the input variable to a double
    double mjd = std::stod(std::string(argv[1])) ;
    
    std::cout << std::endl;
    std::cout << "\tModified Julian Date: " << mjd << std::endl;
    std::cout << "\tJulian Date: " << CEDate::MJD2JD(mjd) << std::endl;
    
    return 0 ;
}

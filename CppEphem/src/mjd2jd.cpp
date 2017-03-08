//
//  mjd2jd.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/6/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include <iostream>
#include "CppEphem.h"

//____________________________________________________________
int main(int argc, const char * argv[]) {
    // Check that we've been passed an actual argument
    if (argc < 2) {
        // Print some usage information
        std::cout << "\nUSAGE: mjd2jd <modified julian date>\n" ;
        std::cout << "RETURNED: Julian Date\n\n" ;
        return 0 ;
    }

    // Convert the input variable to a double
    double mjd = std::stod(std::string(argv[1])) ;
        
    std::printf("%f\n", CEDate::MJD2JD(mjd)) ;
    
    return 0 ;
}

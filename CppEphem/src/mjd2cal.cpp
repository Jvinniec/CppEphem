//
//  mjd2cal.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/7/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include <iostream>
#include "CppEphem.h"

/**********************************************************************//**
 *************************************************************************/
int main(int argc, const char * argv[]) {
    // Set the default return type ID
    int return_format(0) ;
    
    // Check that we've been passed an actual argument
    if (argc < 2) {
        // Print some usage information
        std::cout << "\nUSAGE: mjd2cal <modified julian date> <return format ID>\n" ;
        std::cout << "RETURNED: Gregorian calendar date in two formats:\n" ;
        std::cout << "   (default) ID=0: YYYYMMDD.<date fraction>\n" ;
        std::cout << "             ID=1: YYYY MM DD.<day fraction>" ;
        std::cout << "\n\n" ;
        return 0 ;
    } else if (argc == 3) {
        // Get the return format from the command line
        return_format = std::stoi(argv[2]) ;
    }
    
    // Convert the input variable to a double
    double mjd = std::stod(std::string(argv[1])) ;
    
    if (return_format == 1) {
        std::vector<double> gregorian_vect = CEDate::MJD2GregorianVect(mjd) ;
        std::printf("%4.0f %02.0f %06.4f\n",
                    gregorian_vect[0], gregorian_vect[1], gregorian_vect[2]+gregorian_vect[3]) ;
    } else {
        std::printf("%f\n", CEDate::MJD2Gregorian(mjd)) ;
    }
    
    return 0 ;
}
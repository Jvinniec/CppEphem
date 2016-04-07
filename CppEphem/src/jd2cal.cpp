//
//  jd2cal.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/7/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include <iostream>
#include "CEDate.h"

int main(int argc, const char * argv[]) {
    // Set the default return type ID
    int return_format(1) ;
    
    // Check that we've been passed an actual argument
    if (argc < 2) {
        // Print some usage information
        std::cout << "\nUSAGE: jd2cal <julian date> <return format ID>\n" ;
        std::cout << "RETURNED: Gregorian calendar date in one of two formats:\n" ;
        std::cout << "      ID=1: YYYYMMDD.<date fraction>\n" ;
        std::cout << "      ID=2: Year Month Day.<day fraction>" ;
        std::cout << "\n\n" ;
        return 0 ;
    } else if (argc == 3) {
        // Get the return format from the command line
        return_format = std::stoi(argv[2]) ;
    }
    
    // Convert the input variable to a double
    double jd = std::stod(std::string(argv[1])) ;
    
    if (return_format == 2) {
        std::vector<double> gregorian_vect = CEDate::JD2GregorianVect(jd) ;
        std::printf("%4.0f %02.0f %06.4f\n",
                    gregorian_vect[0], gregorian_vect[1], gregorian_vect[2]+gregorian_vect[3]) ;
    } else {
        std::printf("%f\n", CEDate::JD2Gregorian(jd)) ;
    }
    
    return 0 ;
}
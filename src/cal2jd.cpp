//
//  cal2jd.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/7/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include <iostream>
#include <cmath>
#include "CppEphem.h"

int main(int argc, const char * argv[]) {
    
    // variable to hold the julian date
    double jd(0.0) ;
    
    // Check that we've been passed an actual argument
    if ((argc != 2)&&(argc != 4)) {
        // Print some usage information
        std::cout << "\nUSAGE: May be called in one of two ways:\n";
        std::cout << "    1: cal2jd YYYYMMDD.<day fraction>\n" ;
        std::cout << "    2: cal2jd <year> <month> <day>.<day fraction>\n" ;
        std::cout << "RETURNED: Julian Date\n\n" ;
        return 0 ;
    } else if (argc == 2) {
        jd = CEDate::Gregorian2JD(std::stod(argv[1])) ;
    } else if (argc == 4) {
        // First get the year, month, day information from the pass parameters
        double year(std::stod(argv[1])), month(std::stod(argv[2])), day(std::stod(argv[3])) ;
        std::vector<double> gregorian_vect = {year, month, std::floor(day), 0.0} ;
        gregorian_vect[3] = day - gregorian_vect[2] ;
                
        // Now compute the julian date
        jd = CEDate::GregorianVect2JD(gregorian_vect) ;
    }
    
    // Print the result
    std::printf("%f\n", jd) ;
    
    return 0 ;
}

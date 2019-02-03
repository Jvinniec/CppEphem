/***************************************************************************
 *  cal2jd.cpp: CppEphem                                                   *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016 JCardenzana                                           *
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

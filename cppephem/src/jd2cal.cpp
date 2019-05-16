/***************************************************************************
 *  jd2cal.cpp: CppEphem                                                   *
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
#include "CppEphem.h"

/**********************************************************************//**
 *************************************************************************/
int main(int argc, const char * argv[]) {
    // Set the default return type ID
    int return_format(0) ;
    
    // Check that we've been passed an actual argument
    if (argc < 2) {
        // Print some usage information
        std::cout << "jd2cal v" << CPPEPHEM_VERSION << "\n";
        std::cout << "\nUSAGE: jd2cal <julian date> <return format ID>\n" ;
        std::cout << "RETURNED: Gregorian calendar date in one of two formats:\n" ;
        std::cout << "   (default) ID=0: YYYYMMDD.<date fraction>\n" ;
        std::cout << "             ID=1: YYYY MM DD.<day fraction>" ;
        std::cout << "\n\n" ;
        return 0 ;
    } else if (argc == 3) {
        // Get the return format from the command line
        return_format = std::stoi(argv[2]) ;
    }
    
    // Convert the input variable to a double
    double jd = std::stod(std::string(argv[1])) ;
    
    if (return_format == 1) {
        std::vector<double> gregorian_vect = CEDate::JD2GregorianVect(jd) ;
        std::printf("%4.0f %02.0f %06.4f\n",
                    gregorian_vect[0], gregorian_vect[1], gregorian_vect[2]+gregorian_vect[3]) ;
    } else {
        std::printf("%f\n", CEDate::JD2Gregorian(jd)) ;
    }
    
    return 0 ;
}

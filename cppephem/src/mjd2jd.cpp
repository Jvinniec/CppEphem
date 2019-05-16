/***************************************************************************
 *  mjd2jd.cpp: CppEphem                                                   *
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
    // Check that we've been passed an actual argument
    if (argc < 2) {
        // Print some usage information
        std::cout << "mjd2jd v" << CPPEPHEM_VERSION << "\n";
        std::cout << "\nUSAGE: mjd2jd <modified julian date>\n" ;
        std::cout << "RETURNED: Julian Date\n\n" ;
        return 0 ;
    }

    // Convert the input variable to a double
    double mjd = std::stod(std::string(argv[1])) ;
        
    std::printf("%f\n", CEDate::MJD2JD(mjd)) ;
    
    return 0 ;
}

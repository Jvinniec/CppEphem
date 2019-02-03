/***************************************************************************
 *  test_cpephem.cpp: CppEphem                                             *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2018 JCardenzana                                           *
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

#include <exception>
#include <vector>
#include "test_CEDate.h"

void register_tests(std::vector<CETestSuite*> &tests)
{
    // Add test objects to tests list
    tests.push_back(new test_CEDate());
    
    return;
}


int main(int argc, char** argv) 
{
    // Variable if tests are passed
    bool fail = false;

    // Get the tests
    std::vector<CETestSuite*> tests;
    register_tests(tests);

    // Run the individual tests
    for (int i=0; i<tests.size(); i++) {
        // Make sure that other tests are still able to run
        // if one of the tests throws an exception
        try {
            fail = fail || tests[i]->runtests();
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            fail = false;
        }

        if (tests[i] != nullptr) {
            delete tests[i];
            tests[i] = nullptr;
        }
    }

    return fail;
}
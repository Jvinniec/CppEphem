/***************************************************************************
 *  test_CETime.cpp: CppEphem                                              *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2019 JCardenzana                                           *
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

#include <stdlib.h>
#include <thread>
#include <iostream>
#include "test_CETime.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CETime::test_CETime() :
    CETestSuite()
{
    // Create the base objects for comparison
    base_    = CETime(123456.789, CETimeType::UTC);
    naptime_ = std::chrono::milliseconds(500);

    SetDblTol(1e-11);
    // pause execution to give the the date time to run
    //std::this_thread::sleep_for(naptime_);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CETime::~test_CETime()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CETime::runtests()
{
    std::cout << "\nTesting CETime:\n";

    // Run each of the tests
    test_construct();

    return pass();
}


/**********************************************************************//**
 * Test constructors
 * 
 * @return whether the tests succeed
 *************************************************************************/
bool test_CETime::test_construct(void)
{
    // Vectors containing the same information
    double              timeA = 123456.789;       // 12:34:56.789
    std::vector<double> timeB = {12.0, 34.0, 56.0, 0.789};

    // Create a comparison running date object that should be different
    CETime test1(timeA, CETimeType::UTC);
    test_double(test1.Hour(), timeB[0], __func__, __LINE__);
    test_double(test1.Min(), timeB[1], __func__, __LINE__);
    test_double(test1.Sec(), timeB[2]+timeB[3], __func__, __LINE__);
    std::cout << test1.Sec() - (timeB[2]+timeB[3]) << std::endl;

    // Copy constructor
    CETime test2(timeB, CETimeType::UTC);
    test_double(test2.Hour(), timeB[0], __func__, __LINE__);
    test_double(test2.Min(), timeB[1], __func__, __LINE__);
    test_double(test2.Sec(), timeB[2]+timeB[3], __func__, __LINE__);

    // Default constructor

    return pass();
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) 
{
    test_CETime tester;
    return (!tester.runtests());
}

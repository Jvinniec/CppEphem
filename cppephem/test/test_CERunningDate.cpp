/***************************************************************************
 *  test_CERunningDate.cpp: CppEphem                                       *
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
#include "test_CERunningDate.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CERunningDate::test_CERunningDate() :
    CETestSuite()
{
    // Create the base objects for comparison
    base_date_ = CEDate();
    base_ = CERunningDate();
    naptime_ = std::chrono::milliseconds(500);

    // pause execution to give the the date time to run
    std::this_thread::sleep_for(naptime_);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CERunningDate::~test_CERunningDate()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CERunningDate::runtests()
{
    std::cout << "\nTesting CERunningDate:\n";

    // Run each of the tests
    test_construct();

    return pass();
}


/**********************************************************************//**
 * Test constructors
 * 
 * @return whether the tests succeed
 *************************************************************************/
bool test_CERunningDate::test_construct(void)
{
    // Create a comparison running date object that should be different
    CERunningDate test1(base_);

    // Default constructor
    test(base_date_.JD() != base_.JD(), __func__, __LINE__);

    // These should give the same value
    test_double(test1.JD(), base_.JD(), __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) 
{
    test_CERunningDate tester;
    return (!tester.runtests());
}

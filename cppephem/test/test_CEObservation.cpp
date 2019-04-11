/***************************************************************************
 *  test_CEObservation.cpp: CppEphem                                       *
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

/** \class test_CEObservation
 Defines tests for CEObservation
 */

#include <iostream>

#include "test_CEObservation.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CEObservation::test_CEObservation() :
    CETestSuite()
{
    base_observer_ = CEObserver(-110.952, 31.6751, 1268.0);
    base_date_ = CEDate(CppEphem::julian_date_J2000(), CEDateType::JD);
    base_body_ = CEBody();
    base_obs_ = CEObservation(&base_observer_, &base_body_, &base_date_);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CEObservation::~test_CEObservation()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEObservation::runtests()
{
    std::cout << "\nTesting CEObservation:\n";

    // Run each of the tests
    test_obj_return();

    return pass();
}


/**********************************************************************//**
 * Tests return values for temperature at sea level
 *  @return Status of tests
 *************************************************************************/
bool test_CEObservation::test_obj_return(void)
{
    // Try to return the body object

    return pass();
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) 
{
    test_CEObservation tester;
    return (!tester.runtests());
}

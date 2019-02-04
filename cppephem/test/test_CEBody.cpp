/***************************************************************************
 *  test_CEBody.cpp: CppEphem                                              *
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

#include "test_CEBody.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CEBody::test_CEBody() :
    CETestSuite()
{
    // Let's use the Crab Nebula
    base_ = CEBody("Crab", 83.633, 22.0145, 
                   CECoordinateType::ICRS, 
                   CEAngleType::DEGREES);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CEBody::~test_CEBody()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEBody::runtests()
{
    std::cout << "\nTesting CEBody:\n";

    // Run each of the tests
    test_Name();
    test_GetCoordinates();

    return pass();
}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEBody::test_Name(void)
{
    // Check that we can get the name
    test_string(base_.Name(), "Crab", __func__, __LINE__);

    // Check that we can set the name
    CEBody test_body(base_);
    std::string new_name = "NotCrab";
    test_body.SetName(new_name);
    test_string(test_body.Name(), new_name, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEBody::test_GetCoordinates(void)
{
    // Test getting the coordinates
    CECoordinates test_coords = base_.GetCoordinates();
    
    // Test that the coordinates are equal
    test_bool(test_coords == base_, true, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) {
    test_CEBody tester;
    return (!tester.runtests());
}
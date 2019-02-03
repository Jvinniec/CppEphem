/***************************************************************************
 *  test_CECoordinates.cpp: CppEphem                                       *
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

#include "test_CECoordinates.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CECoordinates::test_CECoordinates() :
    CETestSuite()
{
    // Lets use the Crab Nebula (M1) for our testing
    base_ = CECoordinates(83.633, 22.0145, 
                          CECoordinateType::ICRS, 
                          CEAngleType::DEGREES);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CECoordinates::~test_CECoordinates()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CECoordinates::runtests()
{
    std::cout << "Testing CEDate:\n";

    // Run each of the tests
    update_pass(test_SetCoord_Icrs());
    update_pass(test_SetCoord_Cirs());
    update_pass(test_SetCoord_Galactic());

    return pass();
}


/**********************************************************************//**
 * Test ability to set coordinates as CIRS
 *************************************************************************/
bool test_CECoordinates::test_SetCoord_Cirs()
{
    // Get the CIRS coordinates from the base_coord_ object
    CECoordinates base_cirs = base_.ConvertToCIRS(CppEphem::julian_date_J2000());

    // Create a new coordinates from the values passed by test_coord
    CECoordinates test_coord;
    test_coord.SetCoordinates(base_cirs.XCoordinate_Deg(), 
                              base_cirs.YCoordinate_Deg(),
                              CECoordinateType::CIRS, 
                              CEAngleType::DEGREES);
    
    // Compare the coordinates
    return test_bool((test_coord == base_cirs), true);
}


/**********************************************************************//**
 * Test ability to set coordinates as ICRS
 *************************************************************************/
bool test_CECoordinates::test_SetCoord_Icrs()
{
    // Create a new coordinates from the values passed by test_coord
    CECoordinates test_coord;
    test_coord.SetCoordinates(base_.XCoordinate_Deg(), 
                              base_.YCoordinate_Deg(),
                              CECoordinateType::ICRS, 
                              CEAngleType::DEGREES);

    // Compare the coordinates
    return test_bool((test_coord == base_), true);
}

/**********************************************************************//**
 * Test ability to set coordinates as Galactic
 *************************************************************************/
bool test_CECoordinates::test_SetCoord_Galactic()
{
    // Get the CIRS coordinates from the base_coord_ object
    CECoordinates base_galactic = base_.ConvertToGalactic(CppEphem::julian_date_J2000());

    // Create a new coordinates from the values passed by test_coord
    CECoordinates test_coord;
    test_coord.SetCoordinates(base_galactic.XCoordinate_Deg(), 
                              base_galactic.YCoordinate_Deg(),
                              CECoordinateType::CIRS, 
                              CEAngleType::DEGREES);
    
    // Compare the coordinates
    return test_bool((test_coord == base_galactic), true);
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) {
    test_CECoordinates tester;
    return tester.runtests();
}
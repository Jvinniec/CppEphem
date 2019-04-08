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

#include "test_CEPlanet.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CEPlanet::test_CEPlanet() :
    CETestSuite()
{
    // Let's use the Crab Nebula
    base_ = CEPlanet("Crab", 83.633, 22.0145, 
                   CECoordinateType::ICRS, 
                   CEAngleType::DEGREES);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CEPlanet::~test_CEPlanet()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEPlanet::runtests()
{
    std::cout << "\nTesting CEPlanet:\n";

    // Run each of the tests
    test_construct();

    return pass();
}


/**********************************************************************//**
 * Test constructors
 * 
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_construct(void)
{
    CECoordinates test_coord;

    // Default constructor
    CEPlanet test1;
    test_string(test1.Name(), "undefined", __func__, __LINE__);
    test(test1.GetCoordinates() == test_coord, __func__, __LINE__);

    // Default constructors
    CEPlanet test2("DefaultPlanet", 123.45, 67.89, CECoordinateType::ICRS, CEAngleType::DEGREES);
    test_string(test2.Name(), "DefaultPlanet", __func__, __LINE__);
    test_double(test2.XCoordinate_Deg(), 123.45, __func__, __LINE__);
    test_double(test2.YCoordinate_Deg(), 67.89, __func__, __LINE__);
    test_int(int(test2.GetCoordSystem()), int(CECoordinateType::ICRS), __func__, __LINE__);

    // Copy constructor
    test1.SetName("DefaultPlanet");
    test1.SetCoordinates(123.45, 67.89, CECoordinateType::ICRS, CEAngleType::DEGREES);
    CEPlanet test3(test1);
    test_Planet(test1, test3, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Compare 
 * 
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_Planet(const CEPlanet& planet1, 
                                const CEPlanet& planet2,
                                const std::string& func,
                                const int&         line)
{
    // Consider them equal if they have the same name and coordinates
    if ((planet1.Name() == planet2.Name()) &&
        (planet1.GetCoordinates() == planet2.GetCoordinates())) {
        log_success("Planet objects ARE the same", func, line);
        update_pass(true);
    } else {
        log_failure("Planet objects are NOT the same", func, line);
        update_pass(false);
    }

    return pass();
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) {
    test_CEPlanet tester;
    return (!tester.runtests());
}

/***************************************************************************
 *  test_CENamespace.cpp: CppEphem                                         *
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

/** \class test_CENamespace
 Defines tests for CENamespace
 */

#include <iostream>

#include "test_CENamespace.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CENamespace::test_CENamespace() :
    CETestSuite()
{}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CENamespace::~test_CENamespace()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CENamespace::runtests()
{
    std::cout << "\nTesting CENamespace:\n";

    // Run each of the tests
    test_SeaLevelVals();
    test_Conversions();
    test_Corrections();

    return pass();
}


/**********************************************************************//**
 * Tests return values for temperature at sea level
 *  @return Status of tests
 *************************************************************************/
bool test_CENamespace::test_SeaLevelVals(void)
{
    // Check the temperature at sea level
    double sea_level_temp_K = 288.2;
    test_double(CppEphem::SeaLevelTemp_K(), sea_level_temp_K, __func__, __LINE__);

    // Convert 
    double sea_level_temp_C = sea_level_temp_K - 273.15;
    test_double(CppEphem::SeaLevelTemp_C(), sea_level_temp_C, __func__, __LINE__);

    double sea_level_temp_F = (9.0/5.0) * sea_level_temp_C + 32.0;
    test_double(CppEphem::SeaLevelTemp_F(), sea_level_temp_F, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Tests return values for basic unit conversion constants
 *  @return Status of tests
 *************************************************************************/
bool test_CENamespace::test_Conversions()
{
    // Test julian date 2000
    test_double(CppEphem::julian_date_J2000(), DJ00, __func__, __LINE__);

    // Speed of light (m/sec)
    test_double(CppEphem::c(), DC * DAU / DAYSEC, __func__, __LINE__);

    // Speed of light (AU/day)
    test_double(CppEphem::c_au_per_day(), DC, __func__, __LINE__);

    // Meters per AU
    test_double(CppEphem::m_per_au(), DAU, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Tests basic coordinate conversion correction values
 *  @return Status of tests
 *************************************************************************/
bool test_CENamespace::test_Corrections()
{
    // Get dut1 for J2000
    test_double(CppEphem::dut1(), 0.0, __func__, __LINE__);

    // Get dut1 error for J2000 date
    test_double(CppEphem::dut1Error(), 0.0, __func__, __LINE__);

    // Test x,y polar motion
    test_double(CppEphem::xp(), 0.0, __func__, __LINE__);
    test_double(CppEphem::yp(), 0.0, __func__, __LINE__);

    // Estimate altitude/pressure based on pressure
    test_double(CppEphem::EstimateAltitude_m(1013.25), 0.0, __func__, __LINE__);
    test_double(CppEphem::EstimatePressure_hPa(0.0), 1013.25, __func__, __LINE__);

    return pass();
}

/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) {
    test_CENamespace tester;
    return (!tester.runtests());
}
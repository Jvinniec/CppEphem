/***************************************************************************
 *  CETestSuite.h: CppEphem                                                *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2017 JCardenzana                                           *
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

#include <stdio.h>
#include <cmath>
#include "CETestSuite.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
CETestSuite::CETestSuite()
{}




/**********************************************************************//**
 * Default destructor
 *************************************************************************/
CETestSuite::~CETestSuite()
{}


/**********************************************************************//**
 * Return whether value is within a specified tolerance of 'expected'
 *
 * @param[in] value         Value to be tested
 * @param[in] expected      Expected value to test against
 * @param[in] tol           Tolerance for the comparison
 * @return Whether value is within 'tol' of expected
 *************************************************************************/
bool CETestSuite::test_double(const double& value, 
                              const double& expected,
                              const double& tol)
{
    double relative_tol = expected * tol;
    if (std::fabs(value - expected) < (expected*tol)) {
        std::printf("DOUBLE value of %f is within tolerance of %f of expected value %f.\n",
                    value, relative_tol, expected);
        return true;
    } else {
        std::printf("DOUBLE value of %f is NOT within tolerance of %f of expected value %f.\n",
                    value, relative_tol, expected);
        return false;
    }
}


/**********************************************************************//**
 * Return whether an int is within a specified tolerance of 'expected'
 *
 * @param[in] value         Value to be tested
 * @param[in] expected      Expected value to test against
 * @param[in] tol           Tolerance for the comparison
 * @return Whether value is within 'tol' of expected
 *************************************************************************/
bool CETestSuite::test_int(const int& value, const int& expected,
                                  const int& tol)
{
    int relative_tol = expected * tol;
    if (std::abs(value - expected) < (expected * tol)) {
        std::printf("INT value of %d is within tolerance of %d of expected value %d.\n",
                    value, relative_tol, expected);
        return true;
    } else {
        std::printf("INT value of %d is NOT within tolerance of %d of expected value %d.\n",
                    value, relative_tol, expected);
        return false;
    }
}


/**********************************************************************//**
 * Return whether a boolean is equal to an expected value
 *
 * @param[in] value         Value to be tested
 * @param[in] expected      Expected value to test against
 * @return Whether value and expected are the same
 *************************************************************************/
bool CETestSuite::test_bool(bool value, bool expected)
{
    if (value == expected) {
        std::printf("BOOLEAN value of %d is equal to expected value %d.\n",
                    value, expected);
        return true;
    } else {
        std::printf("BOOLEAN value of %d is NOT equal to expected value %d.\n",
                    value, expected);
        return false;
    }
}


/**********************************************************************//**
 * Return whether a string is equal to an expected value
 *
 * @param[in] value         Value to be tested
 * @param[in] expected      Expected value to test against
 * @return Whether value and expected are the same
 *************************************************************************/
bool CETestSuite::test_string(const std::string& value, const std::string& expected)
{
    if (value.compare(expected) == 0) {
        std::printf("STRING value of %s is equal to expected value %s.\n",
                    value.c_str(), expected.c_str());
        return true;
    } else {
        std::printf("STRING value of %s is NOT equal to expected value %s.\n",
                    value.c_str(), expected.c_str());
        return false;
    }
}

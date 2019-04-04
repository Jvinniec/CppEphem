/***************************************************************************
 *  CETestSuite.h: CppEphem                                                *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2017-2019 JCardenzana                                      *
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

/** \class CETestSuite
 CETestSuite defines the methods that test values to make sure they are
 equivalent to the values that are expected. This is the base class for
 all test classes.
 */

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
bool CETestSuite::test_double(const double&      value, 
                              const double&      expected,
                              const std::string& function,
                              const int&         line)
{
    // Return status
    bool   ret_val      = true;
    double relative_tol = expected * tol_dbl_;

    // Values equal within tolerance
    if (std::fabs(value - expected) <= relative_tol) {
        log_success("DOUBLE value of "+std::to_string(value)+" is within " +
                    "tolerance of "+std::to_string(relative_tol)+" of " +
                    "expected value "+std::to_string(expected)+".",
                    function, line);
        ret_val = true;
    } 
    // Values not within tolerance
    else {
        log_failure("DOUBLE value of "+std::to_string(value)+" is NOT within " +
                    "tolerance of "+std::to_string(relative_tol)+" of " +
                    "expected value "+std::to_string(expected)+".",
                    function, line);
        ret_val = false;
    }

    update_pass(ret_val);
    return ret_val;
}


/**********************************************************************//**
 * Return whether an int is within a specified tolerance of 'expected'
 *
 * @param[in] value         Value to be tested
 * @param[in] expected      Expected value to test against
 * @param[in] tol           Tolerance for the comparison
 * @return Whether value is within 'tol' of expected
 *************************************************************************/
bool CETestSuite::test_int(const int&         value, 
                           const int&         expected,
                           const std::string& function,
                           const int&         line)
{
    // Return status
    bool ret_val      = true;
    int  relative_tol = expected * tol_int_;
    if (std::abs(value - expected) <= relative_tol) {
        log_success("INT value of "+std::to_string(value)+" is within " +
                    "tolerance of "+std::to_string(relative_tol)+" of " +
                    "expected value "+std::to_string(expected)+".",
                    function, line);
        ret_val = true;
    } else {
        log_failure("INT value of "+std::to_string(value)+" is NOT within " +
                    "tolerance of "+std::to_string(relative_tol)+" of " +
                    "expected value "+std::to_string(expected)+".",
                    function, line);
        ret_val = false;
    }

    update_pass(ret_val);
    return ret_val;
}


/**********************************************************************//**
 * Return whether a boolean is equal to an expected value
 *
 * @param[in] value         Value to be tested
 * @param[in] expected      Expected value to test against
 * @return Whether value and expected are the same
 *************************************************************************/
bool CETestSuite::test_bool(bool               value, 
                            bool               expected,
                            const std::string& function,
                            const int&         line)
{
    bool ret_val = true;
    if ((value && expected) || (!value && !expected)) {
        log_success("BOOLEAN value of "+std::to_string(value)+" is equal " +
                    "to expected value " +std::to_string(expected)+ ".",
                    function, line);
        ret_val = true;
    } else {
        log_failure("BOOLEAN value of "+std::to_string(value)+" is NOT equal to " +
                    "to expected value " +std::to_string(expected)+ ".",
                    function, line);
        ret_val = false;
    }

    update_pass(ret_val);
    return ret_val;
}


/**********************************************************************//**
 * Return whether a string is equal to an expected value
 *
 * @param[in] value         Value to be tested
 * @param[in] expected      Expected value to test against
 * @return Whether value and expected are the same
 *************************************************************************/
bool CETestSuite::test_string(const std::string& value, 
                              const std::string& expected,
                              const std::string& function,
                              const int&         line)
{
    bool ret_val = true;
    if (value.compare(expected) == 0) {
        log_success("STRING value of \""+value+"\" is equal to expected " +
                    "value \""+expected+"\".",
                    function, line);
        ret_val = true;
    } else {
        log_failure("STRING value of \""+value+"\" is NOT equal to expected " +
                    "value \""+expected+"\".",
                    function, line);
        ret_val = false;
    }

    update_pass(ret_val);
    return ret_val;
}


/**********************************************************************//**
 * Return whether two vectors contain equivalent values
 *
 * @param[in] value         Value to be tested
 * @param[in] expected      Expected value to test against
 * @return Whether value and expected are the same
 *************************************************************************/
template<class T>
bool CETestSuite::test_vect_(const std::vector<T>& value,
                             const std::vector<T>& expected,
                             const std::string& function,
                             const int&         line)
{
    bool isMatch = true;
    if (value.size() == expected.size()) {
        for (int i=0; i<value.size(); i++) {
            if (value[i] != expected[i]) {
                log_failure("VECTOR values at index "+std::to_string(i)+" "+
                            "are NOT equal.", function, line);
                isMatch = false;
            }
        }

        // If there is a match, then we consider the vectors to be equal
        if (isMatch) {
            log_success("VECTOR values and lengths ARE equal.", function, line);
        }
    } else {
        log_failure("VECTOR lengths are NOT equal.", function, line);
    }

    update_pass(isMatch);
    return isMatch;
}


/**********************************************************************//**
 * Return whether two vector<double>'s contain equivalent values
 *
 * @param[in] value         Value to be tested
 * @param[in] expected      Expected value to test against
 * @return Whether value and expected are the same
 *************************************************************************/
bool CETestSuite::test_vect(const std::vector<double>& value,
                            const std::vector<double>& expected,
                            const std::string&         function,
                            const int&                 line)
{
    return test_vect_<double>(value, expected, function, line);
}


/**********************************************************************//**
 * Responsible for cleaning up anything that needs cleanup
 *************************************************************************/
void CETestSuite::cleanup(void)
{
    return;
}


/**********************************************************************//**
 * Returns the status of the 'pass' variable
 *************************************************************************/
bool CETestSuite::pass(void)
{
    return pass_;
}


/**********************************************************************//**
 * Returns whether the analysis has succeeded or not
 *  @param[in] test_passed      Specifies whether the test has passed
 *************************************************************************/
void CETestSuite::update_pass(const bool& test_passed)
{
    pass_ = pass_ && test_passed;
}


/**********************************************************************//**
 * Prints a message identifying that a test has passed
 *  @param[in] message      Message to be printed
 *  @param[in] function     Name of the function containing the test
 *  @param[in] line         Line number of the testing file
 *************************************************************************/
void CETestSuite::log_success(const std::string& message,
                              const std::string& function,
                              const int&         line)
{
    std::printf("- Fnc: %s, ln %d\n", function.c_str(), line);
    std::printf("  [x] %s\n", message.c_str());
}


/**********************************************************************//**
 * Prints a message identifying that a test has NOT passed
 *  @param[in] message      Message to be printed
 *  @param[in] function     Name of the function containing the test
 *  @param[in] line         Line number of the testing file
 *************************************************************************/
void CETestSuite::log_failure(const std::string& message,
                              const std::string& function,
                              const int&         line)
{
    std::printf("- Fnc: %s, ln %d\n", function.c_str(), line);
    std::printf("  [ ] %s\n", message.c_str());
}
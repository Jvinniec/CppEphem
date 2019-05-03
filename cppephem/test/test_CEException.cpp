/***************************************************************************
 *  test_CEException.cpp: CppEphem                                         *
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

#include "test_CEException.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CEException::test_CEException() :
    CETestSuite()
{}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CEException::~test_CEException()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEException::runtests()
{
    std::cout << "\nTesting test_CEException:\n";

    // Run each of the tests
    test_invalid_value();
    test_construct();
    test_sofa_error();
    test_sofa_exception();
    test_corr_file_except();

    return pass();
}


/**********************************************************************//**
 * Try constructing and accesing information about a CEException
 *  @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEException::test_construct(void)
{
    // Create a message
    std::string msg = "Testing throwing 'CEException::invalid_value'";

    // Try throwing
    try {
        throw CEException::invalid_value("test_CEException::test_invalid_value()",
                                         msg);
        // If we get here there's a problem
        log_failure("CEException::invalid_value failed to throw", __func__, __LINE__);
        update_pass(false);
    } catch (CEException::invalid_value & e) {
        // This is where we should be
        log_success("CEException::invalid_value thrown successfully", __func__, __LINE__);
        update_pass(true);

        // Test that we can access the message
        std::string test_msg(e.message());
        test_string(test_msg, msg, __func__, __LINE__);

        // Test that the backtrace is in fact filled
        std::string test_trace(e.trace());
        test_greaterthan(test_trace.size(), 1, __func__, __LINE__);

        // Make sure the combined output exceeds both the message and trace
        std::string test_what(e.what());
        std::cout << e.what() << std::endl;
        test_greaterthan(test_what.size(), test_msg.size()+test_trace.size(), __func__, __LINE__);

        // See if we can append to the exception
        std::string msg2(" ... now with more text!");
        e.message_append(msg2);
        test_msg = std::string(e.message());
        test_string(test_msg, msg+msg2, __func__, __LINE__);
    }

    return pass();
}


/**********************************************************************//**
 * Try throwing 'CEException::invalid_value'
 *  @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEException::test_invalid_value(void)
{
    // Try throwing
    try {
        throw CEException::invalid_value("test_CEException::test_invalid_value()",
                                         "Testing throwing 'CEException::invalid_value'");
        // If we get here there's a problem
        log_failure("CEException::invalid_value failed to throw", __func__, __LINE__);
        update_pass(false);
    } catch (CEException::invalid_value & e) {
        // This is where we should be
        log_success("CEException::invalid_value thrown successfully", __func__, __LINE__);
        update_pass(true);
    }

    return pass();
}


/**********************************************************************//**
 * Try throwing 'CEException::sofa_errcode'
 *  @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEException::test_sofa_error(void)
{
    // Try throwing
    try {
        // TODO: find a sofa method that can be configured to return a bad error code
        throw CEException::sofa_error("test_CEException::test_sofa_error()",
                                      "No method currently checked",
                                      1, "Testing throwing 'CEException::sofa_error");
        // If we get here there's a problem
        log_failure("CEException::sofa_error failed to throw", __func__, __LINE__);
        update_pass(false);
    } catch (CEException::sofa_error & e) {
        // This is where we should be
        log_success("CEException::sofa_error thrown successfully", __func__, __LINE__);
        update_pass(true);
    }

    return pass();
}


/**********************************************************************//**
 * Try throwing 'CEException::sofa_exception'
 *  @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEException::test_sofa_exception(void)
{
    // Try throwing
    try {
        // TODO: find a sofa method that can be configured to return a bad error code
        throw CEException::sofa_exception("test_CEException::test_sofa_exception()",
                                      "No method currently checked",
                                      "Testing throwing 'CEException::sofa_exception'");
        // If we get here there's a problem
        log_failure("CEException::sofa_exception failed to throw", __func__, __LINE__);
        update_pass(false);
    } catch (CEException::sofa_exception & e) {
        // This is where we should be
        log_success("CEException::sofa_exception thrown successfully", __func__, __LINE__);
        update_pass(true);
    }

    return pass();
}


/**********************************************************************//**
 * Try throwing 'CEException::sofa_exception'
 *  @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEException::test_corr_file_except(void)
{
    // Try throwing
    try {
        // TODO: find a sofa method that can be configured to return a bad error code
        throw CEException::corr_file_load_error("test_CEException::test_corr_file_except()",
                                                "Testing throwing 'CEException::corr_file_load_error'");
        // If we get here there's a problem
        log_failure("CEException::corr_file_load_error failed to throw", __func__, __LINE__);
        update_pass(false);
    } catch (CEException::corr_file_load_error & e) {
        // This is where we should be
        log_success("CEException::corr_file_load_error thrown and caught successfully", __func__, __LINE__);
        update_pass(true);
    }

    return pass();
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) 
{
    test_CEException tester;
    return (!tester.runtests());
}

/***************************************************************************
 *  test_CEDate.cpp: CppEphem                                              *
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

#include "test_CEDate.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CEDate::test_CEDate() :
    CETestSuite()
{
    base_date_.SetDate(CppEphem::julian_date_J2000());
    test_date_.SetDate(base_date_);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CEDate::~test_CEDate()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEDate::runtests()
{
    std::cout << "Testing CEDate: ";

    // Run each of the tests
    update_pass(test_SetDate_JD());
    update_pass(test_SetDate_MJD());
    update_pass(test_Gregorian());
    update_pass(test_ReturnType());

    return pass();
}


/**********************************************************************//**
 * Test ability to set julian date
 *************************************************************************/
bool test_CEDate::test_SetDate_JD()
{
    test_date_.SetDate(base_date_.JD(), CEDateType::MJD);
    return test_double(test_date_.JD(), base_date_.JD());
}

/**********************************************************************//**
 * Test ability to set modified julian date
 *************************************************************************/
bool test_CEDate::test_SetDate_MJD()
{
    test_date_.SetDate(base_date_.MJD(), CEDateType::MJD);
    return test_double(test_date_.MJD(), base_date_.MJD());
}


/**********************************************************************//**
 * Test ability to set Gregorian date
 *************************************************************************/
bool test_CEDate::test_Gregorian()
{
    test_date_.SetDate(base_date_.Gregorian(), CEDateType::GREGORIAN);
    update_pass(test_double(test_date_.Gregorian(), base_date_.Gregorian()));

    // Test getting the year, month, day, and day fraction
    update_pass(test_int(test_date_.Year(), base_date_.Year()));
    update_pass(test_int(test_date_.Month(), base_date_.Month()));
    update_pass(test_int(test_date_.Day(), base_date_.Day()));
    update_pass(test_double(test_date_.DayFraction(), base_date_.DayFraction()));

    // Test getting hte Gregorian date as a vector
    update_pass(test_vect(test_date_.GregorianVect(), base_date_.GregorianVect()));

    return pass();
}


/**********************************************************************//**
 * Test ability set the return type
 *************************************************************************/
bool test_CEDate::test_ReturnType(void)
{
    // Reset the date
    test_date_.SetDate(base_date_.JD(), CEDateType::JD);

    // Set the return type to JD
    test_date_.SetReturnType(CEDateType::JD);
    update_pass(test_double(test_date_, base_date_.JD()));
    
    // Set the return type to MJD
    test_date_.SetReturnType(CEDateType::MJD);
    update_pass(test_double(test_date_, base_date_.MJD()));

    // Set the return type to Gregorian
    test_date_.SetReturnType(CEDateType::GREGORIAN);
    update_pass(test_double(test_date_, base_date_.Gregorian()));

    return pass();
}

/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) {
    test_CEDate tester;
    return tester.runtests();
}
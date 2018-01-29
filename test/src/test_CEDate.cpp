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
    base_date.SetDate(CppEphem::julian_date_J2000());
    test_date.SetDate(base_date);
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
    update_pass(test_set_JD());

    return pass;
}


/**********************************************************************//**
 * Test ability to set julian date
 *************************************************************************/
bool test_CEDate::test_set_JD()
{
    test_date.SetDate(base_date + 1);
    return test_double(test_date, base_date);
}
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
    // Setup the observer
    base_observer_ = CEObserver(0.0, 0.0, 0.0, CEAngleType::DEGREES);
    base_observer_.SetTemperature_C(0.0);
    base_observer_.SetPressure_hPa(0.0);
    base_observer_.SetRelativeHumidity(0.0);
    base_observer_.SetWavelength_um(0.0);

    // Setup the date
    base_date_ = CEDate(CppEphem::julian_date_J2000(), CEDateType::JD);
    base_date_.SetReturnType(CEDateType::JD);

    // Set the object that will be observed
    base_body_ = CEBody("test", CEAngle::Deg(83.663), CEAngle::Deg(22.0145), 
                        CESkyCoordType::ICRS);
    
    // Put it all together
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
    test_constructor();
    test_obj_return();
    test_cache();

    return pass();
}


/**********************************************************************//**
 * Tests constructing a CEObservation object
 *  @return Status of tests
 *************************************************************************/
bool test_CEObservation::test_constructor(void)
{
    // Default constructor
    CEObservation test1;
    test(test1.Body() == nullptr, __func__, __LINE__);
    test(test1.Date() == nullptr, __func__, __LINE__);
    test(test1.Observer() == nullptr, __func__, __LINE__);

    // Test the copy constructor
    CEObservation test2(base_obs_);
    test(test2.Body() == base_obs_.Body(), __func__, __LINE__);
    test(test2.Date() == base_obs_.Date(), __func__, __LINE__);
    test(test2.Observer() == base_obs_.Observer(), __func__, __LINE__);

    // Test construction from base objects
    CEObservation test3(&base_observer_, &base_body_, &base_date_);
    test(test3.Body() == &base_body_, __func__, __LINE__);
    test(test3.Date() == &base_date_, __func__, __LINE__);
    test(test3.Observer() == &base_observer_, __func__, __LINE__);

    // Test the copy assignment operator
    CEObservation test4 = test3;
    test(test4.Body() == &base_body_, __func__, __LINE__);
    test(test4.Date() == &base_date_, __func__, __LINE__);
    test(test4.Observer() == &base_observer_, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Tests accessing underlying observer, date, and object classes
 * @return Status of tests
 *************************************************************************/
bool test_CEObservation::test_obj_return(void)
{
    // Test getting the pointers to the objects
    CEObservation test1 = base_obs_;
    test(test1.Body() != nullptr, __func__, __LINE__);
    test(test1.Body() == &base_body_, __func__, __LINE__);
    test(test1.Date() != nullptr, __func__, __LINE__);
    test(test1.Date() == &base_date_, __func__, __LINE__);
    test(test1.Observer() != nullptr, __func__, __LINE__);
    test(test1.Observer() == &base_observer_, __func__, __LINE__);

    // Copy base objects for testing (the pointers will be different)
    CEBody     test_body(base_body_);
    CEDate     test_date(base_date_);
    CEObserver test_observer(base_observer_);

    // Now reset the pointers
    test1.SetBody(&test_body);
    test1.SetDate(&test_date);
    test1.SetObserver(&test_observer);

    // Test that the pointers have been reset
    test(test1.Body() != nullptr, __func__, __LINE__);
    test(test1.Date() != nullptr, __func__, __LINE__);
    test(test1.Observer() != nullptr, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Tests accessing and updating of cached parameters
 * @return Status of tests
 *************************************************************************/
bool test_CEObservation::test_cache(void)
{
    // Test default values
    CEObservation test1;
    test_double(test1.GetAltitude_Deg(), 90.0, __func__, __LINE__);
    test_double(test1.GetAzimuth_Deg(), 0.0, __func__, __LINE__);
    test_double(test1.GetZenith_Deg(), 0.0, __func__, __LINE__);
    test_double(test1.GetApparentXCoordinate_Deg(), 0.0, __func__, __LINE__);
    test_double(test1.GetApparentYCoordinate_Deg(), 0.0, __func__, __LINE__);
    test_double(test1.GetHourAngle_Deg(), 0.0, __func__, __LINE__);
    
    // Now we setup the actual tests for observed coordinates
    // Note: these coordinates are derived from CESkyCoord tests
    CESkyCoord obs_coords(CEAngle::Deg(35.55160709646245), 
                          CEAngle::Deg(152.5681387256824),
                          CESkyCoordType::OBSERVED);
    CESkyCoord test2(base_obs_.GetAzimuth_Rad(),
                     base_obs_.GetZenith_Rad(),
                     CESkyCoordType::OBSERVED);

    // Test that the coordinates update when the date is changed
    test(obs_coords == test2, __func__, __LINE__);
    test_double(base_obs_.GetApparentXCoordinate_Deg(), 0.0, __func__, __LINE__);
    test_double(base_obs_.GetApparentYCoordinate_Deg(), 0.0, __func__, __LINE__);
    test_double(base_obs_.GetHourAngle_Deg(), 0.0, __func__, __LINE__);

    // Test the actual coordinates
    double test_x(0), test_y(0);
        
    // Test observed coordinates
    base_obs_.GetAzimuthZenith_Deg(&test_x, &test_y);
    test_double(test_x, test2.XCoord().Deg(), __func__, __LINE__);
    test_double(test_y, test2.YCoord().Deg(), __func__, __LINE__);

    // Test apparent coordinates
    base_obs_.GetApparentXYCoordinate_Deg(&test_x, &test_y);
    test_double(test_x, 0.0, __func__, __LINE__);
    test_double(test_y, 0.0, __func__, __LINE__);

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

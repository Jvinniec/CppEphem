/***************************************************************************
 *  test_CEAngle.cpp: CppEphem                                             *
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

#include <iostream>

#include "test_CEAngle.h"
#include "CEException.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CEAngle::test_CEAngle() :
    CETestSuite()
{
    // Let's use 45 degrees for the tests, or PI/2
    base_ = CEAngle(M_PI_2);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CEAngle::~test_CEAngle()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEAngle::runtests()
{
    std::cout << "\nTesting CEAngle:\n";

    // Run each of the tests
    test_construct();
    test_setangle();
    test_retrieve();

    return pass();
}


/**********************************************************************//**
 * Test the various constructor methods
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEAngle::test_construct(void)
{
    // Default constructor
    CEAngle test1;
    test_double(test1.Rad(), 0.0, __func__, __LINE__);

    // Copy constructor (CEAngle)
    CEAngle test2(base_);
    test_double(test2.Rad(), base_.Rad(), __func__, __LINE__);

    // Copy-assignment operator (CEAngle)
    CEAngle test3 = base_;
    test_double(test3.Rad(), base_.Rad(), __func__, __LINE__);

    // Copy-assignment operator (double)
    double angle_test = M_PI;
    CEAngle test4;
    test4 = angle_test;
    test_double(test4, angle_test, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test setting the angle from multiple methods representing 45 degrees
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEAngle::test_setangle(void)
{
    // Construct the test angle
    CEAngle test_ang;
    // Test radians
    double angle_rad = M_PI_2;  // PI/2 radians or 90 degrees
    test_ang.SetAngle(angle_rad, CEAngleType::RADIANS);
    test_double(test_ang, angle_rad, __func__, __LINE__);

    // Test degrees
    double angle_deg = 180.0;
    std::string angle_deg_str = "180.0";
    test_ang.SetAngle(angle_deg, CEAngleType::DEGREES);
    test_double(test_ang, M_PI, __func__, __LINE__);
    test_ang.SetAngle(angle_deg_str.c_str(), CEAngleType::DEGREES);

    // HMS values to be tested
    std::string angle_hms_str1 = "3:0:0";
    std::string angle_hms_str2 = "6 0 0";
    std::string angle_hms_str3 = "9.0.0";
    test_ang.SetAngle(angle_hms_str1.c_str(), CEAngleType::HMS);
    test_double(test_ang, M_PI_2/2.0, __func__, __LINE__);
    test_ang.SetAngle(angle_hms_str2.c_str(), CEAngleType::HMS);
    test_double(test_ang, M_PI_2, __func__, __LINE__);
    test_ang.SetAngle(angle_hms_str3.c_str(), CEAngleType::HMS, '.');
    test_double(test_ang, 1.5 * M_PI_2, __func__, __LINE__);

    // DMS values to be tested
    std::string angle_dms_str1 = "45:0:0";
    test_ang.SetAngle(angle_dms_str1.c_str(), CEAngleType::DMS);
    test_double(test_ang, M_PI_2/2.0, __func__, __LINE__);

    // Try to set the angle by passing the wrong delimiter
    try {
        test_ang.SetAngle(angle_hms_str1.c_str(), CEAngleType::HMS, ' ');
        test(false, __func__, __LINE__);
    } catch (CEException::invalid_delimiter& e) {
        test(true, __func__, __LINE__);
    }

    return pass();
}



/**********************************************************************//**
 * Test getting the angle from multiple methods
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEAngle::test_retrieve(void)
{
    double angle = M_PI;

    // Test getting from...
    // ... degrees
    test_double(CEAngle::Deg(180), angle, __func__, __LINE__);
    // ... radians
    test_double(CEAngle::Rad(angle), angle, __func__, __LINE__);
    // ... Hms (version 1)
    test_double(CEAngle::Hms("12:00:00"), angle, __func__, __LINE__);
    test_double(CEAngle::Hms("12 00 00"), angle, __func__, __LINE__);
    test_double(CEAngle::Hms("12.00.00", '.'), angle, __func__, __LINE__);
    // ... HMS (version 2)
    test_double(CEAngle::Hms({12.0, 0.0, 0.0, 0.0}), angle, __func__, __LINE__);
    // ... Dms (version 1)
    test_double(CEAngle::Dms("180:00:00"), angle, __func__, __LINE__);
    test_double(CEAngle::Dms("180 00 00"), angle, __func__, __LINE__);
    test_double(CEAngle::Dms("180'00'00", '\''), angle, __func__, __LINE__);
    // ... DMS (version 2)
    test_double(CEAngle::Dms({180, 0, 0, 0.0}), angle, __func__, __LINE__);

    // Now test some of the other ways in which to get back the angle using
    // a particular format. For this, we will use 315.25 degrees
    double test_deg = 315.25;
    CEAngle test_ang = test_deg * DD2R;

    // Deg
    test_double(test_ang.Deg(), test_deg, __func__, __LINE__);

    // Rad
    test_double(test_ang.Rad(), test_deg*DD2R, __func__, __LINE__);
    
    // HMS as string
    std::string hms_str = test_ang.HmsStr('\'');
    hms_str = std::string(hms_str.begin(), hms_str.begin()+11);
    test_string(hms_str, "21'01'00.00", __func__, __LINE__);
    
    // HMS as vector<double>
    test_vect(test_ang.HmsVect(), {21, 1, 0, 0}, __func__, __LINE__);
    
    // DMS as string
    std::string dms_str = test_ang.DmsStr('"');
    dms_str = std::string(dms_str.begin(), dms_str.begin()+12);
    test_string(dms_str, "315\"15\"00.00", __func__, __LINE__);

    // DMS as vector<double>
    test_vect(test_ang.DmsVect(), {315, 15, 0, 0}, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) 
{
    test_CEAngle tester;
    return (!tester.runtests());
}

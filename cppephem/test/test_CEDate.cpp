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
    // Set the date to J2000 Julian date
    base_date_.SetDate(2451545.000000);
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
    std::cout << "\nTesting CEDate:\n";

    // Run each of the tests
    test_constructor();
    test_SetDate_JD();
    test_SetDate_MJD();
    test_Gregorian();
    test_ReturnType();
    test_support_methods();

    return pass();
}


/**********************************************************************//**
 * Test ability to create a CEDate object
 *************************************************************************/
bool test_CEDate::test_constructor(void)
{
    // Values for testing
    std::vector<double> greg_20190101 = {2019, 1.0, 1.0, 0.5};
    double              jd_20190101   = 2458485.0;

    // Default constructor. Because the default date is given as the current
    // date, the check is just that the value is greater than 01-01-2019
    CEDate test1;
    test_greaterthan(test1.JD(), jd_20190101, __func__, __LINE__);

    // Copy constructor
    CEDate test2(base_date_);
    test_double(test2.JD(), base_date_.JD(), __func__, __LINE__);

    // Copy assignment operator
    CEDate test3 = base_date_;
    test_double(test3.JD(), base_date_.JD(), __func__, __LINE__);

    // Construct from a vector
    CEDate test4(greg_20190101);
    test_double(test4.JD(), jd_20190101, __func__, __LINE__);

    // Construct from a Julian date
    CEDate test5(jd_20190101);
    test_double(test5.JD(), jd_20190101, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test ability to set julian date
 *************************************************************************/
bool test_CEDate::test_SetDate_JD()
{
    test_date_.SetDate(base_date_.JD(), CEDateType::JD);
    return test_double(test_date_.JD(), base_date_.JD(), __func__, __LINE__);
}

/**********************************************************************//**
 * Test ability to set modified julian date
 *************************************************************************/
bool test_CEDate::test_SetDate_MJD()
{
    test_date_.SetDate(base_date_.MJD(), CEDateType::MJD);
    return test_double(test_date_.MJD(), base_date_.MJD(), __func__, __LINE__);
}


/**********************************************************************//**
 * Test ability to set Gregorian date
 *************************************************************************/
bool test_CEDate::test_Gregorian()
{
    test_date_.SetDate(base_date_.Gregorian(), CEDateType::GREGORIAN);
    test_double(test_date_.Gregorian(), base_date_.Gregorian(), __func__, __LINE__);

    // Test getting the year, month, day, and day fraction
    test_int(test_date_.Year(), base_date_.Year(), __func__, __LINE__);
    test_int(test_date_.Month(), base_date_.Month(), __func__, __LINE__);
    test_int(test_date_.Day(), base_date_.Day(), __func__, __LINE__);
    test_double(test_date_.DayFraction(), base_date_.DayFraction(), __func__, __LINE__);

    // Test getting the Gregorian date as a vector
    test_vect(test_date_.GregorianVect(), base_date_.GregorianVect(), __func__, __LINE__);

    // Test setting the date as a gregorian vector
    std::vector<double> greg_vec = base_date_.GregorianVect();
    CEDate test1;
    test(test1.JD() != base_date_.JD(), __func__, __LINE__);
    test1.SetDate(greg_vec);
    test_vect(test1.GregorianVect(), greg_vec, __func__, __LINE__);

    // Make sure that the returned errors of GregorianVect2JD are handled
    greg_vec                      = base_date_.GregorianVect();
    std::vector<double> greg_vec2 = greg_vec;
    // Test invalid year
    greg_vec2[0] = -50000;
    test_double(CEDate::GregorianVect2JD(greg_vec2), 0.0, __func__, __LINE__);
    greg_vec2 = greg_vec;
    // Test invalid month
    greg_vec2[1] = -1;
    test_double(CEDate::GregorianVect2JD(greg_vec2), 0.0, __func__, __LINE__);
    greg_vec2[1] = 13;
    test_double(CEDate::GregorianVect2JD(greg_vec2), 0.0, __func__, __LINE__);
    greg_vec2 = greg_vec;
    // Test date
    greg_vec2[2] += 100;
    test_double(CEDate::GregorianVect2JD(greg_vec2), base_date_.JD() + 100.0, __func__, __LINE__);

    // Test conversion of gregorian vector to JD, MJD, Gregorian date
    greg_vec = {2000, 01, 01, 0.5};
    double test_jd   = CppEphem::julian_date_J2000();
    double test_mjd  = test_jd - DJM0;
    double test_greg = 20000101.5;
    test_double(CEDate::GregorianVect2JD(greg_vec), test_jd, __func__, __LINE__);
    test_double(CEDate::GregorianVect2MJD(greg_vec), test_mjd, __func__, __LINE__);
    test_double(CEDate::GregorianVect2Gregorian(greg_vec), test_greg, __func__, __LINE__);

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
    test_double(test_date_, base_date_.JD(), __func__, __LINE__);
    
    // Set the return type to MJD
    test_date_.SetReturnType(CEDateType::MJD);
    test_double(test_date_, base_date_.MJD(), __func__, __LINE__);

    // Set the return type to Gregorian
    test_date_.SetReturnType(CEDateType::GREGORIAN);
    test_double(test_date_, base_date_.Gregorian(), __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test the various 
 *************************************************************************/
bool test_CEDate::test_support_methods(void)
{
    // Seconds since midnight
    double test1 = base_date_.MJD() - std::floor(base_date_.MJD());
    test1 *= CppEphem::sec_per_day();
    test_double(base_date_.GetSecondsSinceMidnight(0.0), test1, __func__, __LINE__);

    // Test CurrentJD (that it is in some reasonable range)
    double test_JD = CEDate::CurrentJD();
    test_greaterthan(test_JD, 2458485.0, __func__, __LINE__);
    test_lessthan(test_JD, 2468485.0, __func__, __LINE__);

    // Test GetTime
    double test_GetTime = base_date_.GetTime(2.0);
    double test_GetTime_UTC = base_date_.GetTime_UTC();
    test_double(test_GetTime, 140000.0, __func__, __LINE__);
    test_double(test_GetTime_UTC, 120000.0, __func__, __LINE__);
    
    // Test DUT1
    double mjd  = base_date_.MJD();
    double dut1 = CppEphem::dut1(mjd);
    test_double(base_date_.dut1(), dut1, __func__, __LINE__);
    test_double(CEDate::dut1(mjd, CEDateType::MJD), dut1, __func__, __LINE__);

    // Test xp,yp
    double xpolar = CppEphem::xp(mjd);
    double ypolar = CppEphem::yp(mjd);
    test_double(base_date_.xpolar(), xpolar, __func__, __LINE__);
    test_double(base_date_.ypolar(), ypolar, __func__, __LINE__);
    test_double(CEDate::xpolar(mjd, CEDateType::MJD), xpolar, __func__, __LINE__);
    test_double(CEDate::ypolar(mjd, CEDateType::MJD), ypolar, __func__, __LINE__);

    // UTC -> UT1
    double test_ut1 = base_date_.MJD() + (base_date_.dut1() / CppEphem::sec_per_day());
    double ut11, ut12;
    CEDate::UTC2UT1(base_date_.MJD(), &ut11, &ut12);
    test_double(ut11, CEDate::GetMJD2JDFactor(), __func__, __LINE__);
    test_double(ut12, test_ut1, __func__, __LINE__);

    // UTC -> TT
    // NOTE: This is not a good test without the correct corrections
    double test_tt(test_ut1 + 63.8285/DAYSEC);
    double tt1, tt2;
    CEDate::UTC2TT(base_date_.MJD(), &tt1, &tt2);
    test_double(tt1, CEDate::GetMJD2JDFactor(), __func__, __LINE__);
    test_double(tt2, test_tt, __func__, __LINE__);

    // UTC -> TDB
    // NOTE: This is not a good test without the correct corrections
    double test_tdb(test_tt);
    double tdb1, tdb2;
    CEDate::UTC2TDB(base_date_.MJD(), &tdb1, &tdb2);
    test_double(tdb1, CEDate::GetMJD2JDFactor(), __func__, __LINE__);
    test_double(tdb2, test_tdb, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) 
{
    test_CEDate tester;
    return (!tester.runtests());
}

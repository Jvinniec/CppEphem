/***************************************************************************
 *  test_CECoordinates.cpp: CppEphem                                       *
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

#include "test_CECoordinates.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CECoordinates::test_CECoordinates() :
    CETestSuite()
{
    // Lets use the Crab Nebula (M1) for our testing

    // The following coordinates are derived from Astopy using the script:
    // cppephem/test/astropy_scripts/test_cecoordinates_setup.py
    base_icrs_ = CECoordinates(83.663, 22.0145, 
                               CECoordinateType::ICRS, 
                               CEAngleType::DEGREES);
    base_cirs_ = CECoordinates(83.66843896766689, 22.012942151427037,
                               CECoordinateType::CIRS,
                               CEAngleType::DEGREES);
    base_gal_  = CECoordinates(184.57237825620092, -5.7609283525221615,
                               CECoordinateType::GALACTIC,
                               CEAngleType::DEGREES);
    base_obs_  = CECoordinates(35.55160709646245, 152.5681387256824,
                               CECoordinateType::OBSERVED,
                               CEAngleType::DEGREES);

    // Create the date object
    date_ = CEDate(CppEphem::julian_date_J2000(), CEDateType::JD);
    // Interpret the date object as a julian date
    date_.SetReturnType(CEDateType::JD);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CECoordinates::~test_CECoordinates()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CECoordinates::runtests()
{
    std::cout << "\nTesting CECoordinates:\n";

    // Test constructions
    test_construct();
    test_copy();
    test_HmsDms2Angle();

    // Conversion tests
    test_Convert2Cirs();
    test_Convert2Icrs();
    test_Convert2Galactic();
    test_Convert2Observed();

    return pass();
}

/**********************************************************************//**
 * Test generic constructors
 *************************************************************************/
bool test_CECoordinates::test_construct()
{
    // Test the default constructor
    CECoordinates test1;
    test_double(test1.XCoordinate_Rad(), 0.0, __func__, __LINE__);
    test_double(test1.YCoordinate_Rad(), 0.0, __func__, __LINE__);
    
    // Test constructor from degrees
    double testx(83.633);
    double testy(22.0145);
    CECoordinates test2(testx, testy, CECoordinateType::ICRS, CEAngleType::DEGREES);
    test_double(test2.XCoordinate_Deg(), testx, __func__, __LINE__);
    test_double(test2.YCoordinate_Deg(), testy, __func__, __LINE__);

    std::vector<double> testHMS = CECoordinates::GetHMS(testx,
                                                        CEAngleType::DEGREES);
    std::vector<double> testDMS = CECoordinates::GetDMS(testy,
                                                        CEAngleType::DEGREES);
    CECoordinates test3(testHMS, testDMS, CECoordinateType::ICRS);
    test_double(test3.XCoordinate_Deg(), testx, __func__, __LINE__);
    test_double(test3.YCoordinate_Deg(), testy, __func__, __LINE__);
    
    return pass();
}

/**********************************************************************//**
 * Test ability to copy coordinates
 *************************************************************************/
bool test_CECoordinates::test_copy()
{
    // Make sure the equals and not-equals operators work properly
    test_bool((base_icrs_ == base_icrs_), true, __func__, __LINE__);
    test_bool((base_icrs_ != base_icrs_), false, __func__, __LINE__);
    test_bool((base_icrs_ == base_gal_), false, __func__, __LINE__);
    test_bool((base_icrs_ != base_gal_), true, __func__, __LINE__);

    // Copy CIRS
    CECoordinates cirs = base_cirs_;
    test((cirs == base_cirs_), __func__, __LINE__);

    // Copy ICRS
    CECoordinates icrs = base_icrs_;
    test((icrs == base_icrs_), __func__, __LINE__);

    // Copy Galactic
    CECoordinates gal = base_gal_;
    test((gal == base_gal_), __func__, __LINE__);

    // Copy Observed
    CECoordinates obs = base_obs_;
    test((obs == base_obs_), __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test ability to set coordinates as CIRS
 *************************************************************************/
bool test_CECoordinates::test_Convert2Cirs()
{
    // ICRS -> CIRS
    CECoordinates icrs2cirs = base_icrs_.ConvertToCIRS(date_);
    test((icrs2cirs == base_cirs_), __func__, __LINE__);

    // Galactic -> CIRS
    CECoordinates gal2cirs = base_gal_.ConvertToCIRS(date_);
    test_bool((gal2cirs == base_cirs_), true, __func__, __LINE__);

    // Observed -> CIRS
    CECoordinates obs2cirs = base_obs_.ConvertToCIRS(
                                date_,          // date
                                0.0, 0.0, 0.0,  // lon, lat, elevation
                                0.0, 0.0, 0.0,  // pressure, temperature, humidity
                                date_.dut1(),   // UT1-UTC correction
                                date_.xpolar(), // xpolar motion correction
                                date_.ypolar(), // ypolar motion correction
                                0.0);           // wavelength (micrometers)
    test_bool((obs2cirs == base_cirs_), true, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test ability to convert to ICRS
 *************************************************************************/
bool test_CECoordinates::test_Convert2Icrs()
{
    // CIRS -> ICRS
    CECoordinates cirs2icrs = base_cirs_.ConvertToICRS(date_);
    test_bool((cirs2icrs == base_icrs_), true, __func__, __LINE__);

    // Galactic -> ICRS
    CECoordinates gal2icrs = base_gal_.ConvertToICRS();
    test_bool((gal2icrs == base_icrs_), true, __func__, __LINE__);

    // Observed -> ICRS
    CECoordinates obs2icrs = base_obs_.ConvertToICRS(
                                date_,          // date
                                0.0, 0.0, 0.0,  // lon, lat, elevation
                                0.0, 0.0, 0.0,  // pressure, temperature, humidity
                                date_.dut1(),   // UT1-UTC correction
                                date_.xpolar(), // xpolar motion correction
                                date_.ypolar(), // ypolar motion correction
                                0.0);           // wavelength (micrometers)
    test_bool((obs2icrs == base_icrs_), true, __func__, __LINE__);

    return pass();
}

/**********************************************************************//**
 * Test ability to convert to Galactic
 *************************************************************************/
bool test_CECoordinates::test_Convert2Galactic()
{
    // CIRS -> Galactic
    CECoordinates cirs2gal = base_cirs_.ConvertToGalactic(date_);
    test_bool((cirs2gal == base_gal_), true, __func__, __LINE__);

    // ICRS -> Galactic
    CECoordinates icrs2gal = base_icrs_.ConvertToGalactic();
    test_bool((icrs2gal == base_gal_), true, __func__, __LINE__);

    // Observed -> Galactic
    CECoordinates obs2gal = base_obs_.ConvertToGalactic(
                                date_,          // date
                                0.0, 0.0, 0.0,  // lon, lat, elevation
                                0.0, 0.0, 0.0,  // pressure, temperature, humidity
                                date_.dut1(),   // UT1-UTC correction
                                date_.xpolar(), // xpolar motion correction
                                date_.ypolar(), // ypolar motion correction
                                0.0);           // wavelength (micrometers)
    test_bool((obs2gal == base_gal_), true, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test ability to convert to Observed
 *************************************************************************/
bool test_CECoordinates::test_Convert2Observed()
{
    // CIRS -> Observed
    CECoordinates cirs2obs = base_cirs_.ConvertToObserved(
                                date_,          // date
                                0.0, 0.0, 0.0,  // lon, lat, elevation
                                0.0, 0.0, 0.0,  // pressure, temperature, humidity
                                date_.dut1(),   // UT1-UTC correction
                                date_.xpolar(), // xpolar motion correction
                                date_.ypolar(), // ypolar motion correction
                                0.0);           // wavelength (micrometers)
    test_bool((cirs2obs == base_obs_), true, __func__, __LINE__);

    // ICRS -> Observed
    CECoordinates icrs2obs = base_icrs_.ConvertToObserved(
                                date_,          // date
                                0.0, 0.0, 0.0,  // lon, lat, elevation
                                0.0, 0.0, 0.0,  // pressure, temperature, humidity
                                date_.dut1(),   // UT1-UTC correction
                                date_.xpolar(), // xpolar motion correction
                                date_.ypolar(), // ypolar motion correction
                                0.0);           // wavelength (micrometers)
    test_bool((icrs2obs == base_obs_), true, __func__, __LINE__);

    // Galactic -> Observed
    CECoordinates gal2obs = base_gal_.ConvertToObserved(
                                date_,          // date
                                0.0, 0.0, 0.0,  // lon, lat, elevation
                                0.0, 0.0, 0.0,  // pressure, temperature, humidity
                                date_.dut1(),   // UT1-UTC correction
                                date_.xpolar(), // xpolar motion correction
                                date_.ypolar(), // ypolar motion correction
                                0.0);           // wavelength (micrometers)
    test_bool((gal2obs == base_obs_), true, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test conversions between HMS and DMS to angle and back
 *************************************************************************/
bool test_CECoordinates::test_HmsDms2Angle(void)
{
    // Create some variables for testing
    double              ang_var(45.50833333333333);
    std::vector<double> ang_vec = {45, 30, 30};
    std::vector<double> hrs_vec = {3.0, 2.0, 2.0};

    // Test DMS -> angle
    double test2(CECoordinates::DMSToAngle(ang_vec, CEAngleType::DEGREES));
    test_double(test2, ang_var, __func__, __LINE__);

    // Test HMS -> angle
    double test4(CECoordinates::HMSToAngle(hrs_vec, CEAngleType::DEGREES));
    test_double(test4, ang_var, __func__, __LINE__);

    // Increase the acceptable tolerance
    double tol_old(DblTol());
    SetDblTol(1.0e-11);

    // Test angle -> DMS 
    std::vector<double> test1 = CECoordinates::GetDMS(ang_var, CEAngleType::DEGREES);
    test_vect(test1, ang_vec, __func__, __LINE__);

    // Test angle -> HMS 
    std::vector<double> test3 = CECoordinates::GetHMS(ang_var, CEAngleType::DEGREES);
    test_vect(test3, hrs_vec, __func__, __LINE__);

    // Reset the tolerance
    SetDblTol(tol_old);

    return pass();
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) 
{
    test_CECoordinates tester;
    return (!tester.runtests());
}

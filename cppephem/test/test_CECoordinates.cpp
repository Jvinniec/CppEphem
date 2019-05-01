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
#include "CEObserver.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CECoordinates::test_CECoordinates() :
    CETestSuite()
{
    // Interpolate the correction terms
    CppEphem::CorrectionsInterp(true);

    // Lets use the Crab Nebula (M1) for our testing

    // The following coordinates are derived from Astopy using the script:
    // cppephem/test/astropy_scripts/test_cecoordinates_setup.py
    base_icrs_ = CECoordinates(83.633, 22.0145, 
                               CECoordinateType::ICRS, 
                               CEAngleType::DEGREES);
    base_cirs_ = CECoordinates(83.63843844654943, 22.012942530431776,
                               CECoordinateType::CIRS,
                               CEAngleType::DEGREES);
    base_gal_  = CECoordinates(184.55741630955762, -5.784421958594916,
                               CECoordinateType::GALACTIC,
                               CEAngleType::DEGREES);
    base_obs_  = CECoordinates(35.598599384274294, 152.55068501449307,
                               CECoordinateType::OBSERVED,
                               CEAngleType::DEGREES);

    // Create the date object
    base_date_ = CEDate(CppEphem::julian_date_J2000(), CEDateType::JD);
    // Interpret the date object as a julian date
    base_date_.SetReturnType(CEDateType::JD);

    // Setup the observer object for observed coordinates
    base_observer_ = CEObserver(0.0, 0.0, 0.0, CEAngleType::DEGREES);
    base_observer_.SetTemperature_C(0.0);
    base_observer_.SetPressure_hPa(0.0);
    base_observer_.SetRelativeHumidity(0.0);
    base_observer_.SetWavelength_um(0.0);
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

    // Test dedicated methods
    test_AngularSeparation();
    test_ConvertTo();

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
    
    // Test copy constructor
    CECoordinates test4(test2);
    test_double(test4.XCoordinate_Deg(), testx, __func__, __LINE__);
    test_double(test4.YCoordinate_Deg(), testy, __func__, __LINE__);
    test_int(int(test4.GetCoordSystem()), int(test2.GetCoordSystem()), __func__, __LINE__);

    // Test print of constructed coordinates
    test_greaterthan(test4.print().size(), 0, __func__, __LINE__);

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

    // Copy coordinates directly
    gal.SetCoordinates(icrs);
    test((gal == icrs), __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test ability to set coordinates as CIRS
 *************************************************************************/
bool test_CECoordinates::test_Convert2Cirs()
{
    // ICRS -> CIRS
    CECoordinates icrs2cirs = base_icrs_.ConvertToCIRS(base_date_);
    test_coords(icrs2cirs, base_cirs_, __func__, __LINE__);

    // Galactic -> CIRS
    CECoordinates gal2cirs = base_gal_.ConvertToCIRS(base_date_);
    test_coords(gal2cirs, base_cirs_, __func__, __LINE__);

    // Observed -> CIRS
    CECoordinates obs2cirs = base_obs_.ConvertToCIRS(
                                base_date_,     // date
                                base_observer_.Longitude_Rad(),
                                base_observer_.Latitude_Rad(),
                                base_observer_.Elevation_m(),
                                base_observer_.Pressure_hPa(),
                                base_observer_.Temperature_C(),
                                base_observer_.RelativeHumidity(),
                                base_date_.dut1(),
                                base_date_.xpolar(),
                                base_date_.ypolar(),
                                base_observer_.Wavelength_um());
    test_coords(obs2cirs, base_cirs_, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test ability to convert to ICRS
 *************************************************************************/
bool test_CECoordinates::test_Convert2Icrs()
{
    // CIRS -> ICRS
    CECoordinates cirs2icrs = base_cirs_.ConvertToICRS(base_date_);
    test_coords(cirs2icrs, base_icrs_, __func__, __LINE__);

    // Galactic -> ICRS
    CECoordinates gal2icrs = base_gal_.ConvertToICRS();
    test_coords(gal2icrs, base_icrs_, __func__, __LINE__);

    // Observed -> ICRS
    CECoordinates obs2icrs = base_obs_.ConvertToICRS(
                                base_date_,     // date
                                base_observer_.Longitude_Rad(),
                                base_observer_.Latitude_Rad(),
                                base_observer_.Elevation_m(),
                                base_observer_.Pressure_hPa(),
                                base_observer_.Temperature_C(),
                                base_observer_.RelativeHumidity(),
                                base_date_.dut1(),
                                base_date_.xpolar(),
                                base_date_.ypolar(),
                                base_observer_.Wavelength_um());
    test_coords(obs2icrs, base_icrs_, __func__, __LINE__);

    return pass();
}

/**********************************************************************//**
 * Test ability to convert to Galactic
 *************************************************************************/
bool test_CECoordinates::test_Convert2Galactic()
{
    // CIRS -> Galactic
    CECoordinates cirs2gal = base_cirs_.ConvertToGalactic(base_date_);
    test_coords(cirs2gal, base_gal_, __func__, __LINE__);

    // ICRS -> Galactic
    CECoordinates icrs2gal = base_icrs_.ConvertToGalactic();
    test_coords(icrs2gal, base_gal_, __func__, __LINE__);

    // Observed -> Galactic
    CECoordinates obs2gal = base_obs_.ConvertToGalactic(
                                base_date_,     // date
                                base_observer_.Longitude_Rad(),
                                base_observer_.Latitude_Rad(),
                                base_observer_.Elevation_m(),
                                base_observer_.Pressure_hPa(),
                                base_observer_.Temperature_C(),
                                base_observer_.RelativeHumidity(),
                                base_date_.dut1(),
                                base_date_.xpolar(),
                                base_date_.ypolar(),
                                base_observer_.Wavelength_um());
    test_coords(obs2gal, base_gal_, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test ability to convert to Observed
 *************************************************************************/
bool test_CECoordinates::test_Convert2Observed()
{
    // preliminary variables for observer based conversion
    double az(0.0);
    double zen(0.0);
    CECoordinates testobs;

    // CIRS -> Observed
    CECoordinates cirs2obs = base_cirs_.ConvertToObserved(
                                base_date_,     // date
                                base_observer_.Longitude_Rad(),
                                base_observer_.Latitude_Rad(),
                                base_observer_.Elevation_m(),
                                base_observer_.Pressure_hPa(),
                                base_observer_.Temperature_C(),
                                base_observer_.RelativeHumidity(),
                                base_date_.dut1(),
                                base_date_.xpolar(),
                                base_date_.ypolar(),
                                base_observer_.Wavelength_um());
    test_coords(cirs2obs, base_obs_, __func__, __LINE__);
    // Test the 'CEObserver' based approach
    CECoordinates::CIRS2Observed(base_cirs_.XCoordinate_Deg(),
                                 base_cirs_.YCoordinate_Deg(),
                                 &az, &zen,
                                 base_date_, base_observer_, 
                                 CEAngleType::DEGREES,
                                 base_observer_.Wavelength_um());
    testobs.SetCoordinates(az, zen, CECoordinateType::OBSERVED, CEAngleType::DEGREES);
    test_coords(testobs, base_obs_, __func__, __LINE__);

    // ICRS -> Observed
    CECoordinates icrs2obs = base_icrs_.ConvertToObserved(
                                base_date_,     // date
                                base_observer_.Longitude_Rad(),
                                base_observer_.Latitude_Rad(),
                                base_observer_.Elevation_m(),
                                base_observer_.Pressure_hPa(),
                                base_observer_.Temperature_C(),
                                base_observer_.RelativeHumidity(),
                                base_date_.dut1(),
                                base_date_.xpolar(),
                                base_date_.ypolar(),
                                base_observer_.Wavelength_um());
    test_coords(icrs2obs, base_obs_, __func__, __LINE__);
    // Test the 'CEObserver' based approach
    CECoordinates::ICRS2Observed(base_icrs_.XCoordinate_Deg(),
                                 base_icrs_.YCoordinate_Deg(),
                                 &az, &zen,
                                 base_date_, base_observer_, 
                                 CEAngleType::DEGREES,
                                 base_observer_.Wavelength_um());
    testobs.SetCoordinates(az, zen, CECoordinateType::OBSERVED, CEAngleType::DEGREES);
    test_coords(testobs, base_obs_, __func__, __LINE__);

    // Galactic -> Observed
    CECoordinates gal2obs = base_gal_.ConvertToObserved(
                                base_date_,     // date
                                base_observer_.Longitude_Rad(),
                                base_observer_.Latitude_Rad(),
                                base_observer_.Elevation_m(),
                                base_observer_.Pressure_hPa(),
                                base_observer_.Temperature_C(),
                                base_observer_.RelativeHumidity(),
                                base_date_.dut1(),
                                base_date_.xpolar(),
                                base_date_.ypolar(),
                                base_observer_.Wavelength_um());
    test_coords(gal2obs, base_obs_, __func__, __LINE__);
    // Test the 'CEObserver' based approach
    CECoordinates::Galactic2Observed(base_gal_.XCoordinate_Deg(),
                                     base_gal_.YCoordinate_Deg(),
                                     &az, &zen,
                                     base_date_, base_observer_, 
                                     CEAngleType::DEGREES,
                                     base_observer_.Wavelength_um());
    testobs.SetCoordinates(az, zen, CECoordinateType::OBSERVED, CEAngleType::DEGREES);
    test_coords(testobs, base_obs_, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test conversions between HMS and DMS to angle and back
 *************************************************************************/
bool test_CECoordinates::test_HmsDms2Angle(void)
{
    // Create some variables for testing
    double              ang_var(45.50833333333333);
    std::vector<double> ang_vec = {45, 30, 30, 0.0};
    std::vector<double> hrs_vec = {3.0, 2.0, 2.0, 0.0};

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
 * Test computation of angular separation
 *************************************************************************/
bool test_CECoordinates::test_AngularSeparation(void)
{
    // Establish the preliminary variables that will be used 
    double test1_x(0.0);
    double test1_y(-1.0);
    double test2_x(0.0);
    double test2_y(1.0);
    CECoordinates test1(test1_x, test1_y, CECoordinateType::ICRS, CEAngleType::DEGREES);
    CECoordinates test2(test2_x, test2_y, CECoordinateType::ICRS, CEAngleType::DEGREES);
    
    // Test the default coordinate separation
    double angsep = test1.AngularSeparation(test2, CEAngleType::DEGREES);
    test_double(angsep, 2.0, __func__, __LINE__);
    angsep = CECoordinates::AngularSeparation(test1, test2, CEAngleType::DEGREES);
    test_double(angsep, 2.0, __func__, __LINE__);
    angsep = CECoordinates::AngularSeparation(test1_x, test1_y,
                                              test2_x, test2_y, 
                                              CEAngleType::DEGREES);
    test_double(angsep, 2.0, __func__, __LINE__);

    // Test that the two coordinates are not equal
    test((test1 != test2), __func__, __LINE__);
    
    return pass();
}


/**********************************************************************//**
 * Tests the CECoordinates::ConvrtTo methods
 *************************************************************************/
bool test_CECoordinates::test_ConvertTo(void)
{
    // Create an observer object
    CEObserver observer(0.0, 0.0, 0.0, CEAngleType::DEGREES);
    observer.SetTemperature_C(0.0);
    observer.SetPressure_hPa(0.0);
    observer.SetWavelength_um(0.0);
    observer.SetRelativeHumidity(0.0);

    // Create a date object
    CEDate date(CppEphem::julian_date_J2000(), CEDateType::JD);

    /* ---------------------------*
     * ConvertTo basic form
     * ---------------------------*/
    CECoordinates test_icrs = base_icrs_.ConvertTo(CECoordinateType::ICRS,
                                                   observer, date);
    test_coords(test_icrs, base_icrs_, __func__, __LINE__);
    CECoordinates test_cirs = base_icrs_.ConvertTo(CECoordinateType::CIRS,
                                                   observer, date);
    test_coords(test_cirs, base_cirs_, __func__, __LINE__);
    CECoordinates test_gal = base_icrs_.ConvertTo(CECoordinateType::GALACTIC,
                                                  observer, date);
    test_coords(test_gal, base_gal_, __func__, __LINE__);
    CECoordinates test_obs = base_icrs_.ConvertTo(CECoordinateType::OBSERVED,
                                                  observer, date);
    test_coords(test_obs, base_obs_, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Tests two coordinates are equal and print some help if they aren't
 *************************************************************************/
bool test_CECoordinates::test_coords(const CECoordinates& test,
                                     const CECoordinates& expected,
                                     const std::string&   func,
                                     const int&           line)
{
    // Test if the coordinates are equal
    bool pass = test_bool((test == expected), true, func, line);
    if (!pass) {
        std::printf("     CoordSys (1 v 2): %d v %d\n",
                    int(test.GetCoordSystem()), int(expected.GetCoordSystem()));
        std::printf("     XCoords: %3.15e : %3.15e (deg), diff = %e arcsec\n", 
                    test.XCoordinate_Deg(), expected.XCoordinate_Deg(), 
                    (test.XCoordinate_Deg()-expected.XCoordinate_Deg())*3600.0);
        std::printf("     YCoords: %3.15e : %3.15e (deg), diff = %e arcsec\n", 
                    test.YCoordinate_Deg(), expected.YCoordinate_Deg(), 
                    (test.YCoordinate_Deg()-expected.YCoordinate_Deg())*3600.0);
        std::printf("     AngSep: %e arcsec\n", 
                    test.AngularSeparation(expected, CEAngleType::DEGREES)*3600.0);
    }
    return pass;
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) 
{
    test_CECoordinates tester;
    return (!tester.runtests());
}

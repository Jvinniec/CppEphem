/***************************************************************************
 *  test_CESkyCoord.cpp: CppEphem                                          *
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

#include "test_CESkyCoord.h"
#include "CEObserver.h"
#include "CENamespace.h"
#include "test_CECoordinates.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CESkyCoord::test_CESkyCoord() :
    CETestSuite()
{
    // Interpolate the correction terms
    CppEphem::CorrectionsInterp(true);

    // Lets use the Crab Nebula (M1) for our testing

    // The following coordinates are derived from Astopy using the script:
    // cppephem/test/astropy_scripts/test_cecoordinates_setup.py
    base_icrs_ = CESkyCoord(CEAngle::Deg(83.633), CEAngle::Deg(22.0145), 
                            CESkyCoordType::ICRS);
    base_cirs_ = CESkyCoord(CEAngle::Deg(83.63843844654943), 
                            CEAngle::Deg(22.012942530431776),
                            CESkyCoordType::CIRS);
    base_gal_  = CESkyCoord(CEAngle::Deg(184.55741630955762), 
                            CEAngle::Deg(-5.784421958594916),
                            CESkyCoordType::GALACTIC);
    base_obs_  = CESkyCoord(CEAngle::Deg(35.598599384274294), 
                            CEAngle::Deg(152.55068501449307),
                            CESkyCoordType::OBSERVED);
    base_ecl_  = CESkyCoord(CEAngle::Deg(84.09742873115043),
                            CEAngle::Deg(-1.2944191660399447),
                            CESkyCoordType::ECLIPTIC);

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
test_CESkyCoord::~test_CESkyCoord()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CESkyCoord::runtests()
{
    std::cout << "\nTesting CESkyCoord:\n";

    // Test constructions
    test_construct();
    test_copy();

    // Conversion tests
    test_Convert2Cirs();
    test_Convert2Icrs();
    test_Convert2Galactic();
    test_Convert2Observed();
    test_Convert2Ecliptic();

    // Test dedicated methods
    test_AngularSeparation();
    test_ConvertTo();

    return pass();
}

/**********************************************************************//**
 * Test generic constructors
 *************************************************************************/
bool test_CESkyCoord::test_construct()
{
    // Test the default constructor
    CESkyCoord test1;
    test_double(test1.XCoord().Rad(), 0.0, __func__, __LINE__);
    test_double(test1.YCoord().Rad(), 0.0, __func__, __LINE__);
    
    // Test constructor from CEAngle objects
    CEAngle testx = CEAngle::Deg(83.633);
    CEAngle testy = CEAngle::Deg(22.0145);
    CESkyCoord test2(testx, testy, CESkyCoordType::ICRS);
    test_double(test2.XCoord(), testx, __func__, __LINE__);
    test_double(test2.YCoord(), testy, __func__, __LINE__);
    test_int(int(test2.GetCoordSystem()), int(CESkyCoordType::ICRS), __func__, __LINE__);
    
    // Test copy constructor
    CESkyCoord test4(test2);
    test_double(test4.XCoord(), testx, __func__, __LINE__);
    test_double(test4.YCoord(), testy, __func__, __LINE__);
    test_int(int(test4.GetCoordSystem()), int(test2.GetCoordSystem()), __func__, __LINE__);

    // Test copy-assignment operator
    CESkyCoord test5 = test2;
    test_coords(test5, test2, __func__, __LINE__);

    // Test constructor from CECoordinates
    CECoordinates coord(testx, testy, CECoordinateType::GALACTIC);
    CESkyCoord test6(coord);
    test_double(test6.XCoord(), testx, __func__, __LINE__);
    test_double(test6.YCoord(), testy, __func__, __LINE__);
    test_int(int(test6.GetCoordSystem()), int(CECoordinateType::GALACTIC), __func__, __LINE__);

    // Test print of constructed coordinates
    test_greaterthan(test4.print().size(), 0, __func__, __LINE__);
    test_string(test4.ClassName(), "CESkyCoord", __func__, __LINE__);
    test_greaterthan(test4.describe().size(), 0, __func__, __LINE__);

    return pass();
}

/**********************************************************************//**
 * Test ability to copy coordinates
 *************************************************************************/
bool test_CESkyCoord::test_copy()
{
    // Make sure the equals and not-equals operators work properly
    test_bool((base_icrs_ == base_icrs_), true, __func__, __LINE__);
    test_bool((base_icrs_ != base_icrs_), false, __func__, __LINE__);
    test_bool((base_icrs_ == base_gal_), false, __func__, __LINE__);
    test_bool((base_icrs_ != base_gal_), true, __func__, __LINE__);

    // Copy CIRS
    CESkyCoord cirs = base_cirs_;
    test_coords(cirs, base_cirs_, __func__, __LINE__);

    // Copy ICRS
    CESkyCoord icrs = base_icrs_;
    test_coords(icrs, base_icrs_, __func__, __LINE__);

    // Copy Galactic
    CESkyCoord gal = base_gal_;
    test_coords(gal, base_gal_, __func__, __LINE__);

    // Copy Observed
    CESkyCoord obs = base_obs_;
    test_coords(obs, base_obs_, __func__, __LINE__);

    // Copy coordinates directly
    gal.SetCoordinates(icrs);
    test_coords(gal, icrs, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test ability to set coordinates as CIRS
 *************************************************************************/
bool test_CESkyCoord::test_Convert2Cirs()
{
    // CIRS -> CIRS
    CESkyCoord cirs2cirs = base_cirs_.ConvertToCIRS();
    test_coords(cirs2cirs, base_cirs_, __func__, __LINE__);

    // ICRS -> CIRS
    CESkyCoord icrs2cirs = base_icrs_.ConvertToCIRS(base_date_);
    test_coords(icrs2cirs, base_cirs_, __func__, __LINE__);

    CESkyCoord::ICRS2CIRS(base_icrs_, &icrs2cirs, base_date_);
    test_coords(icrs2cirs, base_cirs_, __func__, __LINE__);

    // Galactic -> CIRS
    CESkyCoord gal2cirs = base_gal_.ConvertToCIRS(base_date_);
    test_coords(gal2cirs, base_cirs_, __func__, __LINE__);

    CESkyCoord::Galactic2CIRS(base_gal_, &gal2cirs, base_date_);
    test_coords(gal2cirs, base_cirs_, __func__, __LINE__);
    
    // Observed -> CIRS
    CESkyCoord obs2cirs = base_obs_.ConvertToCIRS(base_date_, base_observer_);
    test_coords(obs2cirs, base_cirs_, __func__, __LINE__);

    CESkyCoord::Observed2CIRS(base_obs_, &obs2cirs, base_date_, base_observer_);
    test_coords(obs2cirs, base_cirs_, __func__, __LINE__);

    // Ecliptic -> CIRS
    CESkyCoord ecl2cirs = base_ecl_.ConvertToCIRS(base_date_);
    test_coords(ecl2cirs, base_cirs_, __func__, __LINE__);

    CESkyCoord::Ecliptic2CIRS(base_ecl_, &ecl2cirs, base_date_);
    test_coords(ecl2cirs, base_cirs_, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test ability to convert to ICRS
 *************************************************************************/
bool test_CESkyCoord::test_Convert2Icrs()
{
    // CIRS -> ICRS
    CESkyCoord cirs2icrs = base_cirs_.ConvertToICRS(base_date_);
    test_coords(cirs2icrs, base_icrs_, __func__, __LINE__);

    CESkyCoord::CIRS2ICRS(base_cirs_, &cirs2icrs, base_date_);
    test_coords(cirs2icrs, base_icrs_, __func__, __LINE__);
    
    // ICRS -> ICRS
    CESkyCoord icrs2icrs = base_icrs_.ConvertToICRS();
    test_coords(icrs2icrs, base_icrs_, __func__, __LINE__);

    // Galactic -> ICRS
    CESkyCoord gal2icrs = base_gal_.ConvertToICRS();
    test_coords(gal2icrs, base_icrs_, __func__, __LINE__);

    CESkyCoord::Galactic2ICRS(base_gal_, &gal2icrs);
    test_coords(gal2icrs, base_icrs_, __func__, __LINE__);

    // Observed -> ICRS
    CESkyCoord obs2icrs = base_obs_.ConvertToICRS(base_date_, base_observer_);
    test_coords(obs2icrs, base_icrs_, __func__, __LINE__);

    CESkyCoord::Observed2ICRS(base_obs_, &obs2icrs, base_date_, base_observer_);
    test_coords(obs2icrs, base_icrs_, __func__, __LINE__);

    // Ecliptic -> ICRS
    CESkyCoord ecl2icrs = base_ecl_.ConvertToICRS(base_date_);
    test_coords(ecl2icrs, base_icrs_, __func__, __LINE__);

    CESkyCoord::Ecliptic2ICRS(base_ecl_, &ecl2icrs, base_date_);
    test_coords(ecl2icrs, base_icrs_, __func__, __LINE__);

    return pass();
}

/**********************************************************************//**
 * Test ability to convert to Galactic
 *************************************************************************/
bool test_CESkyCoord::test_Convert2Galactic()
{
    // CIRS -> Galactic
    CESkyCoord cirs2gal = base_cirs_.ConvertToGalactic(base_date_);
    test_coords(cirs2gal, base_gal_, __func__, __LINE__);

    CESkyCoord::CIRS2Galactic(base_cirs_, &cirs2gal, base_date_);
    test_coords(cirs2gal, base_gal_, __func__, __LINE__);    

    // ICRS -> Galactic
    CESkyCoord icrs2gal = base_icrs_.ConvertToGalactic();
    test_coords(icrs2gal, base_gal_, __func__, __LINE__);

    CESkyCoord::ICRS2Galactic(base_icrs_, &icrs2gal);
    test_coords(icrs2gal, base_gal_, __func__, __LINE__);

    // Galactic -> Galactic
    CESkyCoord gal2gal = base_gal_.ConvertToGalactic();
    test_coords(gal2gal, base_gal_, __func__, __LINE__);

    // Observed -> Galactic
    CESkyCoord obs2gal = base_obs_.ConvertToGalactic(base_date_, base_observer_);
    test_coords(obs2gal, base_gal_, __func__, __LINE__);

    CESkyCoord::Observed2Galactic(base_obs_, &obs2gal, base_date_, base_observer_);
    test_coords(obs2gal, base_gal_, __func__, __LINE__);

    // Ecliptic -> Galactic
    CESkyCoord ecl2gal = base_ecl_.ConvertToGalactic(base_date_);
    test_coords(ecl2gal, base_gal_, __func__, __LINE__);

    CESkyCoord::Ecliptic2Galactic(base_ecl_, &ecl2gal, base_date_);
    test_coords(ecl2gal, base_gal_, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test ability to convert to Observed
 *************************************************************************/
bool test_CESkyCoord::test_Convert2Observed()
{
    // Preliminary variable for observer based conversion
    CESkyCoord testobs;

    // CIRS -> Observed
    CESkyCoord cirs2obs = base_cirs_.ConvertToObserved(base_date_, base_observer_);
    test_coords(cirs2obs, base_obs_, __func__, __LINE__);

    CESkyCoord::CIRS2Observed(base_cirs_, &testobs, base_date_, base_observer_);
    test_coords(testobs, base_obs_, __func__, __LINE__);

    // ICRS -> Observed
    CESkyCoord icrs2obs = base_icrs_.ConvertToObserved(base_date_, base_observer_);
    test_coords(icrs2obs, base_obs_, __func__, __LINE__);

    CESkyCoord::ICRS2Observed(base_icrs_, &testobs, base_date_, base_observer_);
    test_coords(testobs, base_obs_, __func__, __LINE__);

    // Galactic -> Observed
    CESkyCoord gal2obs = base_gal_.ConvertToObserved(base_date_, base_observer_);
    test_coords(gal2obs, base_obs_, __func__, __LINE__);

    CESkyCoord::Galactic2Observed(base_gal_, &testobs, base_date_, base_observer_);
    test_coords(testobs, base_obs_, __func__, __LINE__);

    // Observed -> Observed
    CESkyCoord obs2obs = base_obs_.ConvertToObserved();
    test_coords(obs2obs, base_obs_, __func__, __LINE__);

    // Ecliptic -> Observed
    CESkyCoord ecl2obs = base_ecl_.ConvertToObserved(base_date_, base_observer_);
    test_coords(ecl2obs, base_obs_, __func__, __LINE__);

    CESkyCoord::Ecliptic2Observed(base_ecl_, &ecl2obs, base_date_, base_observer_);
    test_coords(ecl2obs, base_obs_, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test ability to convert to Ecliptic
 *************************************************************************/
bool test_CESkyCoord::test_Convert2Ecliptic()
{
    // CIRS -> Ecliptic
    CESkyCoord cirs2ecl = base_cirs_.ConvertToEcliptic(base_date_);
    test_coords(cirs2ecl, base_ecl_, __func__, __LINE__);

    CESkyCoord::CIRS2Ecliptic(base_cirs_, &cirs2ecl, base_date_);
    test_coords(cirs2ecl, base_ecl_, __func__, __LINE__);    

    // ICRS -> Ecliptic
    CESkyCoord icrs2ecl = base_icrs_.ConvertToEcliptic(base_date_);
    test_coords(icrs2ecl, base_ecl_, __func__, __LINE__);

    CESkyCoord::ICRS2Ecliptic(base_icrs_, &icrs2ecl, base_date_);
    test_coords(icrs2ecl, base_ecl_, __func__, __LINE__);

    // Galactic -> Ecliptic
    CESkyCoord gal2ecl = base_gal_.ConvertToEcliptic(base_date_);
    test_coords(gal2ecl, base_ecl_, __func__, __LINE__);

    // Observed -> Ecliptic
    CESkyCoord obs2ecl = base_obs_.ConvertToEcliptic(base_date_, base_observer_);
    test_coords(obs2ecl, base_ecl_, __func__, __LINE__);

    CESkyCoord::Observed2Ecliptic(base_obs_, &obs2ecl, base_date_, base_observer_);
    test_coords(obs2ecl, base_ecl_, __func__, __LINE__);

    // Ecliptic -> Ecliptic
    CESkyCoord ecl2ecl = base_ecl_.ConvertToEcliptic();
    test_coords(ecl2ecl, base_ecl_, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test computation of angular separation
 *************************************************************************/
bool test_CESkyCoord::test_AngularSeparation(void)
{
    // Establish the preliminary variables that will be used 
    CEAngle test1_x = CEAngle::Deg(0.0);
    CEAngle test1_y = CEAngle::Deg(-1.0);
    CEAngle test2_x = CEAngle::Deg(0.0);
    CEAngle test2_y = CEAngle::Deg(1.0);
    CESkyCoord test1(test1_x, test1_y, CESkyCoordType::ICRS);
    CESkyCoord test2(test2_x, test2_y, CESkyCoordType::ICRS);
    
    // Test the default coordinate separation
    CEAngle angsep = test1.Separation(test2);
    test_double(angsep.Deg(), 2.0, __func__, __LINE__);

    // 2nd static Method not requiring CESkyCoord objects
    angsep = CESkyCoord::AngularSeparation(test1_x, test1_y,
                                           test2_x, test2_y);
    test_double(angsep.Deg(), 2.0, __func__, __LINE__);

    // Test that the two coordinates are not equal
    test((test1 != test2), __func__, __LINE__);
    
    // Test that it fails if the coordinates are not the same type
    try {
        angsep = base_icrs_.Separation(base_gal_);
        test(false, __func__, __LINE__);
    } catch (std::exception &e) {
        test(true, __func__, __LINE__);
    }

    return pass();
}


/**********************************************************************//**
 * Tests the CESkyCoord::ConvertTo methods
 *************************************************************************/
bool test_CESkyCoord::test_ConvertTo(void)
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
    CESkyCoord test_icrs = base_icrs_.ConvertTo(CESkyCoordType::ICRS,
                                                date, observer);
    test_coords(test_icrs, base_icrs_, __func__, __LINE__);
    CESkyCoord test_cirs = base_icrs_.ConvertTo(CESkyCoordType::CIRS,
                                                date, observer);
    test_coords(test_cirs, base_cirs_, __func__, __LINE__);
    CESkyCoord test_gal = base_icrs_.ConvertTo(CESkyCoordType::GALACTIC,
                                               date, observer);
    test_coords(test_gal, base_gal_, __func__, __LINE__);
    CESkyCoord test_obs = base_icrs_.ConvertTo(CESkyCoordType::OBSERVED,
                                               date, observer);
    test_coords(test_obs, base_obs_, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Tests two coordinates are equal and print some help if they aren't
 *************************************************************************/
bool test_CESkyCoord::test_coords(const CESkyCoord&  test,
                                  const CESkyCoord&  expected,
                                  const std::string& func,
                                  const int&         line)
{
    // Test if the coordinates are equal
    bool pass = test_bool((test == expected), true, func, line);
    if (!pass) {
        std::printf("     CoordSys (1 v 2): %d v %d\n",
                    int(test.GetCoordSystem()), int(expected.GetCoordSystem()));
        std::printf("     XCoords: %3.15e : %3.15e (deg), diff = %e arcsec\n", 
                    test.XCoord().Deg(), expected.XCoord().Deg(), 
                    (test.XCoord().Deg()-expected.XCoord().Deg())*3600.0);
        std::printf("     YCoords: %3.15e : %3.15e (deg), diff = %e arcsec\n", 
                    test.YCoord().Deg(), expected.YCoord().Deg(), 
                    (test.YCoord().Deg()-expected.YCoord().Deg())*3600.0);
        std::printf("     AngSep: %e arcsec\n", 
                    test.Separation(expected).Deg()*3600.0);
    }
    return pass;
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) 
{
    test_CESkyCoord tester;
    return (!tester.runtests());
}

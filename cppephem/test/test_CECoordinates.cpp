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
    base_icrs_ = CECoordinates(CEAngle::Deg(83.633), CEAngle::Deg(22.0145), 
                               CECoordinateType::ICRS);
    base_cirs_ = CECoordinates(CEAngle::Deg(83.63843844654943), 
                               CEAngle::Deg(22.012942530431776),
                               CECoordinateType::CIRS);
    base_gal_  = CECoordinates(CEAngle::Deg(184.55741630955762), 
                               CEAngle::Deg(-5.784421958594916),
                               CECoordinateType::GALACTIC);
    base_obs_  = CECoordinates(CEAngle::Deg(35.598599384274294), 
                               CEAngle::Deg(152.55068501449307),
                               CECoordinateType::OBSERVED);

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
    test_double(test1.XCoord(), 0.0, __func__, __LINE__);
    test_double(test1.YCoordinate_Rad(), 0.0, __func__, __LINE__);
    test_double(test1.YCoord(), 0.0, __func__, __LINE__);
    
    // Test constructor from CEAngle objects
    CEAngle testx = CEAngle::Deg(83.633);
    CEAngle testy = CEAngle::Deg(22.0145);
    CECoordinates test2(testx, testy, CECoordinateType::ICRS);
    test_double(test2.XCoordinate_Rad(), testx, __func__, __LINE__);
    test_double(test2.YCoordinate_Rad(), testy, __func__, __LINE__);
    
    // Test constructor from vector objects
    CECoordinates test3a(testx.HmsVect(), testy.DmsVect(),
                        CECoordinateType::ICRS);
    test_double(test3a.XCoordinate_Rad(), testx, __func__, __LINE__);
    test_double(test3a.YCoordinate_Rad(), testy, __func__, __LINE__);
    CECoordinates test3b(testx.DmsVect(), testy.DmsVect(),
                        CECoordinateType::GALACTIC);
    test_double(test3b.XCoordinate_Rad(), testx, __func__, __LINE__);
    test_double(test3b.YCoordinate_Rad(), testy, __func__, __LINE__);

    // Test copy constructor
    CECoordinates test4(test2);
    test_double(test4.XCoordinate_Rad(), testx, __func__, __LINE__);
    test_double(test4.YCoordinate_Rad(), testy, __func__, __LINE__);
    test_int(int(test4.GetCoordSystem()), int(test2.GetCoordSystem()), __func__, __LINE__);

    // Test constructor from coordinate type
    CECoordinates test5(CECoordinateType::GALACTIC);
    test_int(int(test5.GetCoordSystem()), int(CECoordinateType::GALACTIC), __func__, __LINE__);

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
    double ra(0.0);
    double dec(0.0);

    // CIRS -> CIRS
    CECoordinates cirs2cirs = base_cirs_.ConvertToCIRS();
    test_coords(cirs2cirs, base_cirs_, __func__, __LINE__);

    // ICRS -> CIRS
    CECoordinates icrs2cirs = base_icrs_.ConvertToCIRS(base_date_);
    test_coords(icrs2cirs, base_cirs_, __func__, __LINE__);

    CECoordinates::ICRS2CIRS(base_icrs_.XCoordinate_Deg(), 
                             base_icrs_.YCoordinate_Deg(),
                             &ra, &dec, base_date_, CEAngleType::DEGREES);
    icrs2cirs.SetCoordinates(CEAngle::Deg(ra), CEAngle::Deg(dec), 
                             CECoordinateType::CIRS);
    test_coords(icrs2cirs, base_cirs_, __func__, __LINE__);

    // Galactic -> CIRS
    CECoordinates gal2cirs = base_gal_.ConvertToCIRS(base_date_);
    test_coords(gal2cirs, base_cirs_, __func__, __LINE__);

    ra  = 0.0;
    dec = 0.0;
    CECoordinates::Galactic2CIRS(base_gal_.XCoordinate_Deg(), 
                                 base_gal_.YCoordinate_Deg(),
                                 &ra, &dec, base_date_, CEAngleType::DEGREES);
    icrs2cirs.SetCoordinates(CEAngle::Deg(ra), CEAngle::Deg(dec), 
                             CECoordinateType::CIRS);
    test_coords(icrs2cirs, base_cirs_, __func__, __LINE__);

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

    // Observed -> CIRS (direct v1)
    ra  = 0.0;
    dec = 0.0;
    CECoordinates::Observed2CIRS(base_obs_.XCoordinate_Deg(), 
                                 base_obs_.YCoordinate_Deg(),
                                 &ra, &dec, base_date_, base_observer_,
                                 CEAngleType::DEGREES);
    obs2cirs.SetCoordinates(CEAngle::Deg(ra), CEAngle::Deg(dec), 
                             CECoordinateType::CIRS);
    test_coords(obs2cirs, base_cirs_, __func__, __LINE__);

    // Observed -> CIRS (direct v2)
    ra  = 0.0;
    dec = 0.0;
    CECoordinates::Observed2CIRS(base_obs_.XCoordinate_Rad(), 
                                 base_obs_.YCoordinate_Rad(),
                                 &ra, &dec, base_date_.JD(), 
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
    obs2cirs.SetCoordinates(CEAngle::Rad(ra), CEAngle::Rad(dec), 
                             CECoordinateType::CIRS);
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

    double ra(0.0), dec(0.0);
    CECoordinates::CIRS2ICRS(base_cirs_.XCoord().Deg(), base_cirs_.YCoord().Deg(),
                             &ra, &dec, base_date_, CEAngleType::DEGREES);
    cirs2icrs.SetCoordinates(CEAngle::Deg(ra), CEAngle::Deg(dec), 
                             CECoordinateType::ICRS);
    test_coords(cirs2icrs, base_icrs_, __func__, __LINE__);

    // ICRS -> ICRS
    CECoordinates icrs2icrs = base_icrs_.ConvertToICRS();
    test_coords(icrs2icrs, base_icrs_, __func__, __LINE__);

    // Galactic -> ICRS
    CECoordinates gal2icrs = base_gal_.ConvertToICRS();
    test_coords(gal2icrs, base_icrs_, __func__, __LINE__);

    ra  = 0.0;
    dec = 0.0;
    CECoordinates::Galactic2ICRS(base_gal_.XCoord().Deg(), base_gal_.YCoord().Deg(),
                                 &ra, &dec, CEAngleType::DEGREES);
    gal2icrs.SetCoordinates(CEAngle::Deg(ra), CEAngle::Deg(dec),
                            CECoordinateType::ICRS);
    test_coords(gal2icrs, base_icrs_, __func__, __LINE__);

    // Observed -> ICRS (raw values)
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

    // Observed -> ICRS (CEObserver object)
    CECoordinates::Observed2ICRS(base_obs_.XCoord().Deg(),
                                 base_obs_.YCoord().Deg(),
                                 &ra, &dec,
                                 base_date_, base_observer_,
                                 CEAngleType::DEGREES);
    obs2icrs = CECoordinates(CEAngle::Deg(ra), CEAngle::Deg(dec),
                             CECoordinateType::ICRS);
    test_coords(obs2icrs, base_icrs_, __func__, __LINE__);

    return pass();
}

/**********************************************************************//**
 * Test ability to convert to Galactic
 *************************************************************************/
bool test_CECoordinates::test_Convert2Galactic()
{
    double glon(0.0), glat(0.0);

    // CIRS -> Galactic
    CECoordinates cirs2gal = base_cirs_.ConvertToGalactic(base_date_);
    test_coords(cirs2gal, base_gal_, __func__, __LINE__);

    glon = 0.0;
    glat = 0.0;
    CECoordinates::CIRS2Galactic(base_cirs_.XCoord().Deg(), base_cirs_.YCoord().Deg(),
                                 &glon, &glat, base_date_, CEAngleType::DEGREES);
    cirs2gal.SetCoordinates(CEAngle::Deg(glon), CEAngle::Deg(glat),
                            CECoordinateType::GALACTIC);
    test_coords(cirs2gal, base_gal_, __func__, __LINE__);

    // ICRS -> Galactic
    CECoordinates icrs2gal = base_icrs_.ConvertToGalactic();
    test_coords(icrs2gal, base_gal_, __func__, __LINE__);

    glon = 0.0;
    glat = 0.0;
    CECoordinates::ICRS2Galactic(base_icrs_.XCoord().Deg(), base_icrs_.YCoord().Deg(),
                                 &glon, &glat, CEAngleType::DEGREES);
    icrs2gal.SetCoordinates(CEAngle::Deg(glon), CEAngle::Deg(glat),
                            CECoordinateType::GALACTIC);
    test_coords(icrs2gal, base_gal_, __func__, __LINE__);

    // Galactic -> Galactic
    CECoordinates gal2gal = base_gal_.ConvertToGalactic();
    test_coords(gal2gal, base_gal_, __func__, __LINE__);

    // Observed -> Galactic (raw values)
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

    // Observed -> Galactic (CEObserver values)
    CECoordinates::Observed2Galactic(base_obs_.XCoord().Deg(),
                                     base_obs_.YCoord().Deg(),
                                     &glon, &glat,
                                     base_date_, base_observer_,
                                     CEAngleType::DEGREES);
    obs2gal = CECoordinates(CEAngle::Deg(glon), CEAngle::Deg(glat),
                            CECoordinateType::GALACTIC);
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
    // 'GetObservedCoords' method
    cirs2obs = base_cirs_.GetObservedCoords(base_date_, base_observer_);
    test_coords(cirs2obs, base_obs_, __func__, __LINE__);
    // Test the 'CEObserver' based approach
    CECoordinates::CIRS2Observed(base_cirs_.XCoordinate_Deg(),
                                 base_cirs_.YCoordinate_Deg(),
                                 &az, &zen,
                                 base_date_, base_observer_, 
                                 CEAngleType::DEGREES,
                                 base_observer_.Wavelength_um());
    testobs.SetCoordinates(CEAngle::Deg(az), CEAngle::Deg(zen), 
                           CECoordinateType::OBSERVED);
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
    // 'GetObservedCoords' method
    icrs2obs = base_icrs_.GetObservedCoords(base_date_, base_observer_);
    test_coords(icrs2obs, base_obs_, __func__, __LINE__);
    // Test the 'CEObserver' based approach
    CECoordinates::ICRS2Observed(base_icrs_.XCoordinate_Deg(),
                                 base_icrs_.YCoordinate_Deg(),
                                 &az, &zen,
                                 base_date_, base_observer_, 
                                 CEAngleType::DEGREES,
                                 base_observer_.Wavelength_um());
    testobs.SetCoordinates(CEAngle::Deg(az), CEAngle::Deg(zen), 
                           CECoordinateType::OBSERVED);
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
    // 'GetObservedCoords' method
    gal2obs = base_gal_.GetObservedCoords(base_date_, base_observer_);
    test_coords(gal2obs, base_obs_, __func__, __LINE__);
    // Test the 'CEObserver' based approach
    CECoordinates::Galactic2Observed(base_gal_.XCoordinate_Deg(),
                                     base_gal_.YCoordinate_Deg(),
                                     &az, &zen,
                                     base_date_, base_observer_, 
                                     CEAngleType::DEGREES,
                                     base_observer_.Wavelength_um());
    testobs.SetCoordinates(CEAngle::Deg(az), CEAngle::Deg(zen), 
                           CECoordinateType::OBSERVED);
    test_coords(testobs, base_obs_, __func__, __LINE__);

    // Observed -> Observed
    try {
        CECoordinates obs2obs = base_obs_.ConvertToObserved();
        test(false, __func__, __LINE__);
    } catch (std::exception &e) {
        test(true, __func__, __LINE__);
    }
    // 'GetObservedCoords' method
    CECoordinates obs2obs = base_obs_.GetObservedCoords(base_date_, base_observer_);
    test_coords(obs2obs, base_obs_, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test conversions between HMS and DMS to angle and back
 *************************************************************************/
bool test_CECoordinates::test_HmsDms2Angle(void)
{
    // Create some variables for testing
    double              ang_var(45.50833333333333);
    double              ang_rad = CEAngle::Deg(ang_var).Rad();
    std::vector<double> ang_vec = {45, 30, 30, 0.0};
    std::vector<double> hrs_vec = {3.0, 2.0, 2.0, 0.0};

    // Increase the acceptable tolerance
    double tol_old(DblTol());
    SetDblTol(1.0e-11);

    // Test angle -> DMS 
    std::vector<double> test1 = CECoordinates::GetDMS(ang_var, CEAngleType::DEGREES);
    test_vect(test1, ang_vec, __func__, __LINE__);

    // Test DMS -> angle
    double test2a(CECoordinates::DMSToAngle(ang_vec, CEAngleType::DEGREES));
    test_double(test2a, ang_var, __func__, __LINE__);
    double test2b(CECoordinates::DMSToAngle(ang_vec, CEAngleType::RADIANS));
    test_double(test2b, ang_rad, __func__, __LINE__);
    
    // Test angle -> HMS 
    std::vector<double> test3 = CECoordinates::GetHMS(ang_var, CEAngleType::DEGREES);
    test_vect(test3, hrs_vec, __func__, __LINE__);

    // Test HMS -> angle
    double test4a(CECoordinates::HMSToAngle(hrs_vec, CEAngleType::DEGREES));
    test_double(test4a, ang_var, __func__, __LINE__);
    double test4b(CECoordinates::HMSToAngle(hrs_vec, CEAngleType::RADIANS));
    test_double(test4b, ang_rad, __func__, __LINE__);

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
    CEAngle test1_x = CEAngle::Deg(0.0);
    CEAngle test1_y = CEAngle::Deg(-1.0);
    CEAngle test2_x = CEAngle::Deg(0.0);
    CEAngle test2_y = CEAngle::Deg(1.0);
    CECoordinates test1(test1_x, test1_y, CECoordinateType::ICRS);
    CECoordinates test2(test2_x, test2_y, CECoordinateType::ICRS);
    
    // Test the default coordinate separation
    CEAngle angsep = test1.AngularSeparation(test2);
    test_double(angsep.Deg(), 2.0, __func__, __LINE__);
    angsep = CECoordinates::AngularSeparation(test1, test2);
    test_double(angsep.Deg(), 2.0, __func__, __LINE__);
    angsep = CECoordinates::AngularSeparation(test1_x, test1_y,
                                              test2_x, test2_y);
    test_double(angsep.Deg(), 2.0, __func__, __LINE__);

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
                    test.AngularSeparation(expected).Deg()*3600.0);
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

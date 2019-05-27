/***************************************************************************
 *  test_CEObserver.cpp: CppEphem                                          *
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

/** \class test_CEObserver
 Defines tests for CEObserver
 */

#include <iostream>

#include "test_CEObserver.h"
#include "CECoordinates.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CEObserver::test_CEObserver() :
    CETestSuite()
{
    base_obs_ = CEObserver(0.0, 0.0, 0.0, CEAngleType::DEGREES);
    base_obs_.SetWavelength_um(1.0);
    base_obs_.SetTemperature_C(0.0);
    base_obs_.SetPressure_hPa(0.0);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CEObserver::~test_CEObserver()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEObserver::runtests()
{
    std::cout << "\nTesting CEObserver:\n";

    // Run each of the tests
    test_constructor();
    test_set_geoCoords();
    test_set_atmoPars();
    test_get_obsCoords();
    test_time();

    return pass();
}


/**********************************************************************//**
 * Test that we can set the observer latitude,longitude coordinates
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEObserver::test_constructor(void)
{
    // Default constructor
    CEObserver test1;
    test_double(test1.Longitude_Rad(), 0.0, __func__, __LINE__);
    test_double(test1.Latitude_Rad(),  0.0,  __func__, __LINE__);
    test_double(test1.Temperature_C(), CppEphem::SeaLevelTemp_C(), __func__, __LINE__);
    test_double(test1.Pressure_hPa(),  CppEphem::EstimatePressure_hPa(test1.Elevation_m()),  __func__, __LINE__);
    test_double(test1.RelativeHumidity(), 0.0, __func__, __LINE__);
    test_double(test1.Wavelength_um(), 0.5, __func__, __LINE__);

    // Create a copy for testing
    CEObserver test2(base_obs_);
    test_double(test2.Longitude_Rad(), base_obs_.Longitude_Rad(), __func__, __LINE__);
    test_double(test2.Latitude_Rad(),  base_obs_.Latitude_Rad(),  __func__, __LINE__);
    test_double(test2.Temperature_K(), base_obs_.Temperature_K(), __func__, __LINE__);
    test_double(test2.Pressure_hPa(),  base_obs_.Pressure_hPa(),  __func__, __LINE__);
    test_double(test2.RelativeHumidity(), base_obs_.RelativeHumidity(), __func__, __LINE__);
    test_double(test2.Wavelength_um(), base_obs_.Wavelength_um(), __func__, __LINE__);

    // Copy assignment operator
    CEObserver test3 = base_obs_;
    test_double(test3.Longitude_Rad(), base_obs_.Longitude_Rad(), __func__, __LINE__);
    test_double(test3.Latitude_Rad(),  base_obs_.Latitude_Rad(),  __func__, __LINE__);
    test_double(test3.Temperature_K(), base_obs_.Temperature_K(), __func__, __LINE__);
    test_double(test3.Pressure_hPa(),  base_obs_.Pressure_hPa(),  __func__, __LINE__);
    test_double(test3.RelativeHumidity(), base_obs_.RelativeHumidity(), __func__, __LINE__);
    test_double(test3.Wavelength_um(), base_obs_.Wavelength_um(), __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test that we can set the observer latitude,longitude coordinates
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEObserver::test_set_geoCoords()
{
    // Create a copy with different values for testing
    CEObserver obs(base_obs_);

    // Test longitude
    double lon  = obs.Longitude_Deg() + 10.0;
    obs.SetLongitude(lon, CEAngleType::DEGREES);
    test_double(obs.Longitude_Deg(), lon,  __func__, __LINE__);

    // Test latitude
    double lat  = obs.Latitude_Deg() + 10.0;
    obs.SetLatitude(lat, CEAngleType::DEGREES);
    test_double(obs.Latitude_Deg(),  lat,  __func__, __LINE__);

    // Set both longitude and latitude
    obs.SetGeoCoordinates(lon-10.0, lat-10.0, CEAngleType::DEGREES);
    test_double(obs.Longitude_Deg(), base_obs_.Longitude_Deg(), __func__, __LINE__);
    test_double(obs.Latitude_Deg(), base_obs_.Latitude_Deg(), __func__, __LINE__);

    // Test elevation
    double elev = obs.Elevation_m() + 10.0;
    obs.SetElevation(elev);
    test_double(obs.Elevation_m(),   elev, __func__, __LINE__);
    
    // Test setting UTC offset
    double utc_offset = obs.UTCOffset() + 2.5;
    obs.SetUTCOffset(utc_offset);
    test_double(obs.UTCOffset(), utc_offset, __func__, __LINE__);

    // Test that we get similar positions to Astropy
    CEDate date(CppEphem::julian_date_J2000(), CEDateType::JD);
    std::vector<double> earth_pos_m = {6378137.00000, 0.0, 0.0};
    std::vector<double> test_pos_m  = base_obs_.PositionGeo();
    test_vect(test_pos_m, earth_pos_m, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test that we can set the observer latitude,longitude coordinates
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEObserver::test_set_atmoPars()
{
    // Cache the tolerance
    double old_tol(DblTol());
    SetDblTol(1.0e-11);

    // Create a copy with different values for testing
    CEObserver obs(base_obs_);

    // Test temperature
    double temp_K = obs.Temperature_K() + 10.0;
    double temp_C = CppEphem::Temp_K2C(temp_K);
    double temp_F = CppEphem::Temp_K2F(temp_K);
    obs.SetTemperature_K(temp_K);
    test_double(obs.Temperature_K(), temp_K, __func__, __LINE__);
    test_double(obs.Temperature_C(), temp_C, __func__, __LINE__);
    test_double(obs.Temperature_F(), temp_F, __func__, __LINE__);

    // Test Temperature setting
    temp_F += 10.0;
    obs.SetTemperature_F(temp_F);
    test_double(obs.Temperature_F(), temp_F, __func__, __LINE__);
    temp_C -= 10.0;
    obs.SetTemperature_C(temp_C);
    test_double(obs.Temperature_C(), temp_C, __func__, __LINE__);

    // Test pressure
    double pres = obs.Pressure_hPa() + 10.0;
    obs.SetPressure_hPa(pres);
    test_double(obs.Pressure_hPa(),  pres, __func__, __LINE__);

    // Test Relative humidity
    double relHumidity = obs.RelativeHumidity();
    if (relHumidity == 0.25) {
        relHumidity = 0.5;
    } else {
        relHumidity = 0.25;
    }
    obs.SetRelativeHumidity(relHumidity);
    test_double(obs.RelativeHumidity(),  relHumidity, __func__, __LINE__);
    
    // Test wavelength
    double wavelength = obs.Wavelength_um() + 10.0;
    obs.SetWavelength_um(wavelength);
    test_double(obs.Wavelength_um(), wavelength, __func__, __LINE__);

    // Make sure the print statement actually does something
    test(obs.print().size() > 0, __func__, __LINE__);

    // Reset the tolerance
    SetDblTol(old_tol);
    return pass();
}


/**********************************************************************//**
 * Test that we can get correct observed coordinates
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEObserver::test_get_obsCoords(void)
{
    CEBody body("North Pole", CEAngle::Deg(0.0), CEAngle::Deg(90.0), 
                CECoordinateType::ICRS);
    CEDate date(CppEphem::julian_date_J2000(), CEDateType::JD);
    CECoordinates obs_coords = base_obs_.ObservedPosition(body, date);

    // Note that these test coordinates were derived using Astropy
    // to ensure the values returned are correct
    CECoordinates test_coords(CEAngle::Deg(359.9960211433963), 
                              CEAngle::Deg(90.00137453000666), 
                              CECoordinateType::OBSERVED);

    // Make sure observed coordinates are near where we expect them to be
    CEAngle angsep = CECoordinates::AngularSeparation(obs_coords, test_coords);
    // Make sure the accuracy is within 0.5 arcsecond
    if (!test_lessthan(angsep.Rad(), 0.5*DAS2R, __func__, __LINE__)) {
        std::printf("date: %f\n", date.Gregorian());
        std::printf("dut1,x,y: %f sec, %e rad, %e rad\n", date.dut1(), date.xpolar(), date.ypolar());
        std::printf("Obs %s", obs_coords.print().c_str());
        std::printf("Test %s", test_coords.print().c_str());
        std::printf("Angsep: %e deg\n", angsep.Deg());
        std::printf("%s", base_obs_.print().c_str());
    }

    return pass();
}


/**********************************************************************//**
 * Test that the time methods are valid
 * 
 * @return Whether or not all tests succeeded
 *************************************************************************/
bool test_CEObserver::test_time(void)
{
    double offset = base_obs_.UTCOffset();
    double old_tol(DblTol());
    SetDblTol(1.0e-12);

    // Make sure the time is noon on J2000
    CEDate date(CppEphem::julian_date_J2000());
    std::vector<double> local = base_obs_.Time( date );
    std::vector<double> utc   = base_obs_.Time_UTC( date );
    
    test_vect(local, {offset+12, 0.0, 0.0, 0.0}, __func__, __LINE__);
    test_vect(utc,   {12.0, 0.0, 0.0, 0.0}, __func__, __LINE__);

    SetDblTol(old_tol);
    return pass();
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) 
{
    test_CEObserver tester;
    return (!tester.runtests());
}

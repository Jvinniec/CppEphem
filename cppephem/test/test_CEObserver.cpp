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
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CEObserver::test_CEObserver() :
    CETestSuite()
{
    base_obs_ = CEObserver(0.0, 0.0, 0.0, CEAngleType::DEGREES);
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
    test_copy();
    test_set_geoCoords();
    test_set_atmoPars();

    return pass();
}


/**********************************************************************//**
 * Test that we can set the observer latitude,longitude coordinates
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEObserver::test_copy()
{
    // Create a copy for testing
    CEObserver obs(base_obs_);
    test_double(obs.Longitude_Rad(), base_obs_.Longitude_Rad(), __func__, __LINE__);
    test_double(obs.Latitude_Rad(),  base_obs_.Latitude_Rad(),  __func__, __LINE__);
    test_double(obs.Temperature_K(), base_obs_.Temperature_K(), __func__, __LINE__);
    test_double(obs.Pressure_hPa(),  base_obs_.Pressure_hPa(),  __func__, __LINE__);
    test_double(obs.RelativeHumidity(), base_obs_.RelativeHumidity(), __func__, __LINE__);

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
    test_double(obs.Longitude_Deg(), lon,  __FILE__, __LINE__);

    // Test latitude
    double lat  = obs.Latitude_Deg() + 10.0;
    obs.SetLatitude(lat, CEAngleType::DEGREES);
    test_double(obs.Latitude_Deg(),  lat,  __FILE__, __LINE__);

    // Test elevation
    double elev = obs.Elevation_m() + 10.0;
    obs.SetElevation(elev);
    test_double(obs.Elevation_m(),   elev, __FILE__, __LINE__);
    
    // Test setting lat,lon together

    return pass();
}


/**********************************************************************//**
 * Test that we can set the observer latitude,longitude coordinates
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEObserver::test_set_atmoPars()
{
    // Create a copy with different values for testing
    CEObserver obs(base_obs_);

    // Test temperature
    double temp = obs.Temperature_K() + 10.0;
    obs.SetTemperature_K(temp);
    test_double(obs.Temperature_K(), temp, __func__, __LINE__);

    // Test pressure
    double pres = obs.Pressure_hPa() + 10.0;
    obs.SetPressure(pres);
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
    
    return pass();
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) {
    test_CEObserver tester;
    return (!tester.runtests());
}
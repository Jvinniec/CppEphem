/***************************************************************************
 *  CEObserver.cpp: CppEphem                                               *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016-2019 JCardenzana                                      *
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

/** \class CEObserver
 CEObserver class is a self contained object for describing an observer.
 Information relevant to the observer includes:
 - Geographic longitude  (East positive)
 - Geographic latitude
 - Elevation (meters above sea level)
 - Atmospheric pressure (hPa)
 - Temperature
 - Relative humidity
 
 The above are used in the SOFA software to more accurately calculate
 the affect of refraction through the atmosphere.
 */

// C++ HEADERS
#include <stdio.h>

// CPPEPHEM HEADERS
#include "CEObserver.h"


/**********************************************************************//**
 * Constructor from a geographic position (latitude, longitude, altitude)
 * Note that altitude defaults to sea-level
 * 
 * @param[in] longitude         Observer longitude (east positive)
 * @param[in] latitude          Observer latitude
 * @param[in] elevation         Observer elevation above sea-level (meters)
 * @param[in] angle_type        Angle type for longitude and latitude (RADIANS or DEGREES)
 *************************************************************************/
CEObserver::CEObserver(const double& longitude, 
                       const double& latitude,
                       const double& elevation, 
                       CEAngleType   angle_type)
{
    // Use the internal methods for setting the values
    SetLongitude(longitude, angle_type) ;
    SetLatitude(latitude, angle_type) ;
    SetElevation(elevation) ;
    SetPressure_hPa(CppEphem::EstimatePressure_hPa(elevation_m_)) ;
    SetTemperature_C() ;
    SetRelativeHumidity() ;
}


/**********************************************************************//**
 * Copy constructor
 * 
 * @param[in] other            Another observer object that will be copied
 *************************************************************************/
CEObserver::CEObserver(const CEObserver& other) :
    longitude_(other.longitude_),
    latitude_(other.latitude_),
    elevation_m_(other.elevation_m_),
    pressure_hPa_(other.pressure_hPa_),
    temperature_celsius_(other.temperature_celsius_),
    relative_humidity_(other.relative_humidity_)
{
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
CEObserver::~CEObserver()
{
}


/**********************************************************************//**
 * Get the local sky coordinates for an object as observed
 * by this observer
 * 
 * @param[in] object       Object that we want to find the local coordinates of
 * @return                 Local coordinates of the object
 *************************************************************************/
CECoordinates CEObserver::ObservedPosition(const CEBody& object,
                                           const CEDate& date)
{
    CECoordinates coords = object.GetCoordinates(date);
    return this->ObservedPosition(coords, date);
}


/**********************************************************************//**
 * Get the local sky coordinates for an object as observed
 * by this observer
 * @param[in] coords       Coordinates of an object that we want to find the local coordinates of
 * @return                 'coords' converted into the local coordinates of this observer.
 *************************************************************************/
CECoordinates CEObserver::ObservedPosition(const CECoordinates& coords, 
                                           const CEDate&        date)
{
    // Compute the observed coordinates for these coordinates
    CECoordinates tmpcoords(coords);
    CECoordinates observed_coords = tmpcoords.GetObservedCoords(date,*this);
    return observed_coords;
}

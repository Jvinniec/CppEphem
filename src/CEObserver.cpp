/***************************************************************************
 *  CEObserver.cpp: CppEphem                                               *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016 JCardenzana                                           *
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
 * @param longitude            Observer longitude (east positive)
 * @param latitude             Observer latitude
 * @param elevation            Observer elevation above sea-level (meters)
 * @param angle_type           Angle type for longitude and latitude (RADIANS or DEGREES)
 * @param date                 Current date for the observer
 *************************************************************************/
CEObserver::CEObserver(double longitude, double latitude,
                       double elevation, CppEphem::CEAngleType angle_type,
                       CEDate* date)
{
    // Set the date
    SetDate(date) ;
    
    // Use the internal methods for setting the values
    SetLongitude(longitude, angle_type) ;
    SetLatitude(latitude, angle_type) ;
    SetElevation(elevation) ;
    SetPressure(CppEphem::EstimatePressure_hPa(elevation_m_)) ;
    SetTemperature_C() ;
    SetRelativeHumidity() ;
    SetDate(date) ;
}

/**********************************************************************//**
 * Copy constructor
 * 
 * @param other            Another observer object that will be copied
 *************************************************************************/
CEObserver::CEObserver(const CEObserver& other) :
    longitude_(other.longitude_),
    latitude_(other.latitude_),
    elevation_m_(other.elevation_m_),
    pressure_hPa_(other.pressure_hPa_),
    temperature_celsius_(other.temperature_celsius_),
    relative_humidity_(other.relative_humidity_)
{}

/**********************************************************************//**
 * Destructor
 *************************************************************************/
CEObserver::~CEObserver()
{
    // Delete the date object if we own it
    if (date_is_owned_) delete current_date_ ;
}

#pragma mark - Public Methods

/**********************************************************************//**
 * Get the local sky coordinates for an object as observed
 * by this observer
 * 
 * @param object           Object that we want to find the local coordinates of
 * @return                 Local coordinates of the object
 *************************************************************************/
CECoordinates CEObserver::ObservedPosition(CEBody& object)
{
    CECoordinates coords = object.GetCoordinates() ;
    return ObservedPosition(coords) ;
}

/**********************************************************************//**
 * Get the local sky coordinates for an object as observed
 * by this observer
 * @param coords           Coordinates of an object that we want to find the local coordinates of
 * @return                 'coords' converted into the local coordinates of this observer.
 *************************************************************************/
CECoordinates CEObserver::ObservedPosition(CECoordinates& coords)
{
    // Compute the observed coordinates for these coordinates
    CECoordinates observed_coords = coords.GetObservedCoords(*current_date_,*this) ;
    return observed_coords ;
}

/**********************************************************************//**
 * Set the date object for this observer
 * @param[in]  date                CEDate object
 *************************************************************************/
void CEObserver::SetDate(CEDate* date)
{
    // Prepare to update the current date object
    if ((current_date_ != nullptr) && date_is_owned_) {
        CEDate* ptr_shift = current_date_ ;
        current_date_ = nullptr ;
        delete ptr_shift ;
    }
    
    // Now set the new date
    if (date != nullptr) {
        current_date_ = date ;
        date_is_owned_ = false ;
    } else {
        current_date_ = new CEDate() ;
        date_is_owned_ = true ;
    }
}
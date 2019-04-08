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
 - Geographic latitude (North positive)
 - Elevation (meters above sea level)
 - Atmospheric pressure (hPa)
 - Temperature (Kelvin, Celsius, Fahrenheit)
 - Relative humidity (in range 0-1)
 
 The above are used in the SOFA software to more accurately calculate
 the affect of refraction through the atmosphere.
 */

// C++ HEADERS
#include <stdio.h>

// CPPEPHEM HEADERS
#include "CEObserver.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
CEObserver::CEObserver(void)
{
    init_members();
}


/**********************************************************************//**
 * Constructor from a geographic position (latitude, longitude, altitude)
 * Note that altitude defaults to sea-level
 * 
 * @param[in] longitude         Observer longitude (east positive)
 * @param[in] latitude          Observer latitude
 * @param[in] elevation         Observer elevation above sea-level (meters)
 * @param[in] angle_type        Angle type for lon,lat (default is degrees)
 *************************************************************************/
CEObserver::CEObserver(const double& longitude, 
                       const double& latitude,
                       const double& elevation, 
                       const CEAngleType& angle_type)
{
    init_members();

    // Use the internal methods for setting the values
    SetLongitude(longitude, angle_type) ;
    SetLatitude(latitude, angle_type) ;
    SetElevation(elevation) ;
}


/**********************************************************************//**
 * Copy constructor
 * 
 * @param[in] other            Another observer object that will be copied
 *************************************************************************/
CEObserver::CEObserver(const CEObserver& other)
{
    init_members();
    copy_members(other);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
CEObserver::~CEObserver(void)
{
    free_members();
}


/**********************************************************************//**
 * Copy assignment operator
 * 
 * @param[in] other             CEObserver object to be copied
 * @return Reference to this CEObserver object post-copy
 *************************************************************************/
CEObserver& CEObserver::operator=(const CEObserver& other)
{
    if (this != &other) {
        free_members();
        init_members();
        copy_members(other);
    }
    return *this;
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


/**********************************************************************//**
 * Returns a string containing information about this object
 * @return Formatted string containing information about this observer
 *************************************************************************/
std::string CEObserver::print(void) const
{
    // Fill in the returned string with formatted strings
    std::string msg("Observer:\n");
    msg += "   (Lon,Lat) = (" + std::to_string(Longitude_Deg()) + ", "  + 
                                std::to_string(Latitude_Deg()) + ") deg\n";
    msg += "   Elevation = " + std::to_string(Elevation_m()) + " m\n";
    msg += "   Temp      = " + std::to_string(Temperature_C()) + " C\n";
    msg += "   Pressure  = " + std::to_string(Pressure_hPa()) + " hPa\n";
    msg += "   Humidity  = " + std::to_string(RelativeHumidity()) + " %%\n";
    msg += "   Wavelength= " + std::to_string(Wavelength_um()) + " um\n";
    return msg;
}


/**********************************************************************//**
 * Initialize the data members
 *************************************************************************/
void CEObserver::free_members(void)
{

}


/**********************************************************************//**
 * Copy data members from another CEObserver object
 * 
 * @param[in] other         CEObserver object to copy from
 *************************************************************************/
void CEObserver::copy_members(const CEObserver& other)
{
    longitude_           = other.longitude_;
    latitude_            = other.latitude_;
    elevation_m_         = other.elevation_m_;
    pressure_hPa_        = other.pressure_hPa_;
    temperature_celsius_ = other.temperature_celsius_;
    relative_humidity_   = other.relative_humidity_;
}


/**********************************************************************//**
 * Initialize the data members
 *************************************************************************/
void CEObserver::init_members(void)
{
    longitude_           = 0.0;
    latitude_            = 51.4778;
    elevation_m_         = 0.0;
    temperature_celsius_ = CppEphem::SeaLevelTemp_C();
    pressure_hPa_        = CppEphem::EstimatePressure_hPa(temperature_celsius_);
    relative_humidity_   = 0.0;
}

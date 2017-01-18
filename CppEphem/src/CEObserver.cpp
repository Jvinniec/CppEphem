//
//  CEObserver.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/6/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

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

/////////////////////////////////////////////////////////////////
/// Default constructor
CEObserver::CEObserver()
{
    SetLongitude(0.0) ;
    SetLatitude(0.0) ;
    SetElevation() ;
    SetPressure(EstimatePressure_hPa(0.0)) ;
    SetTemperature_C() ;
    SetRelativeHumidity() ;
}

/////////////////////////////////////////////////////////////////
/// Constructor from a geographic position (latitude, longitude, altitude)
/// Note that altitude defaults to sea-level
///     @param longitude            Observer longitude (east positive)
///     @param latitude             Observer latitude
///     @param elevation            Observer elevation above sea-level (meters)
///     @param angle_type           Angle type for longitude and latitude (RADIANS or DEGREES)
///     @param date                 Current date for the observer
CEObserver::CEObserver(double longitude, double latitude,
                       double elevation, CEAngleType angle_type,
                       CEDate date)
{
    // Use the internal methods for setting the default values
    SetLongitude(longitude, angle_type) ;
    SetLatitude(latitude, angle_type) ;
    SetElevation(elevation) ;
    SetPressure(EstimatePressure_hPa(elevation_m_)) ;
    SetTemperature_C() ;
    SetRelativeHumidity() ;
    SetDate(date) ;
}

/////////////////////////////////////////////////////////////////
/// Copy constructor
///     @param other            Another observer object that will be copied
CEObserver::CEObserver(const CEObserver& other) :
    longitude_(other.longitude_),
    latitude_(other.latitude_),
    elevation_m_(other.elevation_m_),
    pressure_hPa_(other.pressure_hPa_),
    temperature_celsius_(other.temperature_celsius_),
    relative_humidity_(other.relative_humidity_)
{}

/////////////////////////////////////////////////////////////////
/// Destructor
CEObserver::~CEObserver()
{}

#pragma mark - Public Methods

/////////////////////////////////////////////////////////////////
/// Get the local sky coordinates for an object as observed
/// by this observer
///     @param object           Object that we want to find the local coordinates of
///     @return                 Local coordinates of the object
CECoordinates CEObserver::ObservedPosition(CEBody& object)
{
    std::shared_ptr<CECoordinates> coords = object.GetCoordinates() ;
    return ObservedPosition(*coords) ;
}

/////////////////////////////////////////////////////////////////
/// Get the local sky coordinates for an object as observed
/// by this observer
///     @param coords           Coordinates of an object that we want to find the local coordinates of
///     @return                 'coords' converted into the local coordinates of this observer.
CECoordinates CEObserver::ObservedPosition(CECoordinates& coords)
{
    // Test that the coords object isnt empty

    if (false) {
        CECoordinates observed_coords ;
        return coords ;
    } else {
        CECoordinates observed_coords = coords.GetObservedCoords(current_date_,*this) ;
        return observed_coords ;
    }
}

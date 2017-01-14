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
 - Geographic longitude
 - Geographic latitude (East positive)
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
CEObserver::CEObserver(double longitude, double latitude,
                       double elevation, CEAngleType angle_type)
{
    // Use the internal methods for setting the default values
    SetLongitude(longitude, angle_type) ;
    SetLatitude(latitude_, angle_type) ;
    SetElevation(elevation) ;
    SetPressure(EstimatePressure_hPa(elevation_m_)) ;
    SetTemperature_C() ;
    SetRelativeHumidity() ;
}

/////////////////////////////////////////////////////////////////
/// Copy constructor
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
//
//  CEObserver.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/6/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include <stdio.h>

#include "CEObserver.h"

//_________________________________________________
CEObserver::CEObserver()
{
    SetLongitude(0.0) ;
    SetLatitude(0.0) ;
    SetElevation() ;
    SetPressure(EstimatePressure_hPa(0.0)) ;
    SetTemperature_C() ;
    SetRelativeHumidity() ;
}

//_________________________________________________
CEObserver::CEObserver(double longitude, double latitude,
                       double elevation, CEAngleType angle_type)
{
    // Use the internal methods for setting the default values
    SetLongitude(longitude, angle_type) ;
    SetLatitude(latitude_, angle_type) ;
    SetElevation(elevation) ;
    SetPressure(EstimatePressure_hPa(elevation_)) ;
    SetTemperature_C() ;
    SetRelativeHumidity() ;
}

//_________________________________________________
CEObserver::CEObserver(const CEObserver& other) :
    longitude_(other.longitude_),
    latitude_(other.latitude_),
    elevation_(other.elevation_),
    pressure_hPa_(other.pressure_hPa_),
    temperature_celsius_(other.temperature_celsius_),
    relative_humidity_(other.relative_humidity_)
{}


//_________________________________________________
CEObserver::~CEObserver()
{}

#pragma mark - Public Methods
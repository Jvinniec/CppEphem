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
CEObserver::CEObserver() :
    geographic_coords_(),
    altitude_(0.0)
{}

//_________________________________________________
CEObserver::CEObserver(double longitude, double latitude,
                       double altitude) :
    geographic_coords_(CECoordinates(longitude, latitude, CECoordinateType::GEOGRAPHIC)),
    altitude_(altitude)
{}

//_________________________________________________
CEObserver::CEObserver(const CEObserver& other) :
    geographic_coords_(other.geographic_coords_),
    altitude_(other.altitude_)
{}


//_________________________________________________
CEObserver::~CEObserver()
{}

#pragma mark - Public Methods
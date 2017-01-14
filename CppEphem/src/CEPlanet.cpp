//
//  CEPlanet.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 1/13/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

/** \class CEPlanet
 The CEPlanet class describes a planetary object.
 */

#include <stdio.h>

#include "CEPlanet.h"

/////////////////////////////////////////////////////
/// Default constructor
CEPlanet::CEPlanet()
{}

/////////////////////////////////////////////////////
/// Primary constructor
///     @param name             Some identifying name for this object
///     @param coordinates      Coordinates of this object
CEPlanet::CEPlanet(const std::string& name,
                   CECoordinates coordinates) :
    CEBody(coordinates),
    name_(name)
{}

/////////////////////////////////////////////////////
/// Destructor
CEPlanet::~CEPlanet()
{}

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
CEPlanet::CEPlanet() :
    CEBody()
{}

/////////////////////////////////////////////////////
/// Primary constructor
///     @param[in] name             Some identifying name for this object
///     @param[in] xcoord           X-coordinate
///     @param[in] ycoord           Y-coordinate
///     @param[in] coord_type       Coordinate system for passed coordinates
///     @param[in] angle_type       Angle type for the coordinates passed
CEPlanet::CEPlanet(const std::string& name, double xcoord, double ycoord,
                   CECoordinateType coord_type,
                   CEAngleType angle_type) :
    CEBody(name, xcoord, ycoord, coord_type, angle_type)
{}

/////////////////////////////////////////////////////
/// Primary constructor
///     @param[in] name             Some identifying name for this object
///     @param[in] coordinates      Coordinates of this object
CEPlanet::CEPlanet(const std::string& name,
                   CECoordinates coordinates) :
    CEBody(coordinates, name)
{}

/////////////////////////////////////////////////////
/// Destructor
CEPlanet::~CEPlanet()
{}

//
//  CEBody.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/8/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

/** \class CEBody
 The CEBody class defines a celestial object. It should be used as the
 parent class for all classes which define a celestial object (planets,
 stars, etc...).
 */

#include <stdio.h>

#include "CEBody.h"

/////////////////////////////////////////////////////////
/// Default constructor

CEBody::CEBody()
{}

/////////////////////////////////////////////////////////
/// Primary constructor
///     @param[in] name         Name for this object
///     @param[in] xcoord       X-coordinate
///     @param[in] ycoord       Y-coordinate
///     @param[in] angle_type   Angle type (either DEGREES or RADIANS)
///     @param[in] coord_type   Coordinate type (see CECoordinateType)
CEBody::CEBody(const std::string& name,
               double xcoord, double ycoord,
               CECoordinateType coord_type,
               CEAngleType angle_type) :
    CECoordinates(xcoord, ycoord, coord_type, angle_type),
    name_(name)
{}

/////////////////////////////////////////////////////////
/// Copy constructor from a single set of coordinates
///     @param[in] coords       coordinates object
CEBody::CEBody(const CEBody &coords, const std::string& name) :
    CECoordinates(coords),
    name_(name)
{}

/////////////////////////////////////////////////////////
/// Copy constructor from a single set of coordinates
///     @param[in] coords       coordinates object
CEBody::CEBody(const CECoordinates &coords, const std::string& name) :
    CECoordinates(coords),
    name_(name)
{}

/////////////////////////////////////////////////////////
/// Destructor
CEBody::~CEBody()
{}

#pragma Mark - Public methods


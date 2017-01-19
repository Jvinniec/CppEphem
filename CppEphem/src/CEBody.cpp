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
///     @param xcoord       X-coordinate
///     @param ycoord       Y-coordinate
///     @param angle_type   Angle type (either DEGREES or RADIANS)
///     @param coord_type   Coordinate type (see CECoordinateType)
CEBody::CEBody(double xcoord, double ycoord,
               CECoordinateType coord_type,
               CEAngleType angle_type) :
    CECoordinates(xcoord, ycoord, coord_type, angle_type)
{}

/////////////////////////////////////////////////////////
/// Copy constructor from a single set of coordinates
///     @param coords       coordinates object
CEBody::CEBody(const CEBody &coords) :
    CECoordinates(coords)
{}

/////////////////////////////////////////////////////////
/// Copy constructor from a single set of coordinates
///     @param coords       coordinates object
CEBody::CEBody(const CECoordinates &coords) :
    CECoordinates(coords)
{}

/////////////////////////////////////////////////////////
/// Destructor
CEBody::~CEBody()
{}

#pragma Mark - Public methods


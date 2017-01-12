//
//  CEBody.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/8/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

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
    coords_(std::shared_ptr<CECoordinates>(new CECoordinates(xcoord,ycoord,coord_type,angle_type)))
{}

/////////////////////////////////////////////////////////
/// Constructor from a single set of coordinates
///     @param coords       coordinates object

CEBody::CEBody(const CECoordinates &coords) :
    coords_(std::shared_ptr<CECoordinates>(new CECoordinates(coords)))
{}

/////////////////////////////////////////////////////////
/// Copy constructor

CEBody::CEBody(const CEBody& other)
{}

/////////////////////////////////////////////////////////
/// Destructor

CEBody::~CEBody()
{}

#pragma Mark - Public methods

/////////////////////////////////////////////////////////
/// Sets the coordinates of this object
///     @param new_coords           Coordinates object

void CEBody::SetCoordinates(CECoordinates new_coords)
{
    coords_ = std::shared_ptr<CECoordinates>(new CECoordinates(new_coords)) ;
}

/////////////////////////////////////////////////////////
/// Sets the coordinates of this object
///     @param xcoord               X-coordinate
///     @param ycoord               Y-coordinate
///     @param coord_type           Coordinate type (@see CECoordinateType)
///     @param angle_type           Angle type (either DEGREES or RADIANS)

void CEBody::SetCoordinates(double xcoord, double ycoord,
                            CECoordinateType coord_type,
                            CEAngleType angle_type)
{
    coords_ = std::shared_ptr<CECoordinates>(new CECoordinates(xcoord,ycoord,coord_type,angle_type)) ;
}


//
//  CECoordinates.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 3/26/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include "CECoordinates.h"

//__________________________________________________________
// Default constructor
CECoordinates::CECoordinates() :
    xcoord_(0.0),
    ycoord_(0.0),
    coord_type_(CECoordinateType::J2000)
{}

//__________________________________________________________
// Primary constructor
CECoordinates::CECoordinates(double xcoord, double ycoord,
                             CECoordinateType coord_type,
                             CEAngleType angle_type) :
    xcoord_(xcoord),
    ycoord_(ycoord),
    coord_type_(coord_type)
{
    // If we've been given the coordinates in degrees, convert to radians
    if (angle_type == CEAngleType::DEGREES) {
        xcoord_ *= DD2R ;
        ycoord_ *= DD2R ;
    }
}


//__________________________________________________________
// Copy constructor
CECoordinates::CECoordinates(const CECoordinates& other) :
    xcoord_(other.xcoord_),
    ycoord_(other.ycoord_),
    coord_type_(other.coord_type_)
{}
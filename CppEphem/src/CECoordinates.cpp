//
//  CECoordinates.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 3/26/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include "CECoordinates.h"

//__________________________________________________________
// Primary constructor
CECoordinates::CECoordinates(double xcoord, double ycoord, const std::string& coord_system) :
    xcoord_(xcoord),
    ycoord_(ycoord),
    coord_system_(coord_system)
{}


//__________________________________________________________
// Copy constructor
CECoordinates::CECoordinates(const CECoordinates& other) :
    xcoord_(other.xcoord_),
    ycoord_(other.ycoord_),
    coord_system_(other.coord_system_)
{}
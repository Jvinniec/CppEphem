/***************************************************************************
 *  CEBody.cpp: CppEphem                                                   *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016 JCardenzana                                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/

/** \class CEBody
 The CEBody class defines a celestial object. It should be used as the
 parent class for all classes which define a celestial object (planets,
 stars, etc...).
 */

#include <stdio.h>

#include "CEBody.h"

/**********************************************************************//**
 * Default constructor
 *************************************************************************/
CEBody::CEBody()
{}

/**********************************************************************//**
 * Primary constructor
 * 
 * @param[in] name         Name for this object
 * @param[in] xcoord       X-coordinate
 * @param[in] ycoord       Y-coordinate
 * @param[in] angle_type   Angle type (either DEGREES or RADIANS)
 * @param[in] coord_type   Coordinate type (see CECoordinateType)
 *************************************************************************/
CEBody::CEBody(const std::string& name,
               const double& xcoord, const double& ycoord,
               const CECoordinateType& coord_type,
               const CEAngleType& angle_type) :
    CECoordinates(xcoord, ycoord, coord_type, angle_type),
    name_(name)
{}

/**********************************************************************//**
 * Copy constructor from a single set of coordinates
 * @param[in] coords       coordinates object
 *************************************************************************/
CEBody::CEBody(const CEBody &coords, const std::string& name) :
    CECoordinates(coords),
    name_(name)
{}

/**********************************************************************//**
 * Copy constructor from a single set of coordinates
 * @param[in] coords       coordinates object
 *************************************************************************/
CEBody::CEBody(const CECoordinates &coords, const std::string& name) :
    CECoordinates(coords),
    name_(name)
{}

/**********************************************************************//**
 * Destructor
 *************************************************************************/
CEBody::~CEBody()
{}

#pragma Mark - Public methods

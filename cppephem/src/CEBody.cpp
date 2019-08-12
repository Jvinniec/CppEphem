/***************************************************************************
 *  CEBody.cpp: CppEphem                                                   *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016-2019 JCardenzana                                      *
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
CEBody::CEBody() : CESkyCoord()
{
    init_members();
}

/**********************************************************************//**
 * Primary constructor
 * 
 * @param[in] name         Name for this object
 * @param[in] xcoord       X-coordinate
 * @param[in] ycoord       Y-coordinate
 * @param[in] angle_type   Angle type (either DEGREES or RADIANS)
 * @param[in] coord_type   Coordinate type (see CESkyCoordType)
 *************************************************************************/
CEBody::CEBody(const std::string&      name,
               const CEAngle&          xcoord, 
               const CEAngle&          ycoord,
               const CESkyCoordType&   coord_type) :
    CESkyCoord(xcoord, ycoord, coord_type)
{
    init_members();
    SetName(name);
}

/**********************************************************************//**
 * Copy constructor
 * @param[in] other         CEBody object to copy from
 * @param[in] name          New object name
 *************************************************************************/
CEBody::CEBody(const CEBody&      other, 
               const std::string& name) :
    CESkyCoord(other)
{
    init_members();
    copy_members(other);

    // Set the name if it is not empty
    if (name.size() > 0) SetName(name);
}

/**********************************************************************//**
 * Copy constructor from a single set of coordinates
 * @param[in] coords       coordinates object
 *************************************************************************/
CEBody::CEBody(const CESkyCoord&  coords, 
               const std::string& name) :
    CESkyCoord(coords)
{
    init_members();
    
    // Set the name if it is not empty
    if (name.size() > 0) SetName(name);
}

/**********************************************************************//**
 * Destructor
 *************************************************************************/
CEBody::~CEBody()
{
    free_members();
}


/*--------------------------------------------------*
 *                   Overloaded operators
 *--------------------------------------------------*/


/**********************************************************************//**
 * Overloaded assignment operator
 * @param[in] other     CEBody object to be copied
 * @return Reference to this object post-copy
 *************************************************************************/
CEBody& CEBody::operator=(const CEBody& other)
{
    if (this != &other) {
        // Copy parent class parameters
        this->CESkyCoord::operator=(other);

        // Reset this object and copy values from 'other'
        free_members();
        init_members();
        copy_members(other);
    }
    return *this;
}

/*--------------------------------------------------*
 *                   Public methods
 *--------------------------------------------------*/


/**********************************************************************//**
 * Computes the observed coordinates for this object based 
 *************************************************************************/
CESkyCoord CEBody::ObservedCoords(const CEDate&     date,
                                  const CEObserver& observer) const
{
    CESkyCoord coords_icrs = GetCoordinates(date);
    return coords_icrs.ConvertToObserved(date, observer);
}                                         


/*--------------------------------------------------*
 *                  Private methods
 *--------------------------------------------------*/


/**********************************************************************//**
 * Free all allocated data members
 *************************************************************************/
void CEBody::free_members(void)
{
}


/**********************************************************************//**
 * Initialize data members
 *************************************************************************/
void CEBody::init_members(void)
{
    name_ = "undefined";
}


/**********************************************************************//**
 * Copy all data members
 *************************************************************************/
void CEBody::copy_members(const CEBody& other)
{
    name_ = other.name_;
}

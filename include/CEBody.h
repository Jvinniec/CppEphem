/***************************************************************************
 *  CEBody.h: CppEphem                                                     *
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

// ============================================================
// CEBody :
//    This class is meant to serve as the base class for all
//    astronomical object types (stars, planets, etc...). It
//    implements the base coordinate storage objects and
//    implements the methods necessary for coordinate conversions.
// ============================================================

#ifndef CEBody_h
#define CEBody_h

// STANDARD C++ HEADERS
#include <vector>

// CPPEPHEM HEADERS
#include "CECoordinates.h"

class CEBody : public CECoordinates {
public:
    CEBody() ;
    CEBody(const std::string& name, 
           const double& xcoord, const double& ycoord,
           const CECoordinateType& coord_type = CECoordinateType::ICRS,
           const CEAngleType& angle_type = CEAngleType::RADIANS) ;
    CEBody(const CEBody& other, const std::string& name="") ;
    CEBody(const CECoordinates& coords, const std::string& name="") ;
    virtual ~CEBody() ;
    
    std::string Name() const;
    void        SetName(const std::string& new_name);
    
    // -------------------------------------------------------
    // Methods for getting the coordinates of this object
    // -------------------------------------------------------
    
    // Returns the coordinates associated with this object as a 'CECoordinates' object
    CECoordinates GetCoordinates(void) const;
    
protected:
    
    // Proper motion variables for this object. These are used for
    // correctly getting the objects coordinates at some date other
    // than the date indicated in 'coords_'
    std::string name_ = "NONE" ;        ///< Name of this object
    
private:
    
};


/**********************************************************************//**
 * Get the name of this object
 * 
 * @return the name of this object
 *************************************************************************/
inline
std::string CEBody::Name() const
{
    return name_;
} 

/**********************************************************************//**
 * Set the name of this object
 * 
 * @param[in] new_name     New name of this object
 *************************************************************************/
inline
void CEBody::SetName(const std::string& new_name) 
{
    name_ = new_name;
}

/**********************************************************************//**
 * Return the coordinates associated with this object as a 'CECoordinates' object 
 *************************************************************************/
inline
CECoordinates CEBody::GetCoordinates(void) const
{
    return CECoordinates(*this) ;
}

#endif /* CEBody_h */

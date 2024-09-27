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
#include "CESkyCoord.h"

class CEBody : public CESkyCoord {
public:
    /************************************
     * Basic constructors & destructors
     ***********************************/

    CEBody() ;
    CEBody(const std::string&      name, 
           const CEAngle&          xcoord, 
           const CEAngle&          ycoord,
           const CESkyCoordType&   coord_type=CESkyCoordType::ICRS);
    CEBody(const CEBody&      other, 
           const std::string& name="");
    CEBody(const CESkyCoord&  coords, 
           const std::string& name="");
    virtual ~CEBody();
    
    /************************************
     * Overloaded operators
     ***********************************/

    CEBody& operator=(const CEBody& other);

    /************************************
     * Public methods
     ***********************************/

    virtual CESkyCoord GetCoordinates(const CEDate& date=CEDate::CurrentJD()) const;
    virtual CESkyCoord ObservedCoords(const CEDate&     date,
                                      const CEObserver& observer) const;
    std::string   Name(void) const;
    void          SetName(const std::string& new_name);
    
    // Necessary methods
    const std::string ClassName(void) const;
    virtual const std::string describe(void) const;
    
private:
    
    /************************************
     * Private methods
     ***********************************/
    void copy_members(const CEBody& other);
    void free_members(void);
    void init_members(void);

    // Proper motion variables for this object. These are used for
    // correctly getting the objects coordinates at some date other
    // than the date indicated in 'coords_'
    std::string name_;        ///< Name of this object
    
};


/**********************************************************************//**
 * Return name of this class
 *************************************************************************/
inline
const std::string CEBody::ClassName() const
{
    return std::string("CEBody");
}


/**********************************************************************//**
 * Get the name of this object
 * 
 * @return the name of this object
 *************************************************************************/
inline
std::string CEBody::Name(void) const
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
 * Return the ICRS coordinates associated with this object
 * 
 * @param[in] date          Date for the coordinates
 * @return Coordinates of this object in the ICRS frame 
 * 
 * Note that the date information is useful for those objects that may
 * be either planets or stars with proper motion.
 *************************************************************************/
inline
CESkyCoord CEBody::GetCoordinates(const CEDate& date) const
{
    return CESkyCoord(*this) ;
}

#endif /* CEBody_h */

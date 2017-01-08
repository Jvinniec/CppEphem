//
//  CEBody.h
//  CppEphem
//
//  Created by Josh Cardenzana on 4/8/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//
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

class CEBody {
public:
    /** Default constructor */
    CEBody() {} ;
    /** Constructor from two coordinates */
    CEBody(double xcoord, double ycoord,
           CEAngleType angle_type = CEAngleType::RADIANS,
           CECoordinateType coord_type = CECoordinateType::CIRS) {};
    /** Constructor from a CECoordinate object */
    CEBody(const CECoordinates &coords) {} ;
    /** Copy Constructor */
    CEBody(const CEBody& other) {} ;
    /** Destructor */
    virtual ~CEBody() {} ;
    
    // -------------------------------------------------------
    // Methods for getting the coordinates of this object
    // -------------------------------------------------------
    /** Returns the coordinates associated with this object as a 'CECoordinates' object */
    std::shared_ptr<CECoordinates> GetCoordinates() {return coords_ ;}
    
    // -------------------------------------------------------
    // Methods for setting the coordinates of this object
    // -------------------------------------------------------
    /** Method for setting the coordinates associated with this object. */
    void SetCoordinates(CECoordinates new_coords,
                        CECoordinateType coord_type=CECoordinateType::CIRS) ;
    
protected:
    std::shared_ptr<CECoordinates> coords_ ; ///< Coordinates of this object
    
    // Proper motion variables for this object. These are used for
    // correctly getting the objects coordinates at some date other
    // than the date indicated in 'coords_'
    
private:
    
};

#endif /* CEBody_h */

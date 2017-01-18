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
    CEBody() ;
    CEBody(double xcoord, double ycoord,
           CECoordinateType coord_type = CECoordinateType::CIRS,
           CEAngleType angle_type = CEAngleType::RADIANS) ;
    CEBody(const CECoordinates &coords) ;
    CEBody(const CEBody& other) ;
    virtual ~CEBody() ;
    
    // -------------------------------------------------------
    // Methods for getting the coordinates of this object
    // -------------------------------------------------------
    /** Returns the coordinates associated with this object as a 'CECoordinates' object */
    std::shared_ptr<CECoordinates> GetCoordinates() {return coords_ ;}
    
    /// Get the x-coordinates of this object in a given coordinate type
    double GetXCoord_Deg() {return coords_->XCoordinate_Deg() ;}
    /// Get the y-coordinate of this object
    double GetYCoord_Deg() {return coords_->YCoordinate_Deg() ;}
    
    // -------------------------------------------------------
    // Methods for setting the coordinates of this object
    // -------------------------------------------------------
    void SetCoordinates(CECoordinates new_coords) ;
    void SetCoordinates(double xcoord, double ycoord,
                        CECoordinateType coord_type = CECoordinateType::CIRS,
                        CEAngleType angle_type = CEAngleType::RADIANS) ;
    
protected:
    std::shared_ptr<CECoordinates> coords_=std::shared_ptr<CECoordinates>(
                                    new CECoordinates(0.0,0.0,CECoordinateType::CIRS)) ; ///< Coordinates of this object
    
    // Proper motion variables for this object. These are used for
    // correctly getting the objects coordinates at some date other
    // than the date indicated in 'coords_'
    
private:
    
};

#endif /* CEBody_h */

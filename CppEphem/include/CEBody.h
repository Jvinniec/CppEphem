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

class CEBody : public CECoordinates {
public:
    CEBody() ;
    CEBody(const std::string& name, double xcoord, double ycoord,
           CECoordinateType coord_type = CECoordinateType::CIRS,
           CEAngleType angle_type = CEAngleType::RADIANS) ;
    CEBody(const CEBody& other, const std::string& name="") ;
    CEBody(const CECoordinates& coords, const std::string& name="") ;
    virtual ~CEBody() ;
    
    /// Get the name of this object
    ///     @return the name of this object
    std::string Name() {return name_;}
    /// Set the name of this object
    ///     @param[in] new_name     New name of this object
    void SetName(const std::string& new_name) {name_ = new_name;}
    
    // -------------------------------------------------------
    // Methods for getting the coordinates of this object
    // -------------------------------------------------------
    /** Returns the coordinates associated with this object as a 'CECoordinates' object */
    CECoordinates GetCoordinates() {return CECoordinates(*this) ;}
    
protected:
    
    // Proper motion variables for this object. These are used for
    // correctly getting the objects coordinates at some date other
    // than the date indicated in 'coords_'
    std::string name_ = "NONE" ;        ///< Name of this object
    
private:
    
};

#endif /* CEBody_h */

//
//  CECoordinates.h
//  CppEphem
//
//  Created by Josh Cardenzana on 3/26/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#ifndef CECoordinates_h
#define CECoordinates_h

#include <string>

// CppEphem HEADERS
#include "CENamespace.h"

// SOFA HEADER
#include "sofa.h"

using namespace CppEphem ;

// The following enum specifies what the
enum CECoordinateType {J2000,           // RA, Dec
                       GALACTIC,        // Galacitc longitude, latitude
                       LOCALSKY,           // Altitude, azimuth
                       GEOGRAPHIC} ;    // Geographic longitude, latitude

// Initiate the class that holds the coordinate information
class CECoordinates {
public:
    // Default constructor
    CECoordinates() ;
    // Primary constructor (NOTE: xcoord & ycoord are expected to be in radians by default
    CECoordinates(double xcoord, double ycoord,
                  CECoordinateType coord_type,
                  CEAngleType angle_type=CEAngleType::RADIANS) ;
    // Copy onstructor
    CECoordinates(const CECoordinates& other) ;
    // Destructor
    virtual ~CECoordinates() {} ;
    
    // Methods for accessing the coordinate information
    double XCoordinate_Deg() {return xcoord_ * DR2D ;}
    double YCoordinate_Deg() {return ycoord_ * DR2D ;}
    double XCoordinate_Rad() {return xcoord_ ;}
    double YCoordinate_Rad() {return ycoord_ ;}
    CECoordinateType GetCoordSystem() {return coord_type_;}
    
    
protected:
    // Coordinate variables
    double xcoord_ ;                // X coordinate (radians)
    double ycoord_ ;                // Y coordinate (radians)
    CECoordinateType coord_type_ ;  // Coordinate system to which 'xcoord_' and 'ycoord_' belong
                                    // Possible values are "J2000", "GALACTIC", "LOCAL", and "GEOGRAPHIC"
    
    // If we're specifying the coordinates in the form of a
    
private:
    
};

#endif /* CECoordinates_h */

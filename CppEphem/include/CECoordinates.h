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

# define RadToDeg 

// The following enum specifies what the
enum CECoordinateType {J2000,           // RA, Dec
                       GALACTIC,        // Galacitc longitude, latitude
                       LOCAL,           // Altitude, azimuth
                       GEOGRAPHIC} ;    // Geographic longitude, latitude

// Initiate the class that holds the coordinate information
class CECoordinates {
public:
    // Default constructor
    CECoordinates() {} ;
    // Primary constructor
    CECoordinates(double xcoord, double ycoord, CECoordinateType coord_type) ;
    // Copy onstructor
    CECoordinates(const CECoordinates& other) ;
    // Destructor
    virtual ~CECoordinates() {} ;
    
    // Methods for accessing the coordinate information
    double GetXCoordinate_Deg() {return xcoord_;}
    double GetYCoordinate_Deg() {return ycoord_;}
    CECoordinateType GetCoordSystem() {return coord_type_;}
    
    
    
protected:
    // Coordinate variables
    double xcoord_ ;                // X coordinate
    double ycoord_ ;                // Y coordinate
    CECoordinateType coord_type_ ;  // Coordinate system to which 'xcoord_' and "ycoord_' belong
                                    // Possible values are "J2000", "GALACTIC", "LOCAL", and "GEOGRAPHIC"
    
    // If we're specifying the coordinates in the form of a
    
private:
    
};

#endif /* CECoordinates_h */

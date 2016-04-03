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

class CECoordinates {
public:
    // Default constructor
    CECoordinates() {} ;
    // Primary constructor
    CECoordinates(double xcoord, double ycoord, const std::string& coord_system) ;
    // Copy onstructor
    CECoordinates(const CECoordinates& other) ;
    // Destructor
    virtual ~CECoordinates() {} ;
    
    // Methods for accessing the coordinate information
    double GetXCoordinate_Deg() {return xcoord_;}
    double GetYCoordinate_Deg() {return ycoord_;}
    std::string GetCoordSystem() {return coord_system_;}
    
protected:
    // Coordinate variables
    double xcoord_ ;            // X coordinate
    double ycoord_ ;            // Y coordinate
    std::string coord_system_ ; // Coordinate system to which 'xcoord_' and "ycoord_' belong
                                // Possible values are "J2000", "GALACTIC", "LOCAL"
    
private:
    
};

#endif /* CECoordinates_h */

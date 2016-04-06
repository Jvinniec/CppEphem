//
//  CEObserver.h
//  CppEphem
//
//  Created by Josh Cardenzana on 4/6/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#ifndef CEObserver_h
#define CEObserver_h

#include "CECoordinates.h"
#include "CEDate.h"

class CEObserver {
public:
    // Default constructor
    CEObserver() ;
    // Constructor from a geographic position (latitude, longitude, altitude)
    // Note that altitude defaults to sea-level
    CEObserver(double longitude, double latitude, double altitude=0.0) ;
    
    // Copy constructor
    CEObserver(const CEObserver& other) ;
    // Destructor
    virtual ~CEObserver() ;
    
    /****************************************************
     * Methods for accessing the underlying observer info
     ****************************************************/
    CECoordinates GeographicCoordinates() {return geographic_coords_ ;}
    
    // Get the Longitude/Latitude
    double Longitude_Rad() {return geographic_coords_.XCoordinate_Rad() ;}
    double Longitude_Deg() {return geographic_coords_.XCoordinate_Deg() ;}
    double Latitude_Rad() {return geographic_coords_.YCoordinate_Rad() ;}
    double Latitude_Deg() {return geographic_coords_.YCoordinate_Deg() ;}

    // Get the altitude (in meters) above sea-level
    double Altitude() {return altitude_ ;}
    
    // Get the current time vector
    CETime CurrentTime() {return current_time_ ;}
    std::vector<double> CurrentTimeVect() {return current_time_ ;}
    
    /****************************************************
     * Methods for setting the underlying observer info
     ****************************************************/
    void SetGeographicCoordinates(CECoordinates new_geo_coords_) ;
    void SetLongitude(
    
protected:
    // Variables which define the observer
    CECoordinates geographic_coords_ ;  // Geographic position on the Earth
    double altitude_ ;                  // Altitude (in meters) above sea-level
    CETime current_time_ ;              // Current UTC time at the position in question
    
    
private:
    
};
#endif /* CEObserver_h */

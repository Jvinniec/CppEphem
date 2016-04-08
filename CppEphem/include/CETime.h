//
//  CETime.h
//  CppEphem
//
//  Created by Josh Cardenzana on 4/6/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#ifndef CETime_h
#define CETime_h

#include <stdio.h>
#include <vector>

// Time types are defined as:
//      UTC   - Coordinate Universal Time
//      GAST  - Greenwich Apparent Sidereal Time
//      LAST  - Local Apparent Sidereal Time
//      LOCALTIME - Local time (defined as the UTC + timezone_shift_)
enum CETimeType {UTC, GAST, LAST, LOCALTIME} ;

class CETime {
public:
    // Default constructor
    CETime() ;
    // Primary constructor
    CETime(double time, CETimeType time_format=CETimeType::UTC) ;
    CETime(std::vector<double> time, CETimeType time_format=CETimeType::UTC) ;
    // Copy constructor
    CETime(const CETime& other) ;
    // Destructor
    virtual ~CETime() ;
    
    void SetTime(double time, CETimeType time_format=CETimeType::UTC) ;
    
    /*******************************************
     * Convert between the various time types
     *******************************************/
    
    
protected:
    // Variables for storing the time in various formats
    // The vectors store the time in the following format:
    // element 0 - hours
    // element 1 - minutes
    // element 2 - seconds
    // element 3 - fractional seconds
    // Note that the times are actually not connected with each other
    std::vector<double> time_ ;
    
private:
    
};

#endif /* CETime_h */

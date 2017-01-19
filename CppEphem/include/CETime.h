//
//  CETime.h
//  CppEphem
//
//  Created by Josh Cardenzana on 4/6/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#ifndef CETime_h
#define CETime_h

#include <cmath>
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
    
    /*******************************************
     * Get the time
     *******************************************/
    
    static double CurrentUTC() ;
    static std::vector<double> CurrentUTC_vect() ;
    static double UTC(double jd) ;
    static std::vector<double> UTC_vect(double jd) ;
    
    // Convert a double of the form HHMMSS.S to a vector with
    // the same format as 'time_'
    static std::vector<double> TimeDbl2Vect(double time) ;
    static double TimeVect2Dbl(std::vector<double> time) ;
    
    // Convert number of seconds since midnight to HHMMSS.S formatted double
    static double TimeSec2Time(double seconds) ;
    static std::vector<double> TimeSec2Vect(double seconds) ;
    
    /*******************************************
     * Convert between the various time types
     *******************************************/
    
    void SetTime(double time, CETimeType time_format=CETimeType::UTC) ;
    void SetTime(std::vector<double> time_vect,
                 CETimeType time_format=CETimeType::UTC) ;
    void SetHours(double hours)
        {time_[0] = hours ;}
    void SetMinutes(double minutes)
        {time_[1] = minutes ;}
    void SetSeconds(double seconds)
        {time_[2] = std::floor(seconds) ;
         time_[3] = seconds-time_[2] ;}
    
    /*******************************************
     * Convert between the various time types
     *******************************************/
    
    // UTC conversions
    static void UTC2GAST() ;
    static void UTC2LAST() ;
    static void UTC2LOCALTIME() ;
    
    // GAST conversions
    
protected:
    // Variables for storing the time in various formats
    // The vectors store the time in the following format:
    // element 0 - hours
    // element 1 - minutes
    // element 2 - seconds
    // element 3 - fractional seconds
    // Note that the internal stored time is UTC
    std::vector<double> time_ ;
    CETimeType time_type_ ;
    
    // Internal methods for setting the time
    void SetTime_UTC(std::vector<double> time) ;
    void SetTime_GAST(std::vector<double> time) ;
    void SetTime_LST(std::vector<double> time) ;
    void SetTime_LOCALTIME(std::vector<double> time) ;
    
private:
    
};

#endif /* CETime_h */

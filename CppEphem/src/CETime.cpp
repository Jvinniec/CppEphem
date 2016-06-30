//
//  CETime.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/6/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include <stdio.h>
#include "CETime.h"

//_______________________________________
CETime::CETime() :
    time_(std::vector<double>(4,0.0)),
    time_type_(CETimeType::UTC)
{}


//_______________________________________
// Used for setting time from a HHMMSS.S format
CETime::CETime(double time, CETimeType time_format) :
    CETime(TimeDbl2Vect(time), time_format)
{}


//_______________________________________
// Used for setting the time from a vector where:
//    time[0] = hours
//    time[1] = minutes
//    time[2] = seconds
//    time[3] = fractions of a second
CETime::CETime(std::vector<double> time, CETimeType time_format) :
    time_(std::vector<double>(4,0.0)),
    time_type_(time_format)
{
    // Just in case the passed "time" variable isnt the same length
    for (int i=0; i<time.size(); i++) {
        time_[i] = time[i] ;
        // In case "time" has more than 4 elements,
        // quit when we've stored the first 4
        if (i==3) break ;
    }
}


//_______________________________________
CETime::CETime(const CETime& other) :
    time_(other.time_),
    time_type_(other.time_type_)
{}


//_______________________________________
CETime::~CETime()
{}

# pragma mark - Public Methods

//_______________________________________
void CETime::SetTime(double time, CETimeType time_format)
{
    // Convert the double into a vector
    std::vector<double> time_vect = TimeDbl2Vect(time) ;
    
    // Call the vector version of SetTime
    SetTime(time_vect, time_format) ;
}


//_______________________________________
void CETime::SetTime(std::vector<double> time_vect, CETimeType time_format)
{
    // Convert the double into a vector
    if (time_format==CETimeType::UTC) {
        SetTime_UTC(time_vect) ;
    } else if (time_format==CETimeType::GAST) {
        SetTime_GAST(time_vect) ;
    } else if (time_format==CETimeType::LAST) {
        void SetTime_LST(std::vector<double> time) ;
    } else if (time_format==CETimeType::LOCALTIME) {
        void SetTime_LOCALTIME(std::vector<double> time) ;
    }
}
# pragma mark - Protected Methods

//_______________________________________________________________
std::vector<double> CETime::TimeDbl2Vect(double time)
{
    // Create a vector to hold the information
    std::vector<double> time_vect(4,0.0) ;
    
    // Get the seconds fraction
    time_vect[3] = time - std::floor(time) ;
    // Get the seconds value
    time_vect[2] = int(std::floor(time)) % 100 ;
    // Get the minutes
    time_vect[1] = int(std::floor(time - time_vect[2]))/100 % 100 ;
    // Get the hours
    time_vect[0] = (std::floor(time - time_vect[2] - time_vect[1])/10000) ;
    
    return time_vect ;
}


//_______________________________________________________________
void CETime::SetTime_UTC(std::vector<double> time)
{}


//_______________________________________________________________
void CETime::SetTime_GAST(std::vector<double> time)
{}


//_______________________________________________________________
void CETime::SetTime_LST(std::vector<double> time)
{}


//_______________________________________________________________
void CETime::SetTime_LOCALTIME(std::vector<double> time)
{}

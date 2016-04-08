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
    time_(std::vector<double>(4,0.0))
{}


//_______________________________________
CETime::CETime(double time, CETimeType time_format) :
    time_(std::vector<double>(4,0.0))
{}


//_______________________________________
CETime::CETime(std::vector<double> time, CETimeType time_format) :
    time_(std::vector<double>(4,0.0))
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
    time_(other.time_)
{}


//_______________________________________
CETime::~CETime()
{}

# pragma mark - Public Methods

//_______________________________________
void CETime::SetTime(double time, CETimeType time_format)
{
}
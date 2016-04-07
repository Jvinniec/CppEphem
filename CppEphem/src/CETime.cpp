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
    utc_(std::vector<double>(4,0.0))
{}


//_______________________________________
CETime::CETime(double time, CETimeType time_format) :
    utc_(std::vector<double>(4,0.0))
{}


//_______________________________________
CETime::CETime(std::vector<double> time, CETimeType time_format) :
    utc_(std::vector<double>(4,0.0))
{}


//_______________________________________
CETime::CETime(const CETime& other) :
    utc_(other.utc_)
{}


//_______________________________________
CETime::~CETime()
{}

# pragma mark - Public Methods

//_______________________________________
void CETime::SetTime(double time, CETimeType time_format)
{
}
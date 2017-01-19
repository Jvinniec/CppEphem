//
//  CERunningDate.h
//  CppEphem
//
//  Created by Josh Cardenzana on 1/19/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

#ifndef CERunningDate_h
#define CERunningDate_h

#include <chrono>

#include "CEDate.h"


class CERunningDate : public CEDate {
public:
    CERunningDate() ;
    virtual ~CERunningDate() ;
    
    // Some overloaded methods to make sure that the current
    // values are obtained first
    virtual double JD() ;
    virtual double MJD() ;
    virtual double Gregorian() ;
    
    /// Method for getting the number of seconds since this object was created
    inline double RunTime()
        {return std::chrono::duration_cast<std::chrono::duration<double>>(start.time_since_epoch()).count();}
    
protected:
    /// Variable to hold the starting time. Additional updates will be computed from
    /// this start time.
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now() ;
};

#endif /* CERunningDate_h */

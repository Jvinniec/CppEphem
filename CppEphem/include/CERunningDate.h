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
    
    // Method that can be used to change the date that is stored in this object
    virtual void SetDate(double date, CEDateType time_format=CEDateType::JD) ;
    // Method for setting the dates from the Gregorian calendar dates
    virtual void SetDate(std::vector<double> date) ;
    
    /// Method for getting the number of seconds since this object was created or reset
    virtual double RunTime() ;

    /// Returns the ellapsed time scaled by some factor
    inline double ScaledRunTime()
        {return timer_speed_factor_ * RunTime() ;}
    /// Method for resetting the 'start' variable
    inline void ResetTime()
        {start = std::chrono::high_resolution_clock::now();}
    
    /// Get the rate at which the timer is progressing
    ///     @return Time scale factor
    virtual double GetTimerSpeed() {return timer_speed_factor_ ;}
    /// Set the rate at which the timer progresses
    ///     @param scale    Time scale factor
    virtual void SetTimerSpeed(double scale=1.0) {timer_speed_factor_ = scale;}
    
protected:
    /// Variable to hold the starting time. Additional updates will be computed from
    /// this start time.
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now() ;
    
    /// Variable that can be used to speed up, slow down, or reverse the rate of time
    double timer_speed_factor_ = 1.0 ;
};

#endif /* CERunningDate_h */

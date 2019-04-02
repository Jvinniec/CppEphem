/***************************************************************************
 *  CERunningDate.h: CppEphem                                              *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2017 JCardenzana                                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/

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
    virtual double JD() const;
    virtual double MJD() const;
    virtual double Gregorian() const;
    
    // Method that can be used to change the date that is stored in this object
    virtual void SetDate(double date=CEDate::CurrentJD(), CEDateType time_format=CEDateType::JD) ;
    // Method for setting the dates from the Gregorian calendar dates
    virtual void SetDate(std::vector<double> date) ;
    
    /// Method for getting the number of seconds since this object was created or reset
    virtual double RunTime() const;

    /// Returns the ellapsed time scaled by some factor
    inline double ScaledRunTime() const
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

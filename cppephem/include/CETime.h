/***************************************************************************
 *  CETime.h: CppEphem                                                     *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016 JCardenzana                                           *
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

#ifndef CETime_h
#define CETime_h

#include <cmath>
#include <stdio.h>
#include <vector>

#include "CENamespace.h"

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
    CETime(const double& time, 
           CETimeType time_format=CETimeType::UTC) ;
    CETime(std::vector<double> time, 
           CETimeType time_format=CETimeType::UTC) ;
    // Copy constructor
    CETime(const CETime& other) ;
    // Destructor
    virtual ~CETime() ;
    
    CETime& operator=(const CETime& other);

    /*******************************************
     * Get the time
     *******************************************/
    
    static double CurrentUTC() ;
    static std::vector<double> CurrentUTC_vect() ;
    static double UTC(const double& jd) ;
    static std::vector<double> UTC_vect(const double& jd) ;
    
    // Convert a double of the form HHMMSS.S to a vector with
    // the same format as 'time_'
    static std::vector<double> TimeDbl2Vect(const double& time) ;
    static double TimeVect2Dbl(std::vector<double> time) ;
    
    // Convert number of seconds since midnight to HHMMSS.S formatted double
    static double TimeSec2Time(const double& seconds) ;
    static std::vector<double> TimeSec2Vect(const double& seconds) ;
    
    static double SystemUTCOffset_hrs()
    {
        time_t now ;
        time (&now) ;
        struct tm local;
        localtime_r(&now, &local) ;
        return local.tm_gmtoff/DAYSEC * 24.0 ;
    }
    
    /*******************************************
     * Convert between the various time types
     *******************************************/
    
    void SetTime(const double& time, 
                 CETimeType    time_format=CETimeType::UTC) ;
    void SetTime(std::vector<double> time_vect,
                 CETimeType time_format=CETimeType::UTC) ;
    void SetHours(const double& hours)
        {time_[0] = hours ;}
    void SetMinutes(const double& minutes)
        {time_[1] = minutes ;}
    void SetSeconds(const double& seconds)
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
    
private:

    void copy_members(const CETime& other);
    void init_members(void);
    void free_members(void);

    // Internal methods for setting the time
    void SetTime_UTC(std::vector<double> time);
    void SetTime_GAST(std::vector<double> time);
    void SetTime_LST(std::vector<double> time);
    void SetTime_LOCALTIME(std::vector<double> time);  

    // Variables for storing the time in various formats
    // The vectors store the time in the following format:
    // element 0 - hours
    // element 1 - minutes
    // element 2 - seconds
    // element 3 - fractional seconds
    // Note that the internal stored time is UTC
    std::vector<double> time_ ;
    CETimeType time_type_ ;
};

#endif /* CETime_h */

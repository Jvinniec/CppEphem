/***************************************************************************
 *  CETime.cpp: CppEphem                                                   *
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

/** \class CETime
 This class stores and computes information regarding the time and
 handles conversions to current time zone. This method is mostly
 intended to serve as a helper class to CEDate.
 */

// C++ HEADERS
#include <stdio.h>
#include <chrono>
#include <ctime>

// CPPEPHEM HEADERS
#include "CETime.h"

/**********************************************************************//**
 * Default constructor
 *************************************************************************/
CETime::CETime() :
    time_(std::vector<double>(4,0.0)),
    time_type_(CETimeType::UTC)
{}

/**********************************************************************//**
 * Primary constructor
 * 
 * @param time             Time from a HHMMSS.S formatted double
 * @param time_format      Specifies what type is represented by 'time' (see ::CETimeType)
 *************************************************************************/
CETime::CETime(double time, CETimeType time_format) :
    CETime(TimeDbl2Vect(time), time_format)
{}


/**********************************************************************//**
 * Used for setting the time from a vector where:
 * 
 * @param time             vector specifying the time
 *                         - time[0] = hours
 *                         - time[1] = minutes
 *                         - time[2] = seconds
 *                         - time[3] = fractions of a second
 * @param time_format      Specifies what type is represented by 'time' (see ::CETimeType)
 *************************************************************************/
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


/**********************************************************************//**
 * Copy constructor
 * 
 * @param other
 *************************************************************************/
CETime::CETime(const CETime& other) :
    time_(other.time_),
    time_type_(other.time_type_)
{}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
CETime::~CETime()
{}

# pragma mark - Public Methods

/**********************************************************************//**
 * Get the current UTC time as seconds since midnight
 * 
 * @return Seconds since midnight
 *************************************************************************/
double CETime::CurrentUTC()
{
    // Construct time_t of this moment
    std::chrono::system_clock::time_point now(std::chrono::system_clock::now());
    std::time_t now_t(std::chrono::system_clock::to_time_t(
                        std::chrono::system_clock::time_point(now)));
    
    // Get the time at midnight
    struct tm midnight = *gmtime(&now_t) ;
    midnight.tm_hour = 0 ;
    midnight.tm_min = 0 ;
    midnight.tm_sec = 0 ;

    // Midnight object
    auto millisec_start(std::chrono::system_clock::from_time_t(mktime(&midnight)));

    double start = std::chrono::duration_cast<std::chrono::microseconds>(millisec_start.time_since_epoch()).count()/1000000.0 ;
    double stop  = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count()/1000000.0 ;
    
    // Return seconds and fractions of a second since midnight
    return stop-start-midnight.tm_gmtoff ;
}

/**********************************************************************//**
 * Get the current UTC time as a vector
 * 
 * @return Vector containing the various time components
 *         - time[0] = hours
 *         - time[1] = minutes
 *         - time[2] = seconds
 *         - time[3] = fractions of a second
 *************************************************************************/
std::vector<double> CETime::CurrentUTC_vect()
{
    return TimeSec2Vect( CurrentUTC() ) ;
}


/**********************************************************************//**
 * Get the current UTC time
 * 
 * @param jd           Julian date
 * @return             Seconds since midnight for given Julian date
 *************************************************************************/
double CETime::UTC(double jd)
{
    // Note that Julian date is defined such that midnight is at half julian days
    return (jd - (std::floor(jd+0.5)-0.5))*86400.0 ;
}

/**********************************************************************//**
 * Get the UTC time of a given julian date as a vector
 * 
 * @param jd           Julian date (with day fraction)
 * @return             Vector containing the various time components
 *                     - time[0] = hours
 *                     - time[1] = minutes
 *                     - time[2] = seconds
 *                     - time[3] = fractions of a second
 *************************************************************************/
std::vector<double> CETime::UTC_vect(double jd)
{
    return TimeDbl2Vect( TimeSec2Time( UTC(jd) ) ) ;
}


/**********************************************************************//**
 * Set time from double of the form HHMMSS.SS and a specified time format
 * @param time             HHMMSS.SS formated time variable
 *************************************************************************/
void CETime::SetTime(double time, CETimeType time_format)
{
    // Convert the double into a vector
    std::vector<double> time_vect = TimeDbl2Vect(time) ;
    
    // Call the vector version of SetTime
    SetTime(time_vect, time_format) ;
}


/**********************************************************************//**
 * Set the time from a vector and a user specified format
 * @param time_vect        vector specifying the time
 *                         - time[0] = hours
 *                         - time[1] = minutes
 *                         - time[2] = seconds
 *                         - time[3] = fractions of a second
 * @param time_format      Specifies what type is represented by 'time' (see ::CETimeType)
 *************************************************************************/
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

/**********************************************************************//**
 *************************************************************************/
void CETime::UTC2GAST()
{
    
}

/**********************************************************************//**
 *************************************************************************/
void CETime::UTC2LAST()
{
    
}

/**********************************************************************//**
 *************************************************************************/
void CETime::UTC2LOCALTIME()
{
    
}

# pragma mark - Protected Methods

/**********************************************************************//**
 * Convert a time formatted as HHMMSS.SS into a vector.
 * 
 * @param time         Vector containing the various time components
 *                         - time[0] = hours
 *                         - time[1] = minutes
 *                         - time[2] = seconds
 *                         - time[3] = fractions of a second
 * @return Time formated double as HHMMSS.S
 *************************************************************************/
double CETime::TimeVect2Dbl(std::vector<double> time)
{
    return time[0] * 10000 +
           time[1] * 100 +
           time[2] + time[3] ;
}

/**********************************************************************//**
 * Convert a time formatted as HHMMSS.SS into a vector.
 * 
 * @param time         Time object formatted as HHMMSS.SS
 * @return             Vector containing the various time components
 *                         - time[0] = hours
 *                         - time[1] = minutes
 *                         - time[2] = seconds
 *                         - time[3] = fractions of a second
 *************************************************************************/
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

/**********************************************************************//**
 * Convert number of seconds since midnight to HHMMSS.S formatted double
 * 
 * @param seconds          Seconds since midnight
 * @return Vector containing the various time components
 *         - time[0] = hours
 *         - time[1] = minutes
 *         - time[2] = seconds
 *         - time[3] = fractions of a second
 *************************************************************************/
double CETime::TimeSec2Time(double seconds)
{
    double fracsec = seconds - std::floor(seconds) ;    // Fractions of a second
    double sec = int(std::floor(seconds)) % 60 ;        // Whole seconds
    double min = int(std::floor(seconds-sec)/60) % 60 ; // Whole minutes
    double hrs = int(std::floor(seconds-sec)/60)/60 ;   // Whole hours
    return (hrs*10000) + (min*100) + sec + fracsec ;    // Formatted double (HHMMSS.S)
}

/**********************************************************************//**
 * Convert number of seconds since midnight to HHMMSS.S formatted double
 * 
 * @param seconds          Seconds since midnight
 * @return
 *************************************************************************/
std::vector<double> CETime::TimeSec2Vect(double seconds)
{
    return TimeDbl2Vect( TimeSec2Time(seconds) ) ;
}

/**********************************************************************//**
 * Set the time from a vector representing UTC time. The elements are as follows:
 * [0]=hours, [1]=minutes, [2]=whole seconds, [3]=fractional seconds
 *************************************************************************/
void CETime::SetTime_UTC(std::vector<double> time)
{
}


/**********************************************************************//**
 * Set the time from a vector representing Greenwich Apparent Sidereal Time.
 * The elements are as follows:
 * [0]=hours, [1]=minutes, [2]=whole seconds, [3]=fractional seconds
 *************************************************************************/
void CETime::SetTime_GAST(std::vector<double> time)
{
}


/**********************************************************************//**
 * Set the time from a vector representing Local Sidereal Time.
 * 
 * @param time         Vector representing LST time
 *                     - [0]=hours
 *                     - [1]=minutes
 *                     - [2]=whole seconds
 *                     - [3]=fractional seconds
 *************************************************************************/
void CETime::SetTime_LST(std::vector<double> time)
{
}


/**********************************************************************//**
 * Set the time from a vector representing local observer time.
 * The elements are as follows:
 * [0]=hours, [1]=minutes, [2]=whole seconds, [3]=fractional seconds
 *************************************************************************/
void CETime::SetTime_LOCALTIME(std::vector<double> time)
{
}

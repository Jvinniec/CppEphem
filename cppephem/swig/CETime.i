/***************************************************************************
 *  CETime.i: CppEphem                                                     *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2019 JCardenzana                                           *
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

/**
 * @file CETime.i
 * @brief SWIG file for CETime class
 * @author JCardenzana
 */
%{
/* Put headers and other declarations here that are needed for compilation */
// CppEphem HEADERS
#include "CETime.h"
%}

/**********************************************************************//**
 * Time type enum
 *************************************************************************/
enum class CETimeType : unsigned int {
    UTC       = 0,          ///< Coordinate Universal Time
    GAST      = 1,          ///< Greenwich Apparent Sidereal Time
    LAST      = 2,          ///< Local Apparent Sidereal Time
    LOCALTIME = 3           ///< Local time (defined as the UTC + timezone_shift_)
};

class CETime {
public:
    // Default constructor
    CETime();
    CETime(const double& time, 
           CETimeType time_format=CETimeType::UTC);
    CETime(std::vector<double> time, 
           CETimeType time_format=CETimeType::UTC);
    CETime(const CETime& other);
    virtual ~CETime();

    /*******************************************
     * Get the time
     *******************************************/
    
    double Hour(void) const;
    double Min(void) const;
    double Sec(void) const;

    static double CurrentUTC() ;
    static std::vector<double> CurrentUTC_vect() ;
    static double UTC(const double& jd) ;
    static std::vector<double> UTC_vect(const double& jd) ;
    
    // Convert a double of the form HHMMSS.S to a vector with
    // the same format as 'time_'
    static std::vector<double> TimeDbl2Vect(const double& time) ;
    static double              TimeVect2Dbl(std::vector<double> time) ;
    
    // Convert number of seconds since midnight to HHMMSS.S formatted double
    static double TimeSec2Time(const double& seconds) ;
    static std::vector<double> TimeSec2Vect(const double& seconds) ;
    
    static double SystemUTCOffset_hrs()
    {
        time_t now ;
        time (&now) ;
        struct tm local;
        localtime_r(&now, &local) ;
        return local.tm_gmtoff/3600.0;
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
};
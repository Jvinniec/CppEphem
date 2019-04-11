/***************************************************************************
 *  CERunningDate.cpp: CppEphem                                            *
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

#include <stdio.h>

#include "CERunningDate.h"

/** \class CERunningDate
  This class represents a date object that always represents
  the current instantaneous date
  <h2> Details </h2>
  Subsequent evaluations of the underlying methods return different values.
  The speed of ellapsed time by be computed using the SetTimerSpeed method,
  thus allowing the user to speed up, slow down, or even reverse the passage
  of time for this object.
 
 * The object may be evaluated in one of several ways to get the current value:
 * <ul>
 *      <li> Direct evaluation (object acts like a double)
 *      <li> Querrying the GetDate() method, which returns the time formatted
 *           according to the CEDate::return_type_ variable
 *      <li> Querrying one of the JD(), MJD(), or Gregorian() methods
 * </ul>
 */

/**********************************************************************//**
 * Default constructor
 *************************************************************************/
CERunningDate::CERunningDate() :
    CEDate()
{
    init_members();
}


/**********************************************************************//**
 * Copy constructor
 * 
 * @param[in] other             CERunningDate object to copy from
 *************************************************************************/
CERunningDate::CERunningDate(const CERunningDate& other)
{
    init_members();
    copy_members(other);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
CERunningDate::~CERunningDate()
{
    free_members();
}


/**********************************************************************//**
 * Copy assignment operator
 * 
 * @param[in] other             CERunningDate object to copy from
 *************************************************************************/
CERunningDate& CERunningDate::operator=(const CERunningDate& other)
{
    if (this != &other) {
        this->CEDate::operator=(other);
        free_members();
        init_members();
        copy_members(other);
    }
    return *this;
}

/**********************************************************************//**
 * Get the current Julian date.
 * 
 * @return Instantaneous Julian date
 *************************************************************************/
double CERunningDate::JD() const
{
    // Get the current Julian date
    return CEDate::JD() + (ScaledRunTime()/86400.0) ;
}

/**********************************************************************//**
 * Get the current modified Julian date.
 * 
 * @return Instantaneous modified Julian date
 *************************************************************************/
double CERunningDate::MJD() const
{
    return JD2MJD( this->JD() ) ;
}

/**********************************************************************//**
 * Get the current Gregorian calendar date formatted as YYYYMMDD.D
 * 
 * @return Instantaneous Gregorian calendar date formatted as YYYYMMDD.D
 *************************************************************************/
double CERunningDate::Gregorian() const
{
    return JD2Gregorian( this->JD() ) ;
}

/**********************************************************************//**
 * Set the date based on an actual date and the desired time_format.
 * This method will also reset the underlying timer.
 * 
 * @param date             Date
 * @param time_format      Time format (see ::CEDateType)
 *************************************************************************/
void CERunningDate::SetDate(const double&     date, 
                            const CEDateType& time_format)
{
    CEDate::SetDate(date, time_format) ;
    ResetTime() ;
}

/**********************************************************************//**
 * Set the date based on an actual date and the desired time_format
 * 
 * @param date             Gregorian Date
 *                         - [0] = Year
 *                         - [1] = Month
 *                         - [2] = Day
 *                         - [3] = Day fraction
 *************************************************************************/
void CERunningDate::SetDate(std::vector<double> date)
{
    SetDate(GregorianVect2JD(date)) ;
}

/**********************************************************************//**
 * Get the number of seconds since the creation of this object
 *************************************************************************/
double CERunningDate::RunTime() const
{
    // Get the current system time in nanoseconds
    std::chrono::nanoseconds cur_time(std::chrono::high_resolution_clock::now().time_since_epoch());
    std::chrono::nanoseconds start_time(start_.time_since_epoch());

    // Return the difference in seconds
    return (cur_time.count() - start_time.count())/1000000000.0;
}


//-------------------------------------------------
// PRIVATE METHODS
//-------------------------------------------------


/**********************************************************************//**
 * Copy data members from another object
 * 
 * @param[in] other             CERunningDate object to copy from
 *************************************************************************/
void CERunningDate::copy_members(const CERunningDate& other)
{
    start_              = other.start_;
    timer_speed_factor_ = other.timer_speed_factor_;
}


/**********************************************************************//**
 * Initialize data members
 *************************************************************************/
void CERunningDate::init_members(void)
{
    start_              = std::chrono::high_resolution_clock::now();
    timer_speed_factor_ = 1.0;
}


/**********************************************************************//**
 * Deallocate data members
 *************************************************************************/
void CERunningDate::free_members(void)
{
    return;
}

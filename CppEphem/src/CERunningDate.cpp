//
//  CERunningDate.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 1/19/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

#include <stdio.h>

#include "CERunningDate.h"

/** \class CERunningDate
 * The CERunningDate class
 * This class represents a date object that always represents
 * the current instantaneous date
 * <h2> Details </h2>
 * Subsequent evaluations of the underlying methods return different values.
 * The speed of ellapsed time by be computed using the SetTimerSpeed method,
 * thus allowing the user to speed up, slow down, or even reverse the passage
 * of time for this object.
 *
 * The object may be evaluated in one of several ways to get the current value:
 * <ul>
 *      <li> Direct evaluation (object acts like a double)
 *      <li> Querrying the GetDate() method, which returns the time formatted
 *           according to the CEDate::return_type_ variable
 *      <li> Querrying one of the JD(), MJD(), or Gregorian() methods
 * </ul>
 */

///////////////////////////////////////////////////////////////
/// Default constructor
CERunningDate::CERunningDate() :
    CEDate()
{}

///////////////////////////////////////////////////////////////
/// Destructor
CERunningDate::~CERunningDate()
{}

#pragma mark - Public Methods.

///////////////////////////////////////////////////////////////
/// Get the current Julian date.
///     @return Instantaneous Julian date
double CERunningDate::JD()
{
    // Get the current Julian date
    return julian_date_ + (ScaledRunTime()/86400.0) ;
}

///////////////////////////////////////////////////////////////
/// Get the current modified Julian date.
///     @return Instantaneous modified Julian date
double CERunningDate::MJD()
{
    return JD2MJD( JD() ) ;
}

///////////////////////////////////////////////////////////////
/// Get the current Gregorian calendar date formatted as YYYYMMDD.D
///     @return Instantaneous Gregorian calendar date formatted as YYYYMMDD.D
double CERunningDate::Gregorian()
{
    return JD2Gregorian( JD() ) ;
}

/////////////////////////////////////////////////////////////////
/// Set the date based on an actual date and the desired time_format.
/// This method will also reset the underlying timer.
///     @param date             Date
///     @param time_format      Time format (see ::CEDateType)
void CERunningDate::SetDate(double date, CEDateType time_format)
{
    CEDate::SetDate(date, time_format) ;
    ResetTime() ;
}

/////////////////////////////////////////////////////////////////
/// Set the date based on an actual date and the desired time_format
///     @param date             Gregorian Date
///                             - [0] = Year
///                             - [1] = Month
///                             - [2] = Day
///                             - [3] = Day fraction
void CERunningDate::SetDate(std::vector<double> date)
{
    SetDate(GregorianVect2JD(date)) ;
}

#pragma mark - Protected Methods
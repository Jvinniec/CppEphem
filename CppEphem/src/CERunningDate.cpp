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
 * This class represents a date object that always represents
 * the current instantaneous date. Subsequent evaluations of
 * the underlying methods return different values.
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
    return julian_date_ + (RunTime()/86400.0) ;
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

#pragma mark - Protected Methods
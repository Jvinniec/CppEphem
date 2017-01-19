//
//  CEDate.cpp
//  AstroCoordinates
//
//  Created by Josh Cardenzana on 4/3/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

/** \class CEDate
 CEDate class handles conversion between different date formats.
 Date formats used (see ::CEDateType):
  - JD: Julian date
  - MJD: Modified Julian date
  - GREGORIAN: Gregorian calendar date
 
 To set the date, use the CEDate::SetDate method.
 */

#include <cmath>
#include <exception>
#include <stdio.h>

#include "CEDate.h"

/////////////////////////////////////////////////////////////////
/// Constructor from some date format.
/// Different formats are as follows:
/// - JD: Julian date
/// - MJD: Modified Julian date
/// - GREGORIAN: Gregorian calendar date
CEDate::CEDate(double date, CEDateType date_format) :
    julian_date_(0.0),
    mod_julian_date_(0.0),
    gregorian_date_(0.0),
    gregorian_date_vect_(std::vector<double>(4,0.0))
{
    // Use the SetDate function to set the actual time
    SetDate(date, date_format) ;
}

/////////////////////////////////////////////////////////////////
/// Constructor from a set of Gregorian values.
///     @param date Gregorian date vector of the form
///                 - date[0] = year
///                 - date[1] = month
///                 - date[2] = day
///                 - date[3] = day fraction
CEDate::CEDate(std::vector<double> date) :
    julian_date_(0.0),
    mod_julian_date_(0.0),
    gregorian_date_(0.0),
    gregorian_date_vect_(std::vector<double>(4,0.0))
{
    // First get the gregorian date double from the vector, then set the date
    SetDate(GregorianVect2Gregorian(date), CEDateType::GREGORIAN) ;
}

/////////////////////////////////////////////////////////////////
/// Copy constructor
CEDate::CEDate(const CEDate& other) :
    julian_date_(other.julian_date_),
    mod_julian_date_(other.mod_julian_date_),
    gregorian_date_(other.gregorian_date_),
    gregorian_date_vect_(other.gregorian_date_vect_)
{}

#pragma mark - Public Methods

/////////////////////////////////////////////////////////////////
/// Set the date based on an actual date and the desired time_format
///     @param date             Date
///     @param time_format      Time format (see ::CEDateType)
void CEDate::SetDate(double date, CEDateType time_format)
{
    // Fill the internal date storage objects based on the format of the input "date"
    if (time_format == CEDateType::JD) {
        // Save information based on julian date input
        julian_date_ = date ;
        mod_julian_date_ = JD2MJD(date) ;
        gregorian_date_ = JD2Gregorian(date) ;
        gregorian_date_vect_ = JD2GregorianVect(date) ;
    } else if (time_format == CEDateType::MJD) {
        // Save information based on modified julian date input
        julian_date_ = MJD2JD(date) ;
        mod_julian_date_ = date ;
        gregorian_date_ = MJD2Gregorian(date) ;
        gregorian_date_vect_ = MJD2GregorianVect(date) ;
    } else if (time_format == CEDateType::GREGORIAN) {
        // Save information based on gregorian date input
        julian_date_ = Gregorian2JD(date) ;
        mod_julian_date_ = Gregorian2MJD(date) ;
        gregorian_date_ = date ;
        gregorian_date_vect_ = Gregorian2GregorianVect(date) ;
    }
}

/////////////////////////////////////////////////////////////////
/// Set the date based on an actual date and the desired time_format
///     @param date             Gregorian Date
///                             - [0] = Year
///                             - [1] = Month
///                             - [2] = Day
///                             - [3] = Day fraction
void CEDate::SetDate(std::vector<double> date)
{
    SetDate(GregorianVect2JD(date)) ;
}

/////////////////////////////////////////////////////////////////
/// Return the date in a given format
///     @param time_format Format of the output time object (see ::CEDateType)
double CEDate::GetDate(CEDateType time_format)
{
    // Return the julian date if requested
    if (time_format == CEDateType::JD) {
        return JD() ;
    } else if (time_format == CEDateType::MJD) {
        return MJD() ;
    } else if (time_format == CEDateType::GREGORIAN) {
        return Gregorian() ;
    } else {
        throw std::runtime_error("[ERROR] CEDate::GetDate() :: Unrecognized time format!") ;
        return -1.0 ;
    }
}

#pragma mark - Julian Date Converters

/////////////////////////////////////////////////////////////////
/// Julian date -> modified Julian date conversion method
///     @param jd               Julian date
///     @return modified Julian date
double CEDate::JD2MJD(double jd)
{
    return jd - GetMJD2JDFactor();
}

/////////////////////////////////////////////////////////////////
/// Julian date -> Gregorian calendar date conversion method.
///     @param jd               Julian date
///     @return Gregorian calendar date of the form YYYYMMDD.DD
double CEDate::JD2Gregorian(double jd)
{
    // Fill a temporary gregorian vector
    std::vector<double> temp_gregorian_vect = JD2GregorianVect(jd) ;
    return GregorianVect2Gregorian(temp_gregorian_vect) ;
}

/////////////////////////////////////////////////////////////////
/// Julian date -> modified Julian date conversion method. This function
/// makes use of the SOFA 'iauJd2cal' method.
///     @param jd               Julian date
///     @return Gregorian calendar date as a 4-element vector
///             - [0] = year
///             - [1] = month
///             - [2] = day
///             - [3] = day fraction
std::vector<double> CEDate::JD2GregorianVect(double jd)
{
    std::vector<double> full_gregorian_vect(4,0.0) ;
    std::vector<int> gregorian_date(3,0.0) ;
    
    // Try to fill the gregorian_date vector
    if (iauJd2cal(jd, 0, &gregorian_date[0], &gregorian_date[1],
                   &gregorian_date[2], &full_gregorian_vect[3])) {
        std::cerr << "[WARNING] CEDate::JD2GregorianVect() :: Bad date (" << jd << ")!" << std::endl ;
        return std::vector<double>(4,0.0) ;
    } else {
        // Fill the double vector from the integer values
        full_gregorian_vect[0] = 1.0*gregorian_date[0] ;
        full_gregorian_vect[1] = 1.0*gregorian_date[1] ;
        full_gregorian_vect[2] = 1.0*gregorian_date[2] ;
    }
    
    return full_gregorian_vect ;
}

# pragma mark - Modified Julian Date Converters

// Note that the modified julian date converters first do a conversion
// to Julian Date and then call the Julian Date converters

/////////////////////////////////////////////////////////////////
/// Modified Julian date -> Julian date conversion method
///     @param mjd              Modified Julian date
///     @return Julian date
double CEDate::MJD2JD(double mjd)
{return mjd + GetMJD2JDFactor();}

/////////////////////////////////////////////////////////////////
/// Modified Julian date -> Gregorian calendar date conversion method
///     @param mjd              Modified Julian date
///     @return Gregorian calendar date of the form YYYYMMDD.DD
double CEDate::MJD2Gregorian(double mjd)
{
    // Convert the mjd to jd
    double jd = MJD2JD(mjd) ;
    // Now convert julian date to gregorian
    return JD2Gregorian(jd) ;
}

/////////////////////////////////////////////////////////////////
/// Modified Julian date -> Gregorian calendar, vector formatted date
///     @param mjd              Modified Julian date
///     @return Gregorian calendar date as a 4-element vector
///             - [0] = year
///             - [1] = month
///             - [2] = day
///             - [3] = day fraction
std::vector<double> CEDate::MJD2GregorianVect(double mjd)
{
    // Convert mjd to jd
    double jd = MJD2JD(mjd) ;
    // Now convert julian date to gregorian vector
    return JD2GregorianVect(jd) ;
}

# pragma mark - Gregorian Date Converters

/////////////////////////////////////////////////////////////////
/// Gregorian calendar date -> Julian date
///     @param gregorian        Gregorian calendar date formatted as YYYYMMDD.D
///     @return Julian date
double CEDate::Gregorian2JD(double gregorian)
{
    // First convert the gregorian double into a gregorian vector
    std::vector<double> gregorian_vect = Gregorian2GregorianVect(gregorian) ;
    
    return GregorianVect2JD(gregorian_vect) ;
}

/////////////////////////////////////////////////////////////////
/// Gregorian calendar vector formatted date -> Julian date converter.
/// Calls the SOFA "iauCal2jd" function
///     @param gregorian            Gregorian calendar date as a 4-element vector
///                                 - [0] = year
///                                 - [1] = month
///                                 - [2] = day
///                                 - [3] = day fraction
///     @return Julian date
double CEDate::GregorianVect2JD(std::vector<double> gregorian)
{
    // The following stores the two values needed to get the julian date
    double mjd_factor(0.0), mjd(0.0) ;
    
    // Now use the sofa function for converting to julian date
    int err_code = iauCal2jd(int(gregorian[0]), int(gregorian[1]),
                             int(gregorian[2]), &mjd_factor, &mjd) ;
    if (err_code) {
        std::cerr << "[WARNING] CEDate::GregorianVect2JD() :: Bad ";
        if (err_code == -1) {
            std::cerr << "year (" << gregorian[0] << "). Make sure the year is larger than -4800." << std::endl ;
            return 0.0 ;
        } else if (err_code == -2) {
            std::cerr << "month (" << gregorian[1] << "). Make sure this number is in the range 1 -> 12." << std::endl ;
            return 0.0 ;
        } else if (err_code == -3) {
            std::cerr << "day (" << gregorian[2] << ")" << std::endl ;
            // Note that in this case, a julian date was still calculated
            // so we can go ahead and return it
        }
    }
    
    // Return the sum of the two filled date values which represents
    // the Julian date
    return mjd_factor + mjd + gregorian[3] ;
}

/////////////////////////////////////////////////////////////////
/// Gregorian calendar formatted date -> Julian date converter.
///     @param gregorian            Gregorian calendar date formatted as YYYYMMDD.D
///     @return Modified Julian Date
double CEDate::Gregorian2MJD(double gregorian)
{
    // First get the Julian Date
    double jd = Gregorian2JD(gregorian) ;
    // Now convert to Modified Julian Date
    return JD2MJD(jd) ;
}

/////////////////////////////////////////////////////////////////
/// Gregorian calendar vector formatted date -> Modified Julian date converter.
///     @param gregorian            Gregorian calendar date as a 4-element vector
///                                 - [0] = year
///                                 - [1] = month
///                                 - [2] = day
///                                 - [3] = day fraction
///     @return Modified Julian date
double CEDate::GregorianVect2MJD(std::vector<double> gregorian)
{
    // First convert to JD
    double jd = GregorianVect2JD(gregorian) ;
    // Now convert to MJD
    return JD2MJD(jd) ;
}


#pragma mark - Helper Methods

//_______________________________________________________________
double CEDate::dut1(double date, CEDateType date_type)
{
    // Create an object from the date information
    CEDate input_date(date, date_type) ;
    // Return the DUT1 value at this date
    return input_date.dut1() ;
}

//_______________________________________________________________
double CEDate::dut1()
{
    // return the value associated with 'UT1-UTC'
    return CppEphem::dut1(mod_julian_date_) ;
}

//_______________________________________________________________
double CEDate::dut1Error(double date, CEDateType date_type)
{
    CEDate input_date(date, date_type) ;
    return input_date.dut1Error() ;
}

//_______________________________________________________________
double CEDate::dut1Error()
{
    // Return the error on the dut1 value
    return CppEphem::dut1Error(mod_julian_date_) ;
}


//_______________________________________________________________
double CEDate::xpolar(double date, CEDateType date_type)
{
    CEDate input_date(date, date_type) ;
    return CppEphem::xp( input_date.JD() ) ;
}


//_______________________________________________________________
double CEDate::xpolar()
{
    return CppEphem::xp( julian_date_ ) ;
}


//_______________________________________________________________
double CEDate::ypolar(double date, CEDateType date_type)
{
    CEDate input_date(date, date_type) ;
    return CppEphem::yp( input_date.JD() ) ;
}

//_______________________________________________________________
double CEDate::ypolar()
{
    return CppEphem::yp( julian_date_ ) ;
}

/////////////////////////////////////////////////////////////////
/// Helper method for converting from Gregorian vector format to
/// the non-vector format
double CEDate::GregorianVect2Gregorian(std::vector<double> gregorian)
{
    return gregorian[0] * 10000 +
           gregorian[1] * 100 +
           gregorian[2] + gregorian[3] ;
}


/////////////////////////////////////////////////////////////////
/// Helper method for converting from non-vector formatted Gregorian
/// date to vector format Gregorian date
std::vector<double> CEDate::Gregorian2GregorianVect(double gregorian)
{
    // Create a vector to hold the information
    std::vector<double> gregorian_vect(4,0.0) ;
    
    // Get the day fraction
    gregorian_vect[3] = gregorian - std::floor(gregorian) ;
    // Get the day of the month
    gregorian_vect[2] = int(std::floor(gregorian)) % 100 ;
    // Get the month
    gregorian_vect[1] = int(std::floor(gregorian - gregorian_vect[2]))/100 % 100 ;
    // Get the year
    gregorian_vect[0] = (std::floor(gregorian - gregorian_vect[2] - gregorian_vect[1])/10000) ;
    
    return gregorian_vect ;
}

/////////////////////////////////////////////////////////////////
/// Method for getting the number of seconds since midnight. Set the
/// 'utc_offset' in order to get local relative seconds since midnight.
///         @param utc_offset       Observer UTC offset (dont forget about daylight saving time)
///         @return Seconds since midnight
double CEDate::GetSecondsSinceMidnight(double utc_offset)
{
    double jd_offset = julian_date_ + (utc_offset/24.0) ;
    return CETime::UTC( jd_offset ) ;
}

/////////////////////////////////////////////////////////////////
/// Method for getting the current time. Set the 'utc_offset' in order
/// to get local time
///         @param utc_offset       Observer UTC offset (dont forget about daylight saving time)
///         @return Current time formatted as HHMMSS.S
double CEDate::GetTime(double utc_offset)
{
    double jd_offset = JD() + (utc_offset/24.0) ;
    return CETime::TimeSec2Time( CETime::UTC( jd_offset ) ) ;
}

/////////////////////////////////////////////////////////////////
/// Method for getting the current UTC time.
///         @return Current time formatted as HHMMSS.S
double CEDate::GetTime_UTC()
{
    return GetTime() ;
}

/////////////////////////////////////////////////////////////////
/// Static method for getting the current Julian date.
///         @return Current Julian date
double CEDate::CurrentJD()
{
    // Get the current date information
    time_t now ;
    time (&now) ;
    struct tm current_jd = *gmtime(&now) ;
    
    // Put this into a vector
    std::vector<double> date_info = {current_jd.tm_year+1900.0,
                                     current_jd.tm_mon+1.0,
                                     current_jd.tm_mday*1.0,
                                     CETime::CurrentUTC()/DAYSEC} ;
    
    // Convert the Gregorian date information into a Julian date
    return GregorianVect2JD( date_info ) ;
}


# pragma mark - Protected methods


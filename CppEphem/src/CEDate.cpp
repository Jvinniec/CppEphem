//
//  CEDate.cpp
//  AstroCoordinates
//
//  Created by Josh Cardenzana on 4/3/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include <cmath>
#include <exception>
#include <stdio.h>

#include "CEDate.h"

//_______________________________________________________________
CEDate::CEDate(double date, CEDateFormat date_format) :
    julian_date_(0.0),
    mod_julian_date_(0.0),
    gregorian_date_(0.0),
    gregorian_date_vect_(std::vector<double>(4,0.0))
{
    // Use the SetDate function to set the actual time
    SetDate(date, date_format) ;
}

//_______________________________________________________________
CEDate::CEDate(std::vector<double> date) :
    julian_date_(0.0),
    mod_julian_date_(0.0),
    gregorian_date_(0.0),
    gregorian_date_vect_(std::vector<double>(4,0.0))
{
    // First get the gregorian date double from the vector, then set the date
    SetDate(GregorianVect2Gregorian(date), CEDateFormat::GREGORIAN) ;
}

//_______________________________________________________________
// Copy constructor
CEDate::CEDate(const CEDate& other) :
    julian_date_(other.julian_date_),
    mod_julian_date_(other.mod_julian_date_),
    gregorian_date_(other.gregorian_date_),
    gregorian_date_vect_(other.gregorian_date_vect_)
{}

#pragma mark - Public Methods

//_______________________________________________________________
void CEDate::SetDate(double date, CEDateFormat time_format)
{
    // Fill the internal date storage objects based on the format of the input "date"
    if (time_format == CEDateFormat::JD) {
        // Save information based on julian date input
        julian_date_ = date ;
        mod_julian_date_ = JD2MJD(date) ;
        gregorian_date_ = JD2Gregorian(date) ;
        gregorian_date_vect_ = JD2GregorianVect(date) ;
    } else if (time_format == CEDateFormat::MJD) {
        // Save information based on modified julian date input
        julian_date_ = MJD2JD(date) ;
        mod_julian_date_ = date ;
        gregorian_date_ = MJD2Gregorian(date) ;
        gregorian_date_vect_ = MJD2GregorianVect(date) ;
    } else if (time_format == CEDateFormat::GREGORIAN) {
        // Save information based on gregorian date input
        julian_date_ = Gregorian2JD(date) ;
        mod_julian_date_ = Gregorian2MJD(date) ;
        gregorian_date_ = date ;
        gregorian_date_vect_ = Gregorian2GregorianVect(date) ;
    }
}

//_______________________________________________________________
double CEDate::GetDate(CEDateFormat time_format)
{
    // Return the julian date if requested
    if (time_format == CEDateFormat::JD) {
        return JD() ;
    } else if (time_format == CEDateFormat::MJD) {
        return MJD() ;
    } else if (time_format == CEDateFormat::GREGORIAN) {
        return Gregorian() ;
    } else {
        throw std::runtime_error("[ERROR] CEDate::GetDate() :: Unrecognized time format!") ;
        return -1.0 ;
    }
}

#pragma mark - Julian Date Converters

//_______________________________________________________________
double CEDate::JD2MJD(double jd)
{return jd - GetMJD2JDFactor();}

//_______________________________________________________________
double CEDate::JD2Gregorian(double jd)
{
    // Fill a temporary gregorian vector
    std::vector<double> temp_gregorian_vect = JD2GregorianVect(jd) ;
    return GregorianVect2Gregorian(temp_gregorian_vect) ;
}

//_______________________________________________________________
// Uses the SOFA function "iauJd2cal"
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

//_______________________________________________________________
double CEDate::MJD2JD(double mjd)
{return mjd + GetMJD2JDFactor();}

//_______________________________________________________________
double CEDate::MJD2Gregorian(double mjd)
{
    // Convert the mjd to jd
    double jd = MJD2JD(mjd) ;
    // Now convert julian date to gregorian
    return JD2Gregorian(jd) ;
}

//_______________________________________________________________
std::vector<double> CEDate::MJD2GregorianVect(double mjd)
{
    // Convert mjd to jd
    double jd = MJD2JD(mjd) ;
    // Now convert julian date to gregorian vector
    return JD2GregorianVect(jd) ;
}

# pragma mark - Gregorian Date Converters

//_______________________________________________________________
double CEDate::Gregorian2JD(double gregorian)
{
    // First convert the gregorian double into a gregorian vector
    std::vector<double> gregorian_vect = Gregorian2GregorianVect(gregorian) ;
    
    return GregorianVect2JD(gregorian_vect) ;
}

//_______________________________________________________________
// Calls the SOFA "iauCal2jd" function
double CEDate::GregorianVect2JD(std::vector<double> gregorian)
{
    // The following stores the two values needed to get the julian date
    double mjd_factor(0.0), mjd(0.0) ;
    
    // Now use the sofa function for converting to julian date
    if (iauCal2jd(int(gregorian[0]), int(gregorian[1]),
                  int(gregorian[2]), &mjd_factor, &mjd)) {
        std::cerr << "[WARNING] CEDate::JD2GregorianVect() :: Bad date ("
                  << "Y=" << gregorian[0] << ", "
                  << "M=" << gregorian[1] << ", "
                  << "D=" << gregorian[2]+gregorian[3] << ")!" << std::endl ;
        return 0 ;
    }
    
    // Return the sum of the two filled date values which represents
    // the Julian date
    return mjd_factor + mjd ;
}

//_______________________________________________________________
double CEDate::Gregorian2MJD(double gregorian)
{
    // First get the Julian Date
    double jd = Gregorian2JD(gregorian) ;
    // Now convert to Modified Julian Date
    return JD2MJD(jd) ;
}

//_______________________________________________________________
double CEDate::GregorianVect2MJD(std::vector<double> gregorian)
{
    // First convert to JD
    double jd = GregorianVect2JD(gregorian) ;
    // Now convert to MJD
    return JD2MJD(jd) ;
}


#pragma mark - Helper Methods

//_______________________________________________________________
double CEDate::GregorianVect2Gregorian(std::vector<double> gregorian)
{
    return gregorian[0] * 10000 +
           gregorian[1] * 100 +
           gregorian[2] + gregorian[3] ;
}


//_______________________________________________________________
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

# pragma mark - Protected methods


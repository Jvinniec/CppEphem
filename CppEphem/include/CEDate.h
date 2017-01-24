//
//  CEDate.h
//  AstroCoordinates
//
//  Created by Josh Cardenzana on 4/3/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#ifndef CEDate_h
#define CEDate_h

#include <iostream>
#include <string>
#include <vector>

// CppEphem HEADERS
#include "CETime.h"
#include <CENamespace.h>

// SOFA HEADER
#include "sofa.h"

/////////////////////////////////////////////
/// Date enum

enum CEDateType {JD,              ///< Julian Date
                 MJD,             ///< Modified Julian Date
                 GREGORIAN        ///< Gregorian calendar (year, month, day)
                } ;

class CEDate {
public:
    // Default constructor
    CEDate(double date=CurrentJD(), CEDateType date_format=CEDateType::JD) ;
    CEDate(std::vector<double> date) ;
    // Copy constructor
    CEDate(const CEDate& other) ;
    // Destructor
    virtual ~CEDate() {} ;
    
    // Method that can be used to change the date that is stored in this object
    virtual void SetDate(double date=CurrentJD(), CEDateType time_format=CEDateType::JD) ;
    // Method for setting the dates from the Gregorian calendar dates
    virtual void SetDate(std::vector<double> date) ;
    
    /***********************************************************
     * Methods for getting the stored date in the various formats
     ***********************************************************/
    double GetDate(CEDateType time_format=CEDateType::JD) ;
    /// Get the Julian date represented by this object
    virtual double JD() {return julian_date_ ;}
    /// Get the Modified Julian date represented by this object
    virtual double MJD() {return mod_julian_date_ ;}
    /// Get the Gregorian calendar date formatted as a double
    virtual double Gregorian() {return gregorian_date_ ;}
    /// Get the Gregorian calendar date formatted as a vector
    virtual std::vector<double> GregorianVect() {return gregorian_date_vect_ ;}
    
    /// Get the Gregorian calendar year
    int Year() {return gregorian_date_vect_[0] ;}
    /// Get the Gregorian calendar month
    int Month() {return gregorian_date_vect_[1] ;}
    /// Get the Gregorian calendar day
    int Day() {return gregorian_date_vect_[2] ;}
    /// Get the Gregorian calendar day fraction
    double DayFraction() {return gregorian_date_vect_[3] ;}
    
    /***********************************************************
     * Methods for converting between different formats
     ***********************************************************/
    // Methods for converting from julian date (JD) to something else
    static double JD2MJD(double jd) ;
    static double JD2Gregorian(const double jd) ;
    static std::vector<double> JD2GregorianVect(double jd) ;
    
    // Methods for converting from modified julian date to something else
    static double MJD2JD(double mjd) ;
    static double MJD2Gregorian(double mjd) ;
    static std::vector<double> MJD2GregorianVect(double mjd) ;
    
    // Methods for converting from Gregorian calendar date to something else
    static double Gregorian2JD(double gregorian) ;
    static double GregorianVect2JD(std::vector<double> gregorian) ;
    static double Gregorian2MJD(double gregorian) ;
    static double GregorianVect2MJD(std::vector<double> gregorian) ;
    
    /***********************************************************
     * Some useful helper methods
     ***********************************************************/
    
    /// Gets the stored SOFA Julian date to Mod Julian date factor 'DJM0'.
    ///     @return conversion factor for Julian date -> Modified Julian date conversions
    /// Modified Julian date (mjd) can then be computed from this factor
    /// and the Julian date (jd) by the following:
    ///
    ///     mjd = jd - CEDate::GetMJD2JDFactor()
    static inline double GetMJD2JDFactor() {return DJM0;}
    
    /// Returns the 'dut1' value representing the 'UTC-UT1. These methods should
    /// be considered unreliable for the moment.
    static double dut1(double date, CEDateType date_type=CEDateType::JD) ;
    double dut1() ;
    static double dut1Error(double date, CEDateType date_type=CEDateType::JD) ;
    double dut1Error() ;
    
    /// Get the x,y polar motion variables associated with a given date. These
    /// methods should be considered unreliable for the moment.
    static double xpolar(double date, CEDateType date_type=CEDateType::JD) ;
    double xpolar() ;
    static double ypolar(double date, CEDateType date_type=CEDateType::JD) ;
    double ypolar() ;
    
    /// Methods for converting between the double and vector<double> version of
    /// the Gregorian date format.
    static double GregorianVect2Gregorian(std::vector<double> gregorian) ;
    static std::vector<double> Gregorian2GregorianVect(double gregorian) ;
    
    // Method for getting the current time. Set the 'utc_offset' in order
    // to get local coordinates
    virtual double GetSecondsSinceMidnight(double utc_offset=0.0) ;
    virtual double GetTime(double utc_offset=0.0) ;
    virtual double GetTime_UTC() ;
    
    // Returns the current Julian date
    static double CurrentJD() ;
    
    /************************************************************
     * Overloaded operators
     ************************************************************/
    /// Overload of the CEDate object which allows the object to be treated as a
    /// 'double' representing the Julian date of teh object.
    virtual operator double()
    {
        // Return the date formatted according to the 'return_type_' variable
        return GetDate(return_type_) ;
    }
    
    /// Set the return type from the overloaded 'operator double'.
    ///     @param return_type      ::CEDateType for the returned double
    void SetReturnType(CEDateType return_type) {return_type_ = return_type;}
    
protected:
    
    /************************************************************
     * Variables that hold the time information
     ************************************************************/
    double julian_date_ ;                   ///< Julian date formated
    double mod_julian_date_ ;               ///< Modified Julian date formated
    double gregorian_date_ ;                ///< Gregorian calendar date. Format as YYYYMMDD.DD
    std::vector<double> gregorian_date_vect_ ;   ///< Vector containing the gregorian calendar date
                                                ///< 0 - Year, 1 - Month, 2 - date, 3 - date fraction
    CEDateType return_type_ = CEDateType::JD ;  ///< what format the 'operator double' will return
    
private:
    
};

#endif /* CEDate_h */

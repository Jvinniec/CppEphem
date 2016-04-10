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

// SOFA HEADER
#include "sofa.h"

enum CEDateType {JD,              // Julian Date
                 MJD,             // Modified Julian Date
                 GREGORIAN} ;     // Gregorian calendar (year, month, day)

class CEDate {
public:
    // Default constructor
    CEDate() {} ;
    // Constructor from some date format
    CEDate(double date, CEDateType date_format=CEDateType::JD) ;
    CEDate(std::vector<double> date) ;
    // Copy constructor
    CEDate(const CEDate& other) ;
    // Destructor
    virtual ~CEDate() {} ;
    
    // Method that can be used to change the date that is stored in this object
    virtual void SetDate(double date, CEDateType time_format=CEDateType::JD) ;
    // Method for setting the dates from the Gregorian
    virtual void SetDate(std::vector<double> date) {} ;
    
    /***********************************************************
     * Methods for getting the stored date in the various formats
     ***********************************************************/
    double GetDate(CEDateType time_format=CEDateType::JD) ;
    double JD() {return julian_date_ ;}
    double MJD() {return mod_julian_date_ ;}
    double Gregorian() {return gregorian_date_ ;}
    std::vector<double> GregorianVect() {return gregorian_date_vect_ ;}
    
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
    // Gets the stored SOFA Julian date to Mod Julian date factor
    static inline double GetMJD2JDFactor() {return DJM0;}
    
    // Get the dut1 variable (i.e. UT1-UTC) associated with a given date
    static double dut1(double date, CEDateType date_type=CEDateType::JD) ;
    double dut1() ;
    static double dut1Error(double date, CEDateType date_type=CEDateType::JD) ;
    double dut1Error() ;
    
    // Methods for converting between the double and vector<double> version of
    // the Gregorian date format
    static double GregorianVect2Gregorian(std::vector<double> gregorian) ;
    static std::vector<double> Gregorian2GregorianVect(double gregorian) ;
    
protected:
    // Variables that hold the time information
    double julian_date_ ;                   // Time stored as the julian date
    double mod_julian_date_ ;               // Time stored as the modified julian date
    double gregorian_date_ ;                // gregorian calendar date. Format is YYYYMMDD.<date fraction>
    std::vector<double> gregorian_date_vect_ ;   // Vector containing the gregorian calendar date
                                            // 0 - Year, 1 - Month, 2 - date, 3 - date fraction
    
    
private:
    
};

#endif /* CEDate_h */

/***************************************************************************
 *  CEDate.h: CppEphem                                                     *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016-2019 JCardenzana                                      *
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

/**********************************************************************//**
 * Date enum
 *************************************************************************/
enum CEDateType {JD,              ///< Julian Date
                 MJD,             ///< Modified Julian Date
                 GREGORIAN        ///< Gregorian calendar (year, month, day)
                } ;

class CEDate {
public:
    // Default constructor
    CEDate(double date=CurrentJD(), CEDateType date_format=CEDateType::JD) ;
    CEDate(std::vector<double> date) ;
    explicit CEDate(const CEDate& other) ;
    virtual ~CEDate();
    
    // Method that can be used to change the date that is stored in this object
    virtual void SetDate(double date=CurrentJD(), CEDateType time_format=CEDateType::JD) ;
    // Method for setting the dates from the Gregorian calendar dates
    virtual void SetDate(std::vector<double> date) ;
    
    /***********************************************************
     * Methods for getting the stored date in the various formats
     ***********************************************************/
    double                      GetDate(CEDateType time_format=CEDateType::JD) const;
    virtual double              JD() const;
    virtual double              MJD() const;
    virtual double              Gregorian() const;
    virtual std::vector<double> GregorianVect();
    int                         Year();
    int                         Month();
    int                         Day();
    double                      DayFraction();
    
    /***********************************************************
     * Methods for converting between different formats
     ***********************************************************/
    static double               Gregorian2JD(double gregorian);
    static double               GregorianVect2JD(std::vector<double> gregorian);
    static double               Gregorian2MJD(double gregorian);
    static double               GregorianVect2MJD(std::vector<double> gregorian);
    static double               JD2MJD(double jd);
    static double               JD2Gregorian(const double jd);
    static std::vector<double>  JD2GregorianVect(double jd);    
    static double               MJD2JD(double mjd);
    static double               MJD2Gregorian(double mjd);
    static std::vector<double>  MJD2GregorianVect(double mjd);
    static double               UTC2UT1(double jd_utc, double dut1=0.0) ;
    static double               UTC2TT(double jd_utc, double dut1=0.0) ;
    static double               UTC2TDB(double jd_utc, double dut1=0.0) ;
    
    /***********************************************************
     * Some useful helper methods
     ***********************************************************/
    
    static double GetMJD2JDFactor();
    static double dut1(double date, CEDateType date_type=CEDateType::JD) ;
    double        dut1() ;
    static double dut1Error(double date, CEDateType date_type=CEDateType::JD) ;
    double        dut1Error() ;
    static double xpolar(double date, CEDateType date_type=CEDateType::JD) ;
    double        xpolar() ;
    static double ypolar(double date, CEDateType date_type=CEDateType::JD) ;
    double        ypolar() ;
    
    static double GregorianVect2Gregorian(std::vector<double> gregorian) ;
    static std::vector<double> Gregorian2GregorianVect(double gregorian) ;
    virtual double GetSecondsSinceMidnight(double utc_offset=0.0) ;
    virtual double GetTime(const double& utc_offset=0.0) const;
    virtual double GetTime_UTC() const;
    static double CurrentJD();
    void          SetReturnType(CEDateType return_type);
    
    /************************************************************
     * Overloaded operators
     ************************************************************/
    
    operator double();
    operator double() const;
    
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
    
};


/**********************************************************************//**
 * Get the Julian date represented by this object
 *************************************************************************/
inline 
double CEDate::JD(void) const
{
    return julian_date_;
}


/**********************************************************************//**
 * Get the Modified Julian date represented by this object
 *************************************************************************/
inline 
double CEDate::MJD(void) const
{
    return mod_julian_date_;
}


/**********************************************************************//**
 * Get the Gregorian calendar date formatted as a double
 *************************************************************************/
inline 
double CEDate::Gregorian(void) const
{
    return gregorian_date_;
}


/**********************************************************************//**
 * Get the Gregorian calendar date formatted as a vector
 *************************************************************************/
inline 
std::vector<double> CEDate::GregorianVect(void)
{
    return gregorian_date_vect_;
}


/**********************************************************************//**
 * Get the Gregorian calendar year
 *************************************************************************/
inline 
int CEDate::Year(void)
{
    return gregorian_date_vect_[0];
}


/**********************************************************************//**
 * Get the Gregorian calendar month
 *************************************************************************/
inline 
int CEDate::Month(void) 
{
    return gregorian_date_vect_[1];
}


/**********************************************************************//**
 * Get the Gregorian calendar day
 *************************************************************************/
inline 
int CEDate::Day(void) 
{
    return gregorian_date_vect_[2];
}


/**********************************************************************//**
 * Get the Gregorian calendar day fraction
 *************************************************************************/
inline 
double CEDate::DayFraction(void)
{
    return gregorian_date_vect_[3];
}


/**********************************************************************//**
 * Gets the stored SOFA Julian date to Mod Julian date factor 'DJM0'.
 * 
 * @return conversion factor for Julian date -> Modified Julian date conversions
 * Modified Julian date (mjd) can then be computed from this factor
 * and the Julian date (jd) by the following:
 *
 *     mjd = jd - CEDate::GetMJD2JDFactor()
 *************************************************************************/
inline 
double CEDate::GetMJD2JDFactor(void) 
{
    return DJM0;
}


/**********************************************************************//**
 * Set the return type from the overloaded 'operator double'.
 * @param return_type      ::CEDateType for the returned double
 *************************************************************************/
inline
void CEDate::SetReturnType(CEDateType return_type) 
{
    return_type_ = return_type;
}


#endif /* CEDate_h */

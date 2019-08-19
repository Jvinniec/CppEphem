/***************************************************************************
 *  CEDate.i: CppEphem                                                     *
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
 * @file CEDate.i
 * @brief SWIG file for CEDate class
 * @author JCardenzana
 */
%{
/* Put headers and other declarations here that are needed for compilation */
// CppEphem HEADERS
#include "CEDate.h"
%}

/**********************************************************************//**
 * Date enum
 *************************************************************************/
enum class CEDateType : unsigned int {
    JD=0,              ///< Julian Date
    MJD=1,             ///< Modified Julian Date
    GREGORIAN=2        ///< Gregorian calendar (year, month, day)
};

class CEDate {
public:
    // Default constructor
    CEDate(double date=CurrentJD(), CEDateType date_format=CEDateType::JD) ;
    explicit CEDate(std::vector<double> date) ;
    explicit CEDate(const CEDate& other) ;
    virtual ~CEDate();
    
    // Method that can be used to change the date that is stored in this object
    virtual void SetDate(const double&     date=CurrentJD(), 
                         const CEDateType& time_format=CEDateType::JD) ;
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
    static void                 UTC2UT1(const double& mjd,
                                        double*       ut11,
                                        double*       ut12) ;
    static void                 UTC2TT(const double& mjd,
                                       double*       tt1,
                                       double*       tt2) ;
    static void                 UTC2TDB(const double& mjd,
                                        double*       tdb1,
                                        double*       tdb2) ;
    
    /***********************************************************
     * Some useful helper methods
     ***********************************************************/
    
    static double GetMJD2JDFactor();
    static double dut1(const double&     date, 
                       const CEDateType& date_type=CEDateType::JD) ;
    double        dut1(void) const;
    // static double dut1Error(double date, CEDateType date_type=CEDateType::JD) ;
    // double        dut1Error() ;
    static double xpolar(const double&     date, 
                         const CEDateType& date_type=CEDateType::JD) ;
    double        xpolar(void) const;
    static double ypolar(const double&     date, 
                         const CEDateType& date_type=CEDateType::JD) ;
    double        ypolar(void) const;
    
    static double GregorianVect2Gregorian(std::vector<double> gregorian) ;
    static std::vector<double> Gregorian2GregorianVect(double gregorian) ;
    virtual double GetSecondsSinceMidnight(const double& utc_offset=0.0) ;
    virtual double GetTime(const double& utc_offset=0.0) const;
    virtual double GetTime_UTC() const;
    static double CurrentJD();
    void          SetReturnType(CEDateType return_type);

};

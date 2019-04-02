/***************************************************************************
 *  CECoordinates.h: CppEphem                                              *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016 JCardenzana                                           *
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

#ifndef CECoordinates_h
#define CECoordinates_h

#include <string>
#include <vector>

// CppEphem HEADERS
#include "CEDate.h"
#include "CENamespace.h"
//#include "CEObserver.h"
#include "CEException.h"

// SOFA HEADER
#include "sofa.h"

class CEObserver ;

/** The following enum specifies what coordinates this object represents */
enum class CECoordinateType {CIRS,           ///< RA, Dec (referenced at the center of the Earth)
                             ICRS,           ///< RA, Dec (referenced at the barycenter of the solarsystem)
                             GALACTIC,       ///< Galacitc longitude, latitude
                             OBSERVED        ///< Azimuth, Zenith (requires additional observer information)
                            };

// Initiate the class that holds the coordinate information
class CECoordinates {

    friend bool operator==(const CECoordinates& lhs, const CECoordinates& rhs);
    friend bool operator!=(const CECoordinates& lhs, const CECoordinates& rhs);

public:

    /****** CONSTRUCTORS ******/
    CECoordinates() ;
    CECoordinates(const double& xcoord, 
                  const double& ycoord,
                  const CECoordinateType& coord_type=CECoordinateType::ICRS,
                  const CEAngleType& angle_type=CEAngleType::RADIANS) ;
    CECoordinates(const CECoordinateType& coord_type) ;
    CECoordinates(const CECoordinates& other) ;
    virtual ~CECoordinates() ;
    
    /*********************************************************
     * Angular separation between two coordinate positions
     *********************************************************/
    virtual double AngularSeparation(const CECoordinates& coords,
                                     const CEAngleType& return_angle_type=CEAngleType::DEGREES) ;
    static double AngularSeparation(const CECoordinates& coords1, 
                                    const CECoordinates& coords2,
                                    const CEAngleType& return_angle_type=CEAngleType::DEGREES) ;
    static double AngularSeparation(double xcoord_first, 
                                    double ycoord_first,
                                    double xcoord_second, 
                                    double ycoord_second,
                                    const CEAngleType& angle_type=CEAngleType::DEGREES) ;
    
    /**********************************************************
     * Methods for accessing the coordinate information
     **********************************************************/
    
    virtual double XCoordinate_Rad(double jd=CppEphem::julian_date_J2000()) const;
    virtual double XCoordinate_Deg(double jd=CppEphem::julian_date_J2000()) const;
    virtual double YCoordinate_Rad(double jd=CppEphem::julian_date_J2000()) const;
    virtual double YCoordinate_Deg(double jd=CppEphem::julian_date_J2000()) const;
    
    // Convert an angle into Hours:minutes:seconds format
    static std::vector<double> GetHMS(double angle, CEAngleType angle_type = CEAngleType::DEGREES);
    static std::vector<double> GetDMS(double angle, CEAngleType angle_type = CEAngleType::DEGREES);

    // Return coordinate system    
    CECoordinateType GetCoordSystem(void) const;
    
    /**********************************************************
     * Methods for converting between coordinate types
     **********************************************************/
    // Note that whenever a date is required, the default will be set to the
    // start of the J2000 epoch (January 1, 2000 at 12:00 GMT). This corresponds
    // to the Julian Date of 2451545.0.
    
    // Convert from CIRS to other coordinates
    static void CIRS2ICRS(double input_ra, double input_dec, double *return_ra, double *return_dec,
                          CEDate date=CEDate(DJ00, CEDateType::JD),
                          CEAngleType angle_type=CEAngleType::RADIANS);
    static void CIRS2Galactic(double ra, double dec, double *glon, double *glat,
                              CEDate date=CEDate(DJ00, CEDateType::JD),
                              CEAngleType angle_type=CEAngleType::RADIANS);
    static int CIRS2Observed(double ra,
                             double dec,
                             double *az,
                             double *zen,
                             CEDate&     date,
                             CEObserver& observer,
                             CEAngleType angle_type=CEAngleType::RADIANS,
                             double wavelength_um=0.5,
                             double *observed_ra=nullptr,
                             double *observed_dec=nullptr,
                             double *hour_angle=nullptr);
    
    // Convert from ICRS to other coordinates
    static void ICRS2CIRS(double input_ra,
                          double input_dec,
                          double *return_ra,
                          double *return_dec,
                          CEDate date=CEDate(),
                          CEAngleType angle_type=CEAngleType::RADIANS);
    static void ICRS2Galactic(double ra,
                              double dec,
                              double *glon,
                              double *glat,
                              CEAngleType angle_type=CEAngleType::RADIANS);
    static int ICRS2Observed(double ra,
                             double dec,
                             double *az,
                             double *zen,
                             CEDate&     date,
                             CEObserver& observer,
                             CEAngleType angle_type=CEAngleType::RADIANS,
                             double wavelength_um=0.5,
                             double *observed_ra=nullptr,
                             double *observed_dec=nullptr,
                             double *hour_angle=nullptr);
    
    // Convert from GALACTIC to other coordinates
    static void Galactic2CIRS(double glon, double glat, double *ra, double *dec,
                              CEDate date=CEDate(),
                              CEAngleType angle_type=CEAngleType::RADIANS);
    static void Galactic2ICRS(double glon, double glat, double *ra, double *dec,
                              CEAngleType angle_type=CEAngleType::RADIANS);
    static int Galactic2Observed(double glon,
                                 double glat,
                                 double *az,
                                 double *zen,
                                 CEDate&     date,
                                 CEObserver& observer,
                                 CEAngleType angle_type=CEAngleType::RADIANS,
                                 double wavelength_um=0.5,
                                 double *observed_glon=nullptr,
                                 double *observed_glat=nullptr,
                                 double *hour_angle=nullptr);
    // Convert from OBSERVED to other coordinates
    static int Observed2CIRS(double az,
                              double zen,
                              double *ra,
                              double *dec,
                              CEDate&     date,
                              CEObserver& observer,
                              CEAngleType angle_type=CEAngleType::RADIANS);
    static int Observed2ICRS(double az,
                              double zen,
                              double *ra,
                              double *dec,
                              CEDate&     date,
                              CEObserver& observer,
                              CEAngleType angle_type=CEAngleType::RADIANS);
    static int Observed2Galactic(double az,
                              double zen,
                              double *glon,
                              double *glat,
                              CEDate&     date,
                              CEObserver& observer,
                              CEAngleType angle_type=CEAngleType::RADIANS);
    
    
    // The following are provided to allow converting to OBSERVED
    // azimuth, zenith without the need to use the CEObserver class.
    // These are the methods that are ultimately called by the above
    // methods that do use a CEObserver object
    
    /* Raw methods for converting CIRS <-> Observed (observer specific) coordinates. */
    static int CIRS2Observed(double ra,
                             double dec,
                             double *az,
                             double *zen,
                             double julian_date,
                             double longitude,
                             double latitude,
                             double elevation_m=0.0,
                             double pressure_hPa=-1.0,
                             double temperature_celsius=-1000,
                             double relative_humidity=0.0,
                             double dut1=0.0,
                             double xp=0.0,
                             double yp=0.0,
                             double wavelength_um=0.5,
                             double *observed_ra=nullptr,
                             double *observed_dec=nullptr,
                             double *hour_angle=nullptr);
    static int Observed2CIRS(double az,
                             double zen,
                             double *ra,
                             double *dec,
                             double julian_date,
                             double longitude,
                             double latitude,
                             double elevation_m=0.0,
                             double pressure_hPa=-1.0,
                             double temperature_celsius=-1000,
                             double relative_humidity=0.0,
                             double dut1=0.0,
                             double xp=0.0,
                             double yp=0.0,
                             double wavelength_um=0.5);
    
    /* Raw methods for converting ICRS <-> Observed (observer specific) coordinates. */
    static int ICRS2Observed(double ra,
                             double dec,
                             double *az,
                             double *zen,
                             double julian_date,
                             double longitude,
                             double latitude,
                             double elevation_m=0.0,
                             double pressure_hPa=-1.0,
                             double temperature_celsius=-1000,
                             double relative_humidity=0.0,
                             double dut1=0.0,
                             double xp=0.0, double yp=0.0,
                             double wavelength_um=0.5,
                             double *observed_ra=nullptr,
                             double *observed_dec=nullptr,
                             double *hour_angle=nullptr);
    static int Observed2ICRS(double az,
                             double zen,
                             double *ra,
                             double *dec,
                             double julian_date,
                             double longitude,
                             double latitude,
                             double elevation_m=0.0,
                             double pressure_hPa=-1.0,
                             double temperature_celsius=-1000,
                             double relative_humidity=0.0,
                             double dut1=0.0,
                             double xp=0.0, double yp=0.0,
                             double wavelength_um=0.5);
    
    /* Raw methods for converting Galactic <-> Observed (observer specific) coordinates. */
    static int Galactic2Observed(double glon,
                             double glat,
                             double *az,
                             double *zen,
                             double julian_date,
                             double longitude,
                             double latitude,
                             double elevation_m=0.0,
                             double pressure_hPa=-1.0,
                             double temperature_celsius=-1000,
                             double relative_humidity=0.0,
                             double dut1=0.0,
                             double xp=0.0, double yp=0.0,
                             double wavelength_um=0.50,
                             double *observed_glon=nullptr,
                             double *observed_glat=nullptr,
                             double *hour_angle=nullptr);
    static int Observed2Galactic(double az,
                             double zen,
                             double *glon,
                             double *glat,
                             double julian_date,
                             double longitude,
                             double latitude,
                             double elevation_m=0.0,
                             double pressure_hPa=-1.0,
                             double temperature_celsius=-1000,
                             double relative_humidity=0.0,
                             double dut1=0.0,
                             double xp=0.0, double yp=0.0,
                             double wavelength_um=0.50);
    
    virtual CECoordinates GetObservedCoords(const double& julian_date,
                            const double& longitude,
                            const double& latitude,
                            const double& elevation_m=0.0,
                            const double& pressure_hPa=-1.0,
                            const double& temperature_celsius=-1000,
                            const double& relative_humidity=0.0,
                            const double& dut1=0.0,
                            const double& xp=0.0, const double& yp=0.0,
                            const double& wavelength_um=0.5);
    virtual CECoordinates GetObservedCoords(const CEDate& date,
                            const CEObserver& observer,
                            const double& dut1=0.0,
                            const double& xp=0.0, const double& yp=0.0,
                            const double& wavelength_um=0.5);

    /*********************************************************
     * More generic methods for converting between coordinate types
     *********************************************************/
    CECoordinates ConvertTo(CECoordinateType output_coord_type,
                            const CEObserver& observer,
                            const CEDate&     date=CEDate::CurrentJD());
    
    CECoordinates ConvertTo(CECoordinateType output_coord_type,
                            double jd=CEDate::CurrentJD(),
                            double longitude=0.0,
                            double latitude=0.0,
                            double elevation_m=0.0,
                            double pressure_hPa=-1.0,
                            double temperature_celsius=-1000,
                            double relative_humidity=0.0,
                            double dut1=0.0,

                            double xp=0.0, double yp=0.0,
                            double wavelength_um=0.5);
    
    CECoordinates ConvertToCIRS(double jd=CEDate::CurrentJD(),
                                double longitude=0.0,
                                double latitude=0.0,
                                double elevation_m=0.0,
                                double pressure_hPa=-1.0,
                                double temperature_celsius=-1000,
                                double relative_humidity=0.0,
                                double dut1=0.0,
                                double xp=0.0, double yp=0.0,
                                double wavelength_um=0.5);
    CECoordinates ConvertToICRS(double jd=CEDate::CurrentJD(),
                                double longitude=0.0,
                                double latitude=0.0,
                                double elevation_m=0.0,
                                double pressure_hPa=-1.0,
                                double temperature_celsius=-1000,
                                double relative_humidity=0.0,
                                double dut1=0.0,
                                double xp=0.0, double yp=0.0,
                                double wavelength_um=0.5);
    CECoordinates ConvertToGalactic(double jd=CEDate::CurrentJD(),
                                    double longitude=0.0,
                                    double latitude=0.0,
                                    double elevation_m=0.0,
                                    double pressure_hPa=-1.0,
                                    double temperature_celsius=-1000,
                                    double relative_humidity=0.0,
                                    double dut1=0.0,
                                    double xp=0.0, double yp=0.0,
                                    double wavelength_um=0.5);
    CECoordinates ConvertToObserved(double jd=CEDate::CurrentJD(),
                                    double longitude=0.0,
                                    double latitude=0.0,
                                    double elevation_m=0.0,
                                    double pressure_hPa=-1.0,
                                    double temperature_celsius=-1000,
                                    double relative_humidity=0.0,
                                    double dut1=0.0,
                                    double xp=0.0, double yp=0.0,
                                    double wavelength_um=0.5);
    
    /*********************************************************
     * Methods for setting the coordinates of this object
     *********************************************************/
    virtual void SetCoordinates(double xcoord, double ycoord,
                                CECoordinateType coord_type = CECoordinateType::ICRS,
                                CEAngleType angle_type = CEAngleType::RADIANS);
    virtual void SetCoordinates(CECoordinates& coords);
    
protected:
    // Coordinate variables
    mutable double xcoord_;         //<! X coordinate (radians)
    mutable double ycoord_;         //<! Y coordinate (radians)
    CECoordinateType coord_type_;   //<! Coordinate system to which 'xcoord_' and 'ycoord_' belong.
                                    //<! Possible values are "CIRS", "ICRS", "GALACTIC", "OBSERVED", and "GEOGRAPHIC"
    
private:
    
};


/**********************************************************************//**
 * Return x coordinate at given Julian date in radius
 * 
 * @param[in] jd   Julian date (used only by derived classes)
 * @return X-coordinate in radians
 *************************************************************************/
inline
double CECoordinates::XCoordinate_Rad(double jd) const
{
    return xcoord_;
}


/**********************************************************************//**
 * Returns x coordinate at given julian date in degrees
 * 
 * @param[in] jd   Julian date (used only by derived classes)
 * @return X-coordinate in degrees
 *************************************************************************/
inline
double CECoordinates::XCoordinate_Deg(double jd) const
{
    return XCoordinate_Rad(jd) * DR2D;
}


/**********************************************************************//**
 * Returns y coordinate at given Julian date in radius
 * 
 * @param[in] jd   Julian date (used only by derived classes)
 * @return Y-coordinate in radians
*************************************************************************/
inline
double CECoordinates::YCoordinate_Rad(double jd) const
{
    return ycoord_;
}
    
/**********************************************************************//**
 * Returns y coordinate at given Julian date in degrees
 * 
 * @param[in] jd   Julian date (used only by derived classes)
 * @return Y-coordinate in degrees
 *************************************************************************/
inline
double CECoordinates::YCoordinate_Deg(double jd) const
{
    return YCoordinate_Rad(jd) * DR2D;
}


/**********************************************************************//**
 * Return coordinate system
 * @return Coordinate type of this object
 *************************************************************************/
inline
CECoordinateType CECoordinates::GetCoordSystem(void) const 
{
    return coord_type_;
}

#endif /* CECoordinates_h */

//
//  CECoordinates.h
//  CppEphem
//
//  Created by Josh Cardenzana on 3/26/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#ifndef CECoordinates_h
#define CECoordinates_h

#include <string>
#include <vector>

// CppEphem HEADERS
#include "CEDate.h"
#include "CENamespace.h"
//#include "CEObserver.h"

// SOFA HEADER
#include "sofa.h"

using namespace CppEphem ;

class CEObserver ;

/** The following enum specifies what coordinates this object represents */
enum class CECoordinateType {CIRS,           ///< RA, Dec (referenced at the center of the Earth)
                            ICRS,            ///< RA, Dec (referenced at the barycenter of the solarsystem)
                            GALACTIC,        ///< Galacitc longitude, latitude
                            OBSERVED         ///< Azimuth, Zenith (requires additional observer information)
                            };

// Initiate the class that holds the coordinate information
class CECoordinates {
public:
    CECoordinates() ;
    CECoordinates(double xcoord, double ycoord,
                  CECoordinateType coord_type,
                  CEAngleType angle_type=CEAngleType::RADIANS) ;
    CECoordinates(CECoordinateType coord_type) ;
    CECoordinates(const CECoordinates& other) ;
    virtual ~CECoordinates() ;
    
    /**********************************************************
     * Methods for accessing the coordinate information
     **********************************************************/
    virtual double XCoordinate_Deg() {return xcoord_ * DR2D ;}      ///< Returns x-coordinate in degrees
    virtual double YCoordinate_Deg() {return ycoord_ * DR2D ;}      ///< Returns y-coordinate in degrees
    virtual double XCoordinate_Rad() {return xcoord_ ;}             ///< Returns x-coordinate in radians
    virtual double YCoordinate_Rad() {return ycoord_ ;}             ///< Returns y-coordinate in radians
    CECoordinateType GetCoordSystem() {return coord_type_;}         ///< Returns the coordinate type of this object
    
    /**********************************************************
     * Methods for converting between coordinate types
     **********************************************************/
    // Note that whenever a date is required, the default will be set to the
    // start of the J2000 epoch (January 1, 2000 at 12:00 GMT). This corresponds
    // to the Julian Date of 2451545.0.
    
    static void ConvertCoordinates(CECoordinateType input_coord_type,
                                   double input_coord_x,
                                   double input_coord_y,
                                   CECoordinateType return_coord_type,
                                   double *return_coord_x,
                                   double *return_coord_y,
                                   std::vector<double> additional_params=std::vector<double>(0));
    
    // Convert from CIRS to other coordinates
    static void CIRS2ICRS(double input_ra, double input_dec, double *return_ra, double *return_dec,
                          CEDate date=CEDate(DJ00, CEDateType::JD),
                          CEAngleType angle_type=CEAngleType::RADIANS) ;
    static void CIRS2Galactic(double ra, double dec, double *glon, double *glat,
                              CEDate date=CEDate(DJ00, CEDateType::JD),
                              CEAngleType angle_type=CEAngleType::RADIANS) ;
    static int CIRS2Observed(double ra,
                             double dec,
                             double *az,
                             double *zen,
                             CEObserver& observer,
                             CEAngleType angle_type=CEAngleType::RADIANS,
                             double wavelength=0.5,
                             double *observed_ra=nullptr,
                             double *observed_dec=nullptr,
                             double *hour_angle=nullptr) ;
    
    // Convert from ICRS to other coordinates
    static void ICRS2CIRS(double input_ra,
                          double input_dec,
                          double *return_ra,
                          double *return_dec,
                          CEDate date=CEDate(DJ00, CEDateType::JD),
                          CEAngleType angle_type=CEAngleType::RADIANS);
    static void ICRS2Galactic(double ra,
                              double dec,
                              double *glon,
                              double *glat,
                              CEAngleType angle_type=CEAngleType::RADIANS) ;
    static int ICRS2Observed(double ra,
                             double dec,
                             double *az,
                             double *zen,
                             CEObserver& observer,
                             CEAngleType angle_type=CEAngleType::RADIANS,
                             double wavelength=0.5,
                             double *observed_ra=nullptr,
                             double *observed_dec=nullptr);
    
    // Convert from GALACTIC to other coordinates
    static void Galactic2CIRS(double glon, double glat, double *ra, double *dec,
                              CEDate date=CEDate(julian_date_J2000(), CEDateType::JD),
                              CEAngleType angle_type=CEAngleType::RADIANS) ;
    static void Galactic2ICRS(double glon, double glat, double *ra, double *dec,
                              CEAngleType angle_type=CEAngleType::RADIANS) ;
    static int Galactic2Observed(double glon,
                                 double glat,
                                 double *az,
                                 double *zen,
                                 CEObserver& observer,
                                 CEAngleType angle_type=CEAngleType::RADIANS,
                                 double wavelength=0.5,
                                 double *observed_glon=nullptr,
                                 double *observed_glat=nullptr);
    // Convert from OBSERVED to other coordinates
    
    
    // The following are provided to allow converting to OBSERVED
    // azimuth, zenith without the need to use the CEObserver class.
    // They function by creating a temporary CEObserver object and then
    // calling the above functions.
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
                             double wavelength=500.0,
                             double *observed_ra=nullptr,
                             double *observed_dec=nullptr,
                             double *hour_angle=nullptr
                             );
    /** Raw method for converting ICRS -> Observed (observer specific) coordinates. */
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
                             double wavelength=500.0,
                             double *observed_ra=nullptr,
                             double *observed_dec=nullptr) ;
    /** Raw method for converting Galactic -> Observed (observer specific) coordinates. */
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
                             double wavelength=500.0,
                             double *observed_glon=nullptr,
                             double *observed_glat=nullptr) ;
    
    virtual CECoordinates GetObservedCoords(double julian_date,
                            double longitude,
                            double latitude,
                            double elevation_m=0.0,
                            double pressure_hPa=-1.0,
                            double temperature_celsius=-1000,
                            double relative_humidity=0.0,
                            double dut1=0.0,
                            double xp=0.0, double yp=0.0,
                            double wavelength=500.0) ;
    virtual CECoordinates GetObservedCoords(double julian_date,
                            CEObserver& observer,
                            double dut1=0.0,
                            double xp=0.0, double yp=0.0,
                            double wavelength=500.0) ;
    
protected:
    // Coordinate variables
    double xcoord_ ;                ///< X coordinate (radians)
    double ycoord_ ;                ///< Y coordinate (radians)
    CECoordinateType coord_type_ ;  ///< Coordinate system to which 'xcoord_' and 'ycoord_' belong.
                                    ///< Possible values are "CIRS", "ICRS", "GALACTIC", "OBSERVED", and "GEOGRAPHIC"
    
private:
    
};

#endif /* CECoordinates_h */

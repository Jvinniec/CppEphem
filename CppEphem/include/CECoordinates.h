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
#include "CEObserver.h"

// SOFA HEADER
#include "sofa.h"

using namespace CppEphem ;

// The following enum specifies what the
enum CECoordinateType {CIRS,            // RA, Dec (referenced at the center of the Earth)
                       ICRS,            // RA, Dec (referenced at the barycenter of the solarsystem)
                       GALACTIC,        // Galacitc longitude, latitude
                       OBSERVED};       // Azimuth, Zenith (requires additional observer information)

// Initiate the class that holds the coordinate information
class CECoordinates {
public:
    // Default constructor
    CECoordinates() ;
    // Primary constructor (NOTE: xcoord & ycoord are expected to be in radians by default
    CECoordinates(double xcoord, double ycoord,
                  CECoordinateType coord_type,
                  CEAngleType angle_type=CEAngleType::RADIANS) ;
    // Set the object based only on the coordinate type. The default wil be to store
    CECoordinates(CECoordinateType coord_type) ;
    // Copy onstructor
    CECoordinates(const CECoordinates& other) ;
    // Destructor
    virtual ~CECoordinates() {} ;
    
    /**********************************************************
     * Methods for accessing the coordinate information
     **********************************************************/
    double XCoordinate_Deg() {return xcoord_ * DR2D ;}
    double YCoordinate_Deg() {return ycoord_ * DR2D ;}
    double XCoordinate_Rad() {return xcoord_ ;}
    double YCoordinate_Rad() {return ycoord_ ;}
    CECoordinateType GetCoordSystem() {return coord_type_;}
    
    /**********************************************************
     * Methods for converting between coordinate types
     **********************************************************/
    // Note that whenever a date is required, the default will be set to the
    // start of the J2000 epoch (January 1, 2000 at 12:00 GMT). This corresponds
    // to the Julian Date of 2451545.0.
    
    // There should be one main method that can be used to convert from
    // one set of coordinates to another.
    static void ConvertCoordinates(CECoordinateType input_coord_type,
                                   double input_coord_x, double input_coord_y,
                                   CECoordinateType return_coord_type,
                                   double *return_coord_x, double *return_coord2_y,
                                   std::vector<double> additional_params=std::vector<double>(0)) ;
    
    // Convert from CIRS to other coordinates
    static void CIRS2ICRS(double input_ra, double input_dec, double *return_ra, double *return_dec,
                          CEDate date=CEDate(DJ00, CEDateType::JD)) ;
    static void CIRS2Galactic(double ra, double dec, double *glon, double *glat,
                          CEDate date=CEDate(DJ00, CEDateType::JD)) ;
    static int CIRS2Observed(double ra, double dec,             // RA, Dec in CIRS coordinates
                             double *az, double *zen,           // Azimuth, zenith angle
                             CEObserver observer,
                             CEAngleType angle_type=CEAngleType::RADIANS,   // Angle type for all angles provided
                                                                // (either RADIANS or DEGREES)
                             double xp=0.0, double yp=0.0,      // Polar motion, can be found in IERS bulletins,
                                                                // but 0 is probably fine for both
                             double wavelength=0.5,             // Observing wavelength (micrometers)
                             double *observed_ra=nullptr,       // Observed CIRS right ascension
                             double *observed_dec=nullptr,      // Observed CIRS declination
                             double *hour_angle=nullptr) ;      // Hour angle for coordinates
    
    // Convert from ICRS to other coordinates
    static void ICRS2CIRS(double input_ra, double input_dec, double *return_ra, double *return_dec,
                          CEDate date=CEDate(DJ00, CEDateType::JD)) ;
    static void ICRS2Galactic(double ra, double dec, double *glon, double *glat) ;
    static int ICRS2Observed(double ra, double dec,             // RA, Dec in CIRS coordinates
                             double *az, double *zen,           // Azimuth, zenith angle
                             CEObserver observer,
                             CEAngleType angle_type=CEAngleType::RADIANS,   // Angle type for all angles provided
                             // (either RADIANS or DEGREES)
                             double xp=0.0, double yp=0.0,      // Polar motion, can be found in IERS bulletins,
                             // but 0 is probably fine for both
                             double wavelength=0.5,             // Observing wavelength (micrometers)
                             double *observed_ra=nullptr,       // Observed CIRS right ascension
                             double *observed_dec=nullptr,      // Observed CIRS declination
                             double *hour_angle=nullptr) ;      // Hour angle for coordinates
    
    // Convert from GALACTIC to other coordinates
    static void Galactic2CIRS() ;
    static void Galacitc2ICRS() ;
    static int Galacitc2Observed() ;
    
    // Convert from OBSERVED to other coordinates
    
    
    // The following are provided to allow converting to OBSERVED
    // azimuth, zenith without the need to use the CEObserver class.
    // They function by creating a temporary CEObserver object and then
    // calling the above functions.
    static int CIRS2Observed(double ra, double dec,
                             double *az, double *zen,
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
                             double *observed_dec=nullptr,
                             double *hour_angle=nullptr) ;
    static int ICRS2Observed(double ra, double dec,
                             double *az, double *zen,
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
                             double *observed_dec=nullptr,
                             double *hour_angle=nullptr) ;
    static int Galactic2Observed(double glon, double glat,
                             double *az, double *zen,
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
protected:
    // Coordinate variables
    double xcoord_ ;                // X coordinate (radians)
    double ycoord_ ;                // Y coordinate (radians)
    CECoordinateType coord_type_ ;  // Coordinate system to which 'xcoord_' and 'ycoord_' belong
                                    // Possible values are "CIRS", "ICRS", "GALACTIC", "OBSERVED", and "GEOGRAPHIC"
    
private:
    
};

#endif /* CECoordinates_h */

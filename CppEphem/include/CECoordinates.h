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

/** The following enum specifies what coordinates this object represents */
enum class CECoordinateType {CIRS,           ///< RA, Dec (referenced at the center of the Earth)
                            ICRS,            ///< RA, Dec (referenced at the barycenter of the solarsystem)
                            GALACTIC,        ///< Galacitc longitude, latitude
                            OBSERVED         ///< Azimuth, Zenith (requires additional observer information)
                            };

// Initiate the class that holds the coordinate information
class CECoordinates {
public:
    /** Default constructor */
    CECoordinates() ;
    /** Primary constructor (NOTE: xcoord & ycoord are expected to be in radians by default. */
    CECoordinates(double xcoord, double ycoord,
                  CECoordinateType coord_type,
                  CEAngleType angle_type=CEAngleType::RADIANS) ;
    /** Set the object based only on the coordinate type. The default wil be to store. */
    CECoordinates(CECoordinateType coord_type) ;
    /** Copy onstructor */
    CECoordinates(const CECoordinates& other) ;
    /** Destructor */
    virtual ~CECoordinates() {} ;
    
    /**********************************************************
     * Methods for accessing the coordinate information
     **********************************************************/
    double XCoordinate_Deg() {return xcoord_ * DR2D ;}      ///< Returns x-coordinate in degrees
    double YCoordinate_Deg() {return ycoord_ * DR2D ;}      ///< Returns y-coordinate in degrees
    double XCoordinate_Rad() {return xcoord_ ;}             ///< Returns x-coordinate in radians
    double YCoordinate_Rad() {return ycoord_ ;}             ///< Returns y-coordinate in radians
    CECoordinateType GetCoordSystem() {return coord_type_;} ///< Returns the coordinate type of this object
    
    /**********************************************************
     * Methods for converting between coordinate types
     **********************************************************/
    // Note that whenever a date is required, the default will be set to the
    // start of the J2000 epoch (January 1, 2000 at 12:00 GMT). This corresponds
    // to the Julian Date of 2451545.0.
    
    /** Main method used to convert from one set of coordinates to another. */
    static void ConvertCoordinates(CECoordinateType input_coord_type,   ///< Coordinate type to convert FROM
                                   double input_coord_x,                ///< X-coordinate for input
                                   double input_coord_y,                ///< Y-coordinate for input
                                   CECoordinateType return_coord_type,  ///< Coordinate type to convert TO
                                   double *return_coord_x,              ///< X-coordinate for output
                                   double *return_coord_y,              ///< Y-coordinate for output
                                   std::vector<double> additional_params=std::vector<double>(0) ///< Any additional parameters needed
                                   );
    
    // Convert from CIRS to other coordinates
    /** CIRS -> ICRS coordinate conversion */
    static void CIRS2ICRS(double input_ra, double input_dec, double *return_ra, double *return_dec,
                          CEDate date=CEDate(DJ00, CEDateType::JD),
                          CEAngleType angle_type=CEAngleType::RADIANS) ;
    /** CIRS -> Galactic coordinate conversion */
    static void CIRS2Galactic(double ra, double dec, double *glon, double *glat,
                              CEDate date=CEDate(DJ00, CEDateType::JD),
                              CEAngleType angle_type=CEAngleType::RADIANS) ;
    /** CIRS -> Observed (or observer specific) coordinate conversion */
    static int CIRS2Observed(double ra,                         ///< Right ascension in CIRS coordinates
                             double dec,                        ///< Declination in CIRS coordinates
                             double *az,                        ///< Azimuth (returned)
                             double *zen,                       ///< Zenith angle (returned)
                             CEObserver observer,               ///< Observer information
                             CEAngleType angle_type=CEAngleType::RADIANS,   ///< Angle type for all angles provided
                                                                ///< (either RADIANS or DEGREES)
                             double wavelength=0.5,             ///< Observing wavelength (micrometers)
                             double *observed_ra=nullptr,       ///< Observed CIRS right ascension
                             double *observed_dec=nullptr,      ///< Observed CIRS declination
                             double *hour_angle=nullptr         ///< Hour angle for coordinates
                             ) ;
    
    // Convert from ICRS to other coordinates
    /** ICRS -> CIRS coordinate conversion */
    static void ICRS2CIRS(double input_ra,                      ///< Right ascension in ICRS coordinates
                          double input_dec,                     ///< Declination in ICRS coordinates
                          double *return_ra,                    ///< Right ascension in CIRS coordinates (returned)
                          double *return_dec,                   ///< Declination in CIRS coordinates (returned)
                          CEDate date=CEDate(DJ00, CEDateType::JD),     ///< Date information
                          CEAngleType angle_type=CEAngleType::RADIANS   ///< Angle type for all angles
                                                                        ///< (either RADIANS or DEGREES)
                          );
    /** ICRS -> Galactic coordinate conversion */
    static void ICRS2Galactic(double ra,                        ///< Right ascension in ICRS coordinates
                              double dec,                       ///< Declination in ICRS coordinates
                              double *glon,                     ///< Galactic longitude (returned)
                              double *glat,                     ///< Galactic latitude (returned)
                              CEAngleType angle_type=CEAngleType::RADIANS) ;
    /** ICRS -> Observed (observer specific) coordinates */
    static int ICRS2Observed(double ra,                         ///< Right ascension in ICRS coordinates
                             double dec,                        ///< Declination in ICRS coordinates
                             double *az,                        ///< Azimuth angle
                             double *zen,                       ///< Zenith angle
                             CEObserver observer,               ///< Object describing the observer
                             CEAngleType angle_type=CEAngleType::RADIANS,   ///< Angle type for all angles provided
                                                                ///< (either RADIANS or DEGREES)
                             double wavelength=0.5,             ///< Observing wavelength (micrometers)
                             double *observed_ra=nullptr,       ///< Observed ICRS right ascension (returned)
                             double *observed_dec=nullptr       ///< Observed ICRS declination (returned)
                             );
    
    // Convert from GALACTIC to other coordinates
    /** Galactic -> ICRS coordinate conversion */
    static void Galactic2CIRS(double glon, double glat, double *ra, double *dec,
                              CEDate date=CEDate(julian_date_J2000(), CEDateType::JD),
                              CEAngleType angle_type=CEAngleType::RADIANS) ;
    /** Galactic -> ICRS coordinate conversion */
    static void Galactic2ICRS(double glon, double glat, double *ra, double *dec,
                              CEAngleType angle_type=CEAngleType::RADIANS) ;
    /** Galactic -> Observed (observer specific) coordinate conversion */
    static int Galactic2Observed(double glon,                       ///< Galactic longitude
                                 double glat,                       ///< Galactic latitude
                                 double *az,                        ///< Azimuth
                                 double *zen,                       ///< Zenith angle
                                 CEObserver observer,               ///< Object describing the observer
                                 CEAngleType angle_type=CEAngleType::RADIANS,   ///< Angle type for all angles provided
                                                                    ///< (either RADIANS or DEGREES)
                                 double wavelength=0.5,             ///< Observing wavelength (micrometers)
                                 double *observed_glon=nullptr,     ///< Observed Galactic longitude (returned)
                                 double *observed_glat=nullptr    ///< Observed Galactic latitude (returned)
                                 );
    // Convert from OBSERVED to other coordinates
    
    
    // The following are provided to allow converting to OBSERVED
    // azimuth, zenith without the need to use the CEObserver class.
    // They function by creating a temporary CEObserver object and then
    // calling the above functions.
    /** 
     * Raw method for converting CIRS -> Observed (observer specific) coordinates.
     * Note: All angles are expected to be in radians.
     */
    static int CIRS2Observed(double ra,                         ///< Right ascension in CIRS coordinates
                             double dec,                        ///< Declination CIRS coordinates
                             double *az,                        ///< Azimuth (returned)
                             double *zen,                       ///< Zenith (returned)
                             double julian_date,                ///< Julian date of observation
                             double longitude,                  ///< Observer longitude (radians)
                             double latitude,                   ///< Observer latitude (radians)
                             double elevation_m=0.0,            ///< Observer elevation
                             double pressure_hPa=-1.0,          ///< Observer atmospheric pressure (units hPa)
                             double temperature_celsius=-1000,  ///< Observer atmospheric temperature (units Celsius)
                             double relative_humidity=0.0,      ///< Observer atmospheric humidity (0.0-1.0)
                             double dut1=0.0,
                             double xp=0.0,
                             double yp=0.0,
                             double wavelength=500.0,
                             double *observed_ra=nullptr,
                             double *observed_dec=nullptr,
                             double *hour_angle=nullptr
                             );
    /** Raw method for converting ICRS -> Observed (observer specific) coordinates. */
    static int ICRS2Observed(double ra,                         ///< Right ascension in CIRS coordinates
                             double dec,                        ///< Declination CIRS coordinates
                             double *az,                        ///< Azimuth (returned)
                             double *zen,                       ///< Zenith (returned)
                             double julian_date,                ///< Julian date of observation
                             double longitude,                  ///< Observer longitude (radians)
                             double latitude,                   ///< Observer latitude (radians)
                             double elevation_m=0.0,            ///< Observer elevation
                             double pressure_hPa=-1.0,          ///< Observer atmospheric pressure (units hPa)
                             double temperature_celsius=-1000,  ///< Observer atmospheric temperature (units Celsius)
                             double relative_humidity=0.0,      ///< Observer atmospheric humidity (0.0-1.0)
                             double dut1=0.0,
                             double xp=0.0, double yp=0.0,
                             double wavelength=500.0,
                             double *observed_ra=nullptr,
                             double *observed_dec=nullptr) ;
    /** Raw method for converting Galactic -> Observed (observer specific) coordinates. */
    static int Galactic2Observed(double glon,                   ///< Galactic longitude (radians)
                             double glat,                       ///< Galactic latitude (radians)
                             double *az,                        ///< Azimuth (returned)
                             double *zen,                       ///< Zenith (returned)
                             double julian_date,                ///< Julian date of observation
                             double longitude,                  ///< Observer longitude (radians)
                             double latitude,                   ///< Observer latitude (radians)
                             double elevation_m=0.0,            ///< Observer elevation
                             double pressure_hPa=-1.0,          ///< Observer atmospheric pressure (units hPa)
                             double temperature_celsius=-1000,  ///< Observer atmospheric temperature (units Celsius)
                             double relative_humidity=0.0,      ///< Observer atmospheric humidity (0.0-1.0)
                             double dut1=0.0,
                             double xp=0.0, double yp=0.0,
                             double wavelength=500.0,
                             double *observed_glon=nullptr,
                             double *observed_glat=nullptr) ;
protected:
    // Coordinate variables
    double xcoord_ ;                ///< X coordinate (radians)
    double ycoord_ ;                ///< Y coordinate (radians)
    CECoordinateType coord_type_ ;  ///< Coordinate system to which 'xcoord_' and 'ycoord_' belong.
                                    ///< Possible values are "CIRS", "ICRS", "GALACTIC", "OBSERVED", and "GEOGRAPHIC"
    
private:
    
};

#endif /* CECoordinates_h */

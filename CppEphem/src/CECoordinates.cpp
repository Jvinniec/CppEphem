//
//  CECoordinates.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 3/26/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

/** \class CECoordinates
 CECoordinates class is responsible for doing all of the celestial coordinate
 conversions. Command line tools are also available for "on the fly" conversions.
 
 The different coordinate systems used:
 - CIRS: Earth-centric coordinate systems
 - ICRS: Solar system barycentric coordinate system
 - Galactic: Galactic coordinates
 - Observed: Observer specific coordinate system
 */

#include "CECoordinates.h"
#include "CEObserver.h"

#pragma mark - Constructors & Destructors

////////////////////////////////////////////////////////////
/// Default constructor
CECoordinates::CECoordinates() :
    xcoord_(0.0),
    ycoord_(0.0),
    coord_type_(CECoordinateType::CIRS)
{}

////////////////////////////////////////////////////////////
/// Primary constructor (NOTE: xcoord & ycoord are expected to be in radians by default.
/// @param xcoord X-Coordinate (radians)
/// @param ycoord Y-Coordinate (radians)
/// @param coord_type Coordinate type (see CECoordinateType)
/// @param angle_type Angle type (either DEGREES or RADIANS)

CECoordinates::CECoordinates(double xcoord, double ycoord,
                             CECoordinateType coord_type,
                             CEAngleType angle_type) :
    xcoord_(xcoord),
    ycoord_(ycoord),
    coord_type_(coord_type)
{
    // If we've been given the coordinates in degrees, convert to radians
    if (angle_type == CEAngleType::DEGREES) {
        xcoord_ *= DD2R ;
        ycoord_ *= DD2R ;
    }
}

////////////////////////////////////////////////////////////
/// Constructor from a coordinate type
/// @param coord_type Coordinate type (see CECoordinateType)

CECoordinates::CECoordinates(CECoordinateType coord_type) :
    xcoord_(0.0),
    ycoord_(0.0),
    coord_type_(coord_type)
{}

////////////////////////////////////////////////////////////
/// Copy constructor

CECoordinates::CECoordinates(const CECoordinates& other) :
    xcoord_(other.xcoord_),
    ycoord_(other.ycoord_),
    coord_type_(other.coord_type_)
{}

////////////////////////////////////////////////////////////
/// Destructor

CECoordinates::~CECoordinates()
{}
#pragma mark - Public Methods


#pragma mark - Conversion routines

////////////////////////////////////////////////////////////
/// Main method used to convert from one set of coordinates to another.
/// @param input_coord_type Input coordinate type
/// @param input_coord_x Input x-coordinate
/// @param input_coord_y Input y-coordinate
/// @param return_coord_type Returned coordinate type
/// @param return_coord_x Returned x-coordinate
/// @param return_coord_y Returned y-coordinate
/// @param additional_params Additional parameters

void CECoordinates::ConvertCoordinates(CECoordinateType input_coord_type,
                                       double input_coord_x, double input_coord_y,
                                       CECoordinateType return_coord_type,
                                       double *return_coord_x, double *return_coord_y,
                                       std::vector<double> additional_params)
{}

////////////////////////////////////////////////////////////
/// CIRS -> ICRS coordinate conversion
///     @param input_ra     CIRS right ascension
///     @param input_dec    CIRS declination
///     @param return_ra    ICRS right ascension (returned)
///     @param return_dec   ICRS declinaton (returned)
///     @param date         Date information
///     @param angle_type   Angle type

void CECoordinates::CIRS2ICRS(double input_ra, double input_dec,
                              double *return_ra, double *return_dec,
                              CEDate date, CEAngleType angle_type)
{
    // Convert to radians if that's what is passed
    if (angle_type==CEAngleType::DEGREES) {
        input_ra  *= DD2R ;
        input_dec *= DD2R ;
    }
    
    // Use the sofa library to convert these coordinates
    double eo ; // Equation of the origins
    iauAtic13(input_ra, input_dec, date.JD(), 0.0, return_ra, return_dec, &eo) ;

    // Subtract the eo from RA
    *return_ra -= eo ;
    
    // Return the coordinates in the requested units
    if (angle_type == CEAngleType::DEGREES) {
        *return_ra  *= DR2D ;
        *return_dec *= DR2D ;
    }
}

////////////////////////////////////////////////////////////
/// CIRS -> Galactic coordinate conversion.
///     @param input_ra     CIRS right ascension
///     @param input_dec    CIRS declination
///     @param glon         Galactic longitude
///     @param glat         Galactic latitude
///     @param date         Date information
///     @param angle_type   Angle format (DEGREES or RADIANS)

void CECoordinates::CIRS2Galactic(double input_ra, double input_dec, double *glon, double *glat,
                                  CEDate date, CEAngleType angle_type)
{
    // Convert to radians if that's what is passed
    if (angle_type==CEAngleType::DEGREES) {
        input_ra  *= DD2R ;
        input_dec *= DD2R ;
    }
    
    // In order to do this with the sofa package, we must first
    // convert from CIRS -> ICRS
    double ICRS_ra(0.0), ICRS_dec(0.0) ;
    CIRS2ICRS(input_ra, input_dec, &ICRS_ra, &ICRS_dec, date) ;
    
    // Now we can convert to galactic
    ICRS2Galactic(ICRS_ra, ICRS_dec, glon, glat) ;
    
    // Convert to the desired units
    if (angle_type == CEAngleType::DEGREES) {
        *glon *= DR2D ;
        *glat *= DR2D ;
    }
}

////////////////////////////////////////////////////////////
/// CIRS -> Observed (or observer specific) coordinate conversion
/// This function takes in verious observation parameters
/// The integer returned is a status code with the following meanings:
///     +1 = dubious year (too far into the past/future to be trusted)
///      0 = OK status
///     -1 = unacceptable date
///
///     @param ra           Right ascension in CIRS coordinates
///     @param dec          Declination in CIRS coordinates
///     @param az           Azimuth (returned)
///     @param zen          Zenith angle (returned)
///     @param observer     Observer information
///     @param angle_type   Angle type (see CEAngleType)
///     @param wavelength   Wavelength of the light being observed (units?)
///     @param observed_ra  Observed right ascension (returned)
///     @param observed_dec Observed declination (returned)
///     @param hour_angle   Hour angle of object being observed (returned)

int CECoordinates::CIRS2Observed(double ra, double dec,
                                 double *az, double *zen,
                                 CEObserver& observer,
                                 CEAngleType angle_type,
                                 double wavelength,
                                 double *observed_ra,
                                 double *observed_dec,
                                 double *hour_angle)
{
    // If we've passed in angles that are in degrees we need to convert to radians
    if (angle_type == CEAngleType::DEGREES) {
        ra  *= DD2R ;
        dec *= DD2R ;
    }
    
    // Setup the observed RA, Dec and hour_angle variables
    double *temp_ra, *temp_dec, *temp_hour_angle ;
    // If values were passed, point these variables at the passed ones
    if (observed_ra != nullptr) temp_ra = observed_ra ;
    if (observed_dec != nullptr) temp_dec = observed_dec ;
    if (hour_angle != nullptr) temp_hour_angle = hour_angle ;
    
    int err_code = CIRS2Observed(ra, dec,
                                 az, zen,
                                 observer.Date()->JD(),
                                 observer.Longitude_Rad(),
                                 observer.Latitude_Rad(),
                                 observer.Elevation_m(),
                                 observer.Pressure_hPa(),
                                 observer.Temperature_C(),
                                 observer.RelativeHumidity(),
                                 observer.Date()->dut1(),
                                 observer.Date()->xpolar(),
                                 observer.Date()->xpolar(),
                                 wavelength,
                                 temp_ra, temp_dec, temp_hour_angle) ;
    
    // Now convert back to degrees if that's what we were passed
    if (angle_type == CEAngleType::DEGREES) {
        *az              *= DR2D ;
        *zen             *= DR2D ;
        *temp_ra         *= DR2D ;
        *temp_dec        *= DR2D ;
        *temp_hour_angle *= DR2D ;
    }
    
    return err_code ;
}

////////////////////////////////////////////////////////////
/// ICRS -> CIRS coordinate conversion
///
///     @param input_ra     Right ascension to be converted
///     @param input_dec    Declination to be converted
///     @param return_ra    CIRS Right ascension (returned)
///     @param return_dec   CIRS Declination (returned)
///     @param date         Date object
///     @param angle_type   Angle type (either DEGREES or RADIANS)

void CECoordinates::ICRS2CIRS(double input_ra, double input_dec,
                              double *return_ra, double *return_dec,
                              CEDate date, CEAngleType angle_type)
{
    // Convert to radians if necessary
    if (angle_type==CEAngleType::DEGREES) {
        input_ra  *= DD2R ;
        input_dec *= DD2R ;
    }
    
    // Store the equation of the origins
    double eo ; // Equation of the origins
    
    // Get the star-independent astrometry parameters for this date
    iauASTROM astrom ;
    iauApci13(date.JD(), 0.0, &astrom, &eo) ;
    
    // Use the sofa library to convert these coordinates
    iauAtciqz(input_ra+eo, input_dec, &astrom, return_ra, return_dec) ;
    
    return ;
}

////////////////////////////////////////////////////////////
/// ICRS -> Galactic coordinate conversion
///     @param input_ra     ICRS Right ascension
///     @param input_dec    ICRS Declination
///     @param glon         Galactic longitude
///     @param glat         Galactic latitude
///     @param angle_type   Angle type

void CECoordinates::ICRS2Galactic(double input_ra, double input_dec, double *glon, double *glat,
                                  CEAngleType angle_type)
{
    // Use the sofa method to convert the coordinates
    iauIcrs2g(input_ra, input_dec, glon, glat) ;
}

////////////////////////////////////////////////////////////
/// ICRS -> Observed coordinate conversion.
/// The integer returned is a status code with the following meanings:
///     +1 = dubious year (too far into the past/future to be trusted)
///      0 = OK status
///     -1 = unacceptable date
///
///     @param ra           ICRS right ascension
///     @param dec          ICRS declination
///     @param az           Azimuth
///     @param zen          Zenith
///     @param observer     Observer information
///     @param angle_type   Angle type
///     @param observed_ra  Apparent ICRS Right Ascension
///     @param observed_dec Apparent ICRS Declination

int CECoordinates::ICRS2Observed(double ra, double dec,             // RA, Dec in CIRS coordinates
                                 double *az, double *zen,           // Azimuth, zenith angle
                                 CEObserver& observer,        // Observer quantities, like geographic position,
                                                                    // weather conditions, and date
                                 CEAngleType angle_type,            // Angle type for all angles provided
                                                                    // (either RADIANS or DEGREES)
                                 double wavelength,                 // Observing wavelength (micrometers)
                                 double *observed_ra,               // Observed CIRS right ascension
                                 double *observed_dec)              // Observed CIRS declination
{
    // If we've passed in angles that are in degrees we need to convert to radians
    if (angle_type == CEAngleType::DEGREES) {
        ra  *= DD2R ;
        dec *= DD2R ;
    }

    // Call the necessary sofa method
    int err_code = ICRS2Observed(ra, dec,
                                 az, zen,
                                 observer.Date()->JD(),
                                 observer.Longitude_Rad(),
                                 observer.Latitude_Rad(),
                                 observer.Elevation_m(),
                                 observer.Pressure_hPa(),
                                 observer.Temperature_C(),
                                 observer.RelativeHumidity(),
                                 observer.Date()->dut1(),
                                 observer.Date()->xpolar(),
                                 observer.Date()->xpolar(),
                                 wavelength,
                                 observed_ra, observed_dec) ;
    
    // Now convert back to degrees if that's what we were passed
    if (angle_type == CEAngleType::DEGREES) {
        *az           *= DR2D ;
        *zen          *= DR2D ;
        *observed_ra  *= DR2D ;
        *observed_dec *= DR2D ;
    }
    
    return err_code ;
}


/*********************************************************
 * Convert GALACTIC Coordinates
 *********************************************************/

////////////////////////////////////////////////////////////
/// Galactic -> CIRS coordinate conversion
///     @param glon     Galactic longitude
///     @param glat     Galactic latitude
///     @param ra       CIRS right ascension
///     @param dec      CIRS declination
///     @param date     Date information
///     @param angle_type   Angle type (RADIANS or DEGREES)

void CECoordinates::Galactic2CIRS(double glon, double glat, double *ra, double *dec,
                                  CEDate date, CEAngleType angle_type)
{
    // Check for degrees
    if (angle_type == CEAngleType::DEGREES) {
        glon *= DD2R ;
        glat *= DD2R ;
    }
    
    // Do the Galactic -> ICRS converstion
    Galactic2ICRS(glon, glat, ra, dec) ;
    
    // Now convert ICRS -> CIRS
    double tmp_ra(*ra), tmp_dec(*dec) ;
    ICRS2CIRS(tmp_ra, tmp_dec, ra, dec, date) ;
    
    // Now make sure to return the coordinates in the correct format
    if (angle_type == CEAngleType::DEGREES) {
        *ra  *= DR2D ;
        *dec *= DR2D ;
    }
}

////////////////////////////////////////////////////////////
/// Galactic -> ICRS coordinate conversion
///     @param glon         Galactic longitude
///     @param glat         Galactic latitude
///     @param ra           ICRS right ascension (returned)
///     @param dec          ICRS declinaton (returned)
///     @param angle_type   Angle type (DEGREES or RADIANS)

void CECoordinates::Galactic2ICRS(double glon, double glat, double *ra, double *dec,
                                  CEAngleType angle_type)
{
    // Check for degrees
    if (angle_type == CEAngleType::DEGREES) {
        glon *= DD2R ;
        glat *= DD2R ;
    }
    
    // Do the Galactic -> ICRS converstion
    iauG2icrs(glon, glat, ra, dec) ;
    
    // Now make sure to return the coordinates in the correct format
    if (angle_type == CEAngleType::DEGREES) {
        *ra  *= DR2D ;
        *dec *= DR2D ;
    }
}

////////////////////////////////////////////////////////////
/// Galactic -> Observed (i.e. observer specific) coordinate conversion.
/// For the raw version of this method see CECoordinates::Galactic2Observed().
///     @param glon             Galactic longitude
///     @param glat             Galactic latitude
///     @param az               Azimuth angle (returned)
///     @param zen              Zenith angle (returned)
///     @param observer         Observer information
///     @param wavelength       Wavelength
///     @param observed_glon    Observed galactic longitude (returned)
///     @param observed_glat    Observed galactic latitude (returned)

int CECoordinates::Galactic2Observed(double glon, double glat,
                                     double *az, double *zen,
                                     CEObserver& observer,
                                     CEAngleType angle_type,
                                     double wavelength,
                                     double *observed_glon,
                                     double *observed_glat)
{
    // Convert angles to radians if necessary
    if (angle_type == CEAngleType::DEGREES) {
        glon *= DD2R ;
        glat *= DD2R ;
    }
    
    // Do the conversion
    int error_code = Galactic2Observed(glon, glat,
                                       az, zen,
                                       observer.Date()->JD(),
                                       observer.Longitude_Rad(),
                                       observer.Latitude_Rad(),
                                       observer.Elevation_m(),
                                       observer.Pressure_hPa(),
                                       observer.Temperature_C(),
                                       observer.RelativeHumidity(),
                                       observer.Date()->dut1(),
                                       observer.Date()->xpolar(),
                                       observer.Date()->xpolar(),
                                       wavelength,
                                       observed_glon, observed_glat) ;
    
    // Convert back to degrees if necessary
    if (angle_type == CEAngleType::DEGREES) {
        *az  *= DR2D ;
        *zen *= DR2D ;
        if (observed_glon != nullptr) *observed_glon *= DR2D ;
        if (observed_glat != nullptr) *observed_glat *= DR2D ;
    }
    
    return error_code;
}

/*********************************************************
 * Convert OBSERVED Coordinates
 *********************************************************/


/*********************************************************
 * Main routines for converting TO OBSERVED Coordinates
 *********************************************************/

////////////////////////////////////////////////////////////
/// Raw method for converting CIRS -> Observed (observer specific) coordinates.
/// Note: All angles are expected to be in radians.
///     @param ra                   CIRS right ascension
///     @param dec                  CIRS declination
///     @param az                   Observed azimuth angle (returned)
///     @param zen                  Observed zenith angle (returned)
///     @param julian_date          Julian date for conversion
///     @param latitude             Observer geographic latitude
///     @param longitude            Observer geographic longitude
///     @param elevation_m          Observer elevation (meters)
///     @param pressure_hPa         Atmospheric pressure (HPa)
///     @param temperature_celsius  Temperature (degrees Celsius)
///     @param relative_humidity    Relative humidity (0.0 - 1.0)
///     @param dut1                 UTC - UT1
///     @param xp                   "x" polar motion
///     @param yp                   "y" polar motion
///     @param wavelength           Wavelength
///     @param observed_ra          Apparent right ascension (returned)
///     @param observed_dec         Apparent declination (returned)
///     @param hour_angle           Hour angle

int CECoordinates::CIRS2Observed(double ra, double dec,
                                 double *az, double *zen,
                                 double julian_date,
                                 double longitude,
                                 double latitude,
                                 double elevation_m,
                                 double pressure_hPa,
                                 double temperature_celsius,
                                 double relative_humidity,
                                 double dut1,
                                 double xp, double yp,
                                 double wavelength,
                                 double *observed_ra,
                                 double *observed_dec,
                                 double *hour_angle)
{
    // Setup the observed RA, Dec and hour_angle variables
    double *temp_ra, *temp_dec, *temp_hour_angle ;
    // If values were passed, point these variables at the passed ones
    if (observed_ra != nullptr) temp_ra = observed_ra ;
    if (observed_dec != nullptr) temp_dec = observed_dec ;
    if (hour_angle != nullptr) temp_hour_angle = hour_angle ;

    // Call the necessary sofa method
    int err_code = iauAtio13(ra, dec,
                             julian_date, 0.0,
                             dut1,
                             longitude,
                             latitude,
                             elevation_m,
                             xp, yp,
                             pressure_hPa,
                             temperature_celsius,
                             relative_humidity,
                             wavelength,
                             az, zen,
                             temp_hour_angle, temp_dec, temp_ra) ;

    return err_code ;
}

////////////////////////////////////////////////////////////
/// Raw method for converting CIRS -> Observed (observer specific) coordinates.
/// Note: All angles are expected to be in radians.
///     @param ra                   CIRS right ascension
///     @param dec                  CIRS declination
///     @param az                   Observed azimuth angle (returned)
///     @param zen                  Observed zenith angle (returned)
///     @param julian_date          Julian date for conversion
///     @param latitude             Observer geographic latitude
///     @param longitude            Observer geographic longitude
///     @param elevation_m          Observer elevation (meters)
///     @param pressure_hPa         Atmospheric pressure (HPa)
///     @param temperature_celsius  Temperature (degrees Celsius)
///     @param relative_humidity    Relative humidity (0.0 - 1.0)
///     @param dut1                 UTC - UT1
///     @param xp                   "x" polar motion
///     @param yp                   "y" polar motion
///     @param wavelength           Wavelength
///     @param observed_ra          Apparent right ascension (returned)
///     @param observed_dec         Apparent declination (returned)

int CECoordinates::ICRS2Observed(double ra, double dec,
                                 double *az, double *zen,
                                 double julian_date,
                                 double longitude,
                                 double latitude,
                                 double elevation_m,
                                 double pressure_hPa,
                                 double temperature_celsius,
                                 double relative_humidity,
                                 double dut1,
                                 double xp, double yp,
                                 double wavelength,
                                 double *observed_ra,
                                 double *observed_dec)
{
    // Setup the observed RA, Dec and hour_angle variables
    double *temp_ra, *temp_dec ;
    // If values were passed, point these variables at the passed ones
    if (observed_ra != nullptr) temp_ra = observed_ra ;
    if (observed_dec != nullptr) temp_dec = observed_dec ;
    
    // First convert the ICRS coordinats to CIRS coordinates
    CEDate date(julian_date, CEDateType::JD) ;
    ICRS2CIRS(ra, dec, &ra, &dec, date) ;
    
    // Call the necessary sofa method
    int err_code = CIRS2Observed(ra, dec,
                                 az, zen,
                                 julian_date,
                                 longitude,
                                 latitude,
                                 elevation_m,
                                 pressure_hPa,
                                 temperature_celsius,
                                 relative_humidity,
                                 dut1, xp, yp,
                                 wavelength,
                                 temp_ra,
                                 temp_dec) ;
    
    // Convert the apparent CIRS RA,Dec to ICRS RA,Dec
    CIRS2ICRS(*temp_ra, *temp_dec, temp_ra, temp_dec, date) ;
    
    return err_code ;
}

////////////////////////////////////////////////////////////
/// Raw method for converting Galactic -> Observed (observer specific) coordinates.
/// Note: All angles are expected to be in radians.
///     @param glon                 Galactic longitude
///     @param glat                 Galactic latitude
///     @param az                   Observed azimuth angle (returned)
///     @param zen                  Observed zenith angle (returned)
///     @param julian_date          Julian date for conversion
///     @param latitude             Observer geographic latitude
///     @param longitude            Observer geographic longitude
///     @param elevation_m          Observer elevation (meters)
///     @param pressure_hPa         Atmospheric pressure (HPa)
///     @param temperature_celsius  Temperature (degrees Celsius)
///     @param relative_humidity    Relative humidity (0.0 - 1.0)
///     @param dut1                 UTC - UT1
///     @param xp                   "x" polar motion
///     @param yp                   "y" polar motion
///     @param wavelength           Wavelength
///     @param observed_glon        Apparent galactic longitude (returned)
///     @param observed_glat        Apparent galactic latitude (returned)

int CECoordinates::Galactic2Observed(double glon, double glat,
                                     double *az, double *zen,
                                     double julian_date,
                                     double longitude,
                                     double latitude,
                                     double elevation_m,
                                     double pressure_hPa,
                                     double temperature_celsius,
                                     double relative_humidity,
                                     double dut1,
                                     double xp, double yp,
                                     double wavelength,
                                     double *observed_glon,
                                     double *observed_glat)
{
    // Setup the observed RA, Dec and hour_angle variables
    double *temp_glon, *temp_glat ;
    double temp_ra, temp_dec, temp_ha ;
    // If values were passed, point these variables at the passed ones
    if (observed_glon != nullptr) temp_glon = observed_glon ;
    if (observed_glat != nullptr) temp_glat = observed_glat ;
    
    // Convert GALACTIC to ICRS
    double ra(0.0), dec(0.0) ;
    CEDate date(julian_date, CEDateType::JD) ;
    Galactic2CIRS(glon, glat, &ra, &dec, date) ;

    // Call the necessary sofa method
    int err_code = CIRS2Observed(ra, dec,
                                 az, zen,
                                 julian_date,
                                 longitude,
                                 latitude,
                                 elevation_m,
                                 pressure_hPa,
                                 temperature_celsius,
                                 relative_humidity,
                                 dut1, xp, yp,
                                 wavelength,
                                 &temp_ra,
                                 &temp_dec,
                                 &temp_ha) ;

    // Convert the apparent RA,Dec to galactic longitude,latitude
    CIRS2Galactic(temp_ra, temp_dec, temp_glon, temp_glat, date) ;
    
    return err_code ;
}

////////////////////////////////////////////////////////////
/// Return the local sky coordinates of this object as a CECoordinates object
///     @param julian_date          Julian date of the observation
///     @param longitude            Observer longitude (radians)
///     @param latitude             Observer latitude (radians)
///     @param elevation_m          Observer elevation (meters)
///     @param pressure_hPa         Observer atmospheric pressure (hPa)
///     @param temerature_celsius   Temperature (degrees Celsius)
///     @param relative_humidity    Relative humidity
///     @param dut1                 'UTC-UT1'
///     @param xp                   x-polar motion
///     @param yp                   y-polar motion
///     @param wavelength           Wavelength being observed
///     @return These coordinates converted into the observed coordinates of the observer specified
CECoordinates CECoordinates::GetObservedCoords(double julian_date,
                                double longitude,
                                double latitude,
                                double elevation_m,
                                double pressure_hPa,
                                double temperature_celsius,
                                double relative_humidity,
                                double dut1,
                                double xp, double yp,
                                double wavelength)
{
    double azimuth, zenith ;
    double observed1, observed2, observed3 ;
    
    if (coord_type_ == CECoordinateType::CIRS) {
        // Convert CIRS to Observed
        CIRS2Observed(xcoord_, ycoord_, &azimuth, &zenith,
                      julian_date, longitude, latitude,
                      elevation_m, pressure_hPa, temperature_celsius,
                      relative_humidity, dut1, xp, yp, wavelength,
                      &observed1, &observed2, &observed3) ;
    } else if (coord_type_ == CECoordinateType::ICRS) {
        // Convert ICRS to Observed
        ICRS2Observed(xcoord_, ycoord_, &azimuth, &zenith,
                      julian_date, longitude, latitude,
                      elevation_m, pressure_hPa, temperature_celsius,
                      relative_humidity, dut1, xp, yp, wavelength,
                      &observed1, &observed3) ;
    } else if (coord_type_ == CECoordinateType::GALACTIC) {
        // Convert Galactic to Observed
        Galactic2Observed(xcoord_, ycoord_, &azimuth, &zenith,
                          julian_date, longitude, latitude,
                          elevation_m, pressure_hPa, temperature_celsius,
                          relative_humidity, dut1, xp, yp, wavelength,
                          &observed1, &observed2) ;
    }
    
    // Create the CECoordinates object to be returned
    return CECoordinates(azimuth, zenith, CECoordinateType::OBSERVED) ;
}

////////////////////////////////////////////////////////////
/// Return the observed coordinates using an observer object (CEObserver)
///     @param julian_date          Julian date of the observation
///     @param observer             Observer information
///     @param dut1                 'UTC-UT1'
///     @param xp                   x-polar motion
///     @param yp                   y-polar motion
///     @param wavelength           Wavelength being observed
///     @return These coordinates converted into the observed coordinates of 'observer'
CECoordinates CECoordinates::GetObservedCoords(double julian_date,
                                CEObserver& observer,
                                double dut1,
                                double xp, double yp,
                                double wavelength)
{
    return GetObservedCoords(julian_date,
                             observer.Longitude_Rad(),
                             observer.Latitude_Rad(),
                             observer.Elevation_m(),
                             observer.Pressure_hPa(),
                             observer.Temperature_C(),
                             observer.RelativeHumidity(),
                             dut1, xp, yp, wavelength);
}
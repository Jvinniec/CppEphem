//
//  CECoordinates.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 3/26/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include "CECoordinates.h"

#pragma mark - Constructors & Destructors

//__________________________________________________________
/** Default constructor */
CECoordinates::CECoordinates() :
    xcoord_(0.0),
    ycoord_(0.0),
    coord_type_(CECoordinateType::CIRS)
{}

//__________________________________________________________
/** Primary constructor */
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


//__________________________________________________________
CECoordinates::CECoordinates(CECoordinateType coord_type) :
    xcoord_(0.0),
    ycoord_(0.0),
    coord_type_(coord_type)
{}


//__________________________________________________________
// Copy constructor
CECoordinates::CECoordinates(const CECoordinates& other) :
    xcoord_(other.xcoord_),
    ycoord_(other.ycoord_),
    coord_type_(other.coord_type_)
{}


#pragma mark - Public Methods


#pragma mark - Conversion routines

//__________________________________________________________
void CECoordinates::ConvertCoordinates(CECoordinateType input_coord_type,
                                       double input_coord_x, double input_coord_y,
                                       CECoordinateType return_coord_type,
                                       double *return_coord_x, double *return_coord_y,
                                       std::vector<double> additional_params)
{}

/*********************************************************
 * Convert CIRS Coordinates
 *********************************************************/

//__________________________________________________________
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

//__________________________________________________________
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


/*********************************************************************
 * This function takes in verious observation parameters
 * The integer returned is a status code with the following meanings:
 *      +1 = dubious year (too far into the past/future to be trusted)
 *       0 = OK status
 *      -1 = unacceptable date
 * RETURNED:
 *      observed_ra  = observed right ascension
 *      observed_dec = observed declination
 *      hour_angle   = observed hour angle
 *      az           = observed azimuth
 *      zen          = observed zenith
 *********************************************************************/
int CECoordinates::CIRS2Observed(double ra, double dec,             // RA, Dec in CIRS coordinates
                                 double *az, double *zen,           // Azimuth, zenith angle
                                 CEObserver observer,               // Observer quantities, like geographic position,
                                                                    // weather conditions, and date
                                 CEAngleType angle_type,            // Angle type for all angles provided
                                                                    // (either RADIANS or DEGREES)
                                 double wavelength,                 // Observing wavelength (micrometers)
                                 double *observed_ra,               // Observed CIRS right ascension
                                 double *observed_dec,              // Observed CIRS declination
                                 double *hour_angle)                // Hour angle for coordinates
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
                                 observer.Date().JD(),
                                 observer.Longitude_Rad(),
                                 observer.Latitude_Rad(),
                                 observer.Elevation(),
                                 observer.Pressure(),
                                 observer.Temperature_C(),
                                 observer.RelativeHumidity(),
                                 observer.Date().dut1(),
                                 observer.Date().xpolar(),
                                 observer.Date().xpolar(),
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

/*********************************************************
 * Convert ICRS Coordinates
 *********************************************************/

//__________________________________________________________
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

//__________________________________________________________
void CECoordinates::ICRS2Galactic(double input_ra, double input_dec, double *glon, double *glat,
                                  CEAngleType angle_type)
{
    // Use the sofa method to convert the coordinates
    iauIcrs2g(input_ra, input_dec, glon, glat) ;
}


/*********************************************************************
 * This function takes in verious observation parameters
 * The integer returned is a status code with the following meanings:
 *      +1 = dubious year (too far into the past/future to be trusted)
 *       0 = OK status
 *      -1 = unacceptable date
 * RETURNED:
 *      az           = Observed azimuth
 *      zen          = Observed zenith
 *      observed_ra  = Apparent ICRS Right Ascension
 *      observed_dec = Apparent ICRS Declination
 *********************************************************************/
int CECoordinates::ICRS2Observed(double ra, double dec,             // RA, Dec in CIRS coordinates
                                 double *az, double *zen,           // Azimuth, zenith angle
                                 CEObserver observer,               // Observer quantities, like geographic position,
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
                                 observer.Date().JD(),
                                 observer.Longitude_Rad(),
                                 observer.Latitude_Rad(),
                                 observer.Elevation(),
                                 observer.Pressure(),
                                 observer.Temperature_C(),
                                 observer.RelativeHumidity(),
                                 observer.Date().dut1(),
                                 observer.Date().xpolar(),
                                 observer.Date().xpolar(),
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

/*********************************************************************
 * This function takes in verious observation parameters
 * PASSED:
 *      glon = galactic longitude
 *      glat = galactic latitude
 * RETURNED:
 *      ra  = CIRS right ascension
 *      dec = CIRS declination
 *********************************************************************/
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

/*********************************************************************
 * This function takes in verious observation parameters
 * PASSED:
 *      glon = galactic longitude
 *      glat = galactic latitude
 * RETURNED:
 *      ra  = CIRS right ascension
 *      dec = CIRS declination
 *********************************************************************/
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


/*********************************************************************
 * This function takes in verious observation parameters
 * PASSED:
 *      glon = galactic longitude
 *      glat = galactic latitude
 * RETURNED:
 *      ra  = CIRS right ascension
 *      dec = CIRS declination
 *********************************************************************/
int CECoordinates::Galactic2Observed(double glon, double glat,  // RA, Dec in CIRS coordinates
                                     double *az, double *zen,   // Azimuth, zenith angle
                                     CEObserver observer,
                                     CEAngleType angle_type,    // Angle type for all angles provided
                                                                // (either RADIANS or DEGREES)
                                     double wavelength,         // Observing wavelength (micrometers)
                                     double *observed_glon,     // Apparent galactic longitude
                                     double *observed_glat)     // Apparent galactic latitude
{
    // Convert angles to radians if necessary
    if (angle_type == CEAngleType::DEGREES) {
        glon *= DD2R ;
        glat *= DD2R ;
    }
    
    // Do the conversion
    int error_code = Galactic2Observed(glon, glat,
                                       az, zen,
                                       observer.Date().JD(),
                                       observer.Longitude_Rad(),
                                       observer.Latitude_Rad(),
                                       observer.Elevation(),
                                       observer.Pressure(),
                                       observer.Temperature_C(),
                                       observer.RelativeHumidity(),
                                       observer.Date().dut1(),
                                       observer.Date().xpolar(),
                                       observer.Date().xpolar(),
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

/*********************************************************************
 * CONVERTS CIRS to OBSERVED coordinates
 * This function takes in verious observation parameters
 * PASSED:
 *      ra          = CIRS Right Ascension (radians)
 *      dec         = CIRS Declination (radians)
 *      julian_date = Julian date for conversion
 *      longitude   = Geographic longitude (radians,East=positive)
 *      latitude    = Geographic latitude (radians)
 * OPTIONAL PASSED:
 *      elevation_m  = Observer elevation (meters)
 *      pressure_hPa = Atmospheric pressure (hPa)
 *      temerature_celsius = Atmospheric temperature (Celsius)
 *      dut1         = UT1 - UTC
 *      xp,yp        = x & y component of motion of the pole
 *      wavelength   = wavelength of light observed (micrometers)
 * RETURNED:
 *      az   = Azimuth (radians)
 *      zen  = Zenith angle (radians)
 *   (optional)
 *      observed_ra  = Observed CIRS Right Ascension
 *      observed_dec = Observed CIRS Declination
 *      hour_angle   = hour angle of the coordinates passed
 *********************************************************************/
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


/*********************************************************************
 * Converts ICRS to OBSERVED coordinates.s
 * This function takes in verious observation parameters
 * PASSED:
 *      ra          = ICRS Right Ascension (radians)
 *      dec         = ICRS Declination (radians)
 *      julian_date = Julian date for conversion
 *      longitude   = Geographic longitude (radians,East=positive)
 *      latitude    = Geographic latitude (radians)
 * OPTIONAL PASSED:
 *      elevation_m  = Observer elevation (meters)
 *      pressure_hPa = Atmospheric pressure (hPa)
 *      temerature_celsius = Atmospheric temperature (Celsius)
 *      dut1         = UT1 - UTC
 *      xp,yp        = x & y component of motion of the pole
 *      wavelength   = wavelength of light observed (micrometers)
 * RETURNED:
 *      az   = Azimuth (radians)
 *      zen  = Zenith angle (radians)
 *   (optional)
 *      observed_ra  = Observed CIRS Right Ascension
 *      observed_dec = Observed CIRS Declination
 *********************************************************************/
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


/*********************************************************************
 * Converts ICRS to OBSERVED coordinates.s
 * This function takes in verious observation parameters
 * PASSED:
 *      glon        = Galactic longitude (radians)
 *      glat        = Galactic latitude (radians)
 *      julian_date = Julian date for conversion
 *      longitude   = Geographic longitude (radians,East=positive)
 *      latitude    = Geographic latitude (radians)
 * OPTIONAL PASSED:
 *      elevation_m  = Observer elevation (meters)
 *      pressure_hPa = Atmospheric pressure (hPa)
 *      temerature_celsius = Atmospheric temperature (Celsius)
 *      dut1         = UT1 - UTC
 *      xp,yp        = x & y component of motion of the pole
 *      wavelength   = wavelength of light observed (micrometers)
 * RETURNED:
 *      az   = Azimuth (radians)
 *      zen  = Zenith angle (radians)
 *   (optional)
 *      observed_glon = Observed Galactic longitude
 *      observed_glat = Observed galactic latitude
 *********************************************************************/
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
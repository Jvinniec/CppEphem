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
// Default constructor
CECoordinates::CECoordinates() :
    xcoord_(0.0),
    ycoord_(0.0),
    coord_type_(CECoordinateType::CIRS)
{}

//__________________________________________________________
// Primary constructor
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
                                       double *return_coord_x, double *return_coord2_y,
                                       std::vector<double> additional_params)
{}

/*********************************************************
 * Convert CIRS Coordinates
 *********************************************************/

//__________________________________________________________
void CECoordinates::CIRS2ICRS(double input_ra, double input_dec,
                              double *return_ra, double *return_dec,
                              CEDate date)
{
    // Use the sofa library to convert these coordinates
    double eo ; // Equation of the origins
    iauAtic13(input_ra, input_dec, date.JD(), 0.0, return_ra, return_dec, &eo) ;

    // Subtract the
    return ;
}

//__________________________________________________________
void CECoordinates::CIRS2Galactic(double input_ra, double input_dec, double *glon, double *glat,
                                  CEDate date)
{
    // In order to do this with the sofa package, we must first
    // convert from CIRS -> ICRS
    double ICRS_ra(0.0), ICRS_dec(0.0) ;
    CIRS2ICRS(input_ra, input_dec, &ICRS_ra, &ICRS_dec, date) ;
    
    // Now we can convert to galactic
    ICRS2Galactic(ICRS_ra, ICRS_dec, glon, glat) ;
}


//__________________________________________________________
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
                                 double xp, double yp,              // Polar motion, can be found in IERS bulletins,
                                                                    // but 0 is probably fine for both
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
    
    // Call the necessary sofa method
    int err_code = iauAtio13(ra, dec,
                             observer.Date().JD(), 0.0,
                             observer.Date().dut1(),
                             observer.Longitude_Rad(),
                             observer.Latitude_Rad(),
                             observer.Elevation(),
                             xp, yp,
                             observer.Pressure(),
                             observer.Temperature_C(),
                             observer.RelativeHumidity(),
                             wavelength,
                             az, zen,
                             temp_hour_angle, temp_ra, temp_dec) ;
    
    // Now convert back to degrees if that's what we were passed
    if (angle_type == CEAngleType::DEGREES) {
        *temp_ra         *= DR2D ;
        *temp_dec        *= DR2D ;
        *temp_hour_angle *= DR2D ;
        *az              *= DR2D ;
        *zen             *= DR2D ;
    }
    
    return err_code ;
}

/*********************************************************
 * Convert ICRS Coordinates
 *********************************************************/

//__________________________________________________________
void CECoordinates::ICRS2CIRS(double input_ra, double input_dec,
                              double *return_ra, double *return_dec,
                              CEDate date)
{
    // Use the sofa library to convert these coordinates
    double eo ; // Equation of the origins
    iauAtic13(input_ra, input_dec, date.JD(), 0.0, return_ra, return_dec, &eo) ;
    
    // Subtract the
    return ;
}

//__________________________________________________________
void CECoordinates::ICRS2Galactic(double input_ra, double input_dec, double *glon, double *glat)
{
    // Use the sofa method to convert the coordinates
    iauIcrs2g(input_ra, input_dec, glon, glat) ;
}


//__________________________________________________________
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
int CECoordinates::ICRS2Observed(double ra, double dec,             // RA, Dec in CIRS coordinates
                                 double *az, double *zen,           // Azimuth, zenith angle
                                 CEObserver observer,               // Observer quantities, like geographic position,
                                                                    // weather conditions, and date
                                 CEAngleType angle_type,            // Angle type for all angles provided
                                                                    // (either RADIANS or DEGREES)
                                 double xp, double yp,              // Polar motion, can be found in IERS bulletins,
                                                                    // but 0 is probably fine for both
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
    
    // Call the necessary sofa method
    int err_code = iauAtio13(ra, dec,
                             observer.Date().JD(), 0.0,
                             observer.Date().dut1(),
                             observer.Longitude_Rad(),
                             observer.Latitude_Rad(),
                             observer.Elevation(),
                             xp, yp,
                             observer.Pressure(),
                             observer.Temperature_C(),
                             observer.RelativeHumidity(),
                             wavelength,
                             az, zen,
                             temp_hour_angle, temp_ra, temp_dec) ;
    
    // Now convert back to degrees if that's what we were passed
    if (angle_type == CEAngleType::DEGREES) {
        *temp_ra         *= DR2D ;
        *temp_dec        *= DR2D ;
        *temp_hour_angle *= DR2D ;
        *az              *= DR2D ;
        *zen             *= DR2D ;
    }
    
    return err_code ;
}


/*********************************************************
 * Convert GALACTIC Coordinates
 *********************************************************/


/*********************************************************
 * Convert OBSERVED Coordinates
 *********************************************************/


/*********************************************************
 * Main routines for converting TO OBSERVED Coordinates
 *********************************************************/
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
                             temp_hour_angle, temp_ra, temp_dec) ;
    
    return err_code ;
}
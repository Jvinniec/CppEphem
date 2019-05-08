/***************************************************************************
 *  CECoordinates.cpp: CppEphem                                            *
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

/**********************************************************************//**
 * Default constructor
 *************************************************************************/
CECoordinates::CECoordinates()
{
    init_members();
}


/**********************************************************************//**
 * Primary constructor (NOTE: xcoord & ycoord are expected to be in radians by default.
 * 
 * @param[in] xcoord X-Coordinate (radians)
 * @param[in] ycoord Y-Coordinate (radians)
 * @param[in] coord_type Coordinate type (see CECoordinateType)
 * @param[in] angle_type Angle type (either DEGREES or RADIANS)
 *************************************************************************/
CECoordinates::CECoordinates(const double& xcoord, 
                             const double& ycoord,
                             const CECoordinateType& coord_type,
                             const CEAngleType& angle_type)
{
    init_members();
    CECoordinates::SetCoordinates(xcoord, ycoord, coord_type, angle_type);
}


/**********************************************************************//**
 * Primary constructor 
 * 
 * @param[in] xcoord X-Coordinate {hours,minutes,seconds}
 * @param[in] ycoord Y-Coordinate {degrees, arcmin, arcsec}
 * @param[in] coord_type Coordinate type (see CECoordinateType)
 * @param[in] angle_type Angle type (either DEGREES or RADIANS)
 *************************************************************************/
CECoordinates::CECoordinates(const std::vector<double>& xcoord,
                             const std::vector<double>& ycoord,
                             const CECoordinateType& coord_type)
{
    init_members();
    double x = CECoordinates::HMSToAngle(xcoord, CEAngleType::RADIANS);
    double y = CECoordinates::DMSToAngle(ycoord, CEAngleType::RADIANS);
    CECoordinates::SetCoordinates(x, y, coord_type, CEAngleType::RADIANS);
}


/**********************************************************************//**
 * Constructor from a coordinate type
 * 
 * @param[in] coord_type Coordinate type (see CECoordinateType)
 *************************************************************************/
CECoordinates::CECoordinates(const CECoordinateType& coord_type) :
    coord_type_(coord_type)
{
    init_members();
}

/**********************************************************************//**
 * Copy constructor
 *************************************************************************/
CECoordinates::CECoordinates(const CECoordinates& other)
{
    free_members();
    init_members();
    copy_members(other);
}

/**********************************************************************//**
 * Destructor
 *************************************************************************/
CECoordinates::~CECoordinates()
{}


/**********************************************************************//**
 * Overloaded '=' (assignment) operator
 *************************************************************************/
CECoordinates& CECoordinates::operator=(const CECoordinates& other)
{
    if (this != &other) {
        free_members();
        init_members();
        copy_members(other);
    }
    return *this;
}


/**********************************************************************//**
 * CIRS -> ICRS coordinate conversion.
 * (uses SOFA 'iauAtic13' function)
 * 
 * @param[in]  input_ra     CIRS right ascension
 * @param[in]  input_dec    CIRS declination
 * @param[out] return_ra    ICRS right ascension (returned)
 * @param[out] return_dec   ICRS declinaton (returned)
 * @param[in]  date         Date information
 * @param[in] angle_type    Input/output angle type (DEGREES or RADIANS)
 *************************************************************************/
void CECoordinates::CIRS2ICRS(double input_ra, double input_dec,
                              double *return_ra, double *return_dec,
                              const CEDate& date, const CEAngleType& angle_type)
{
    // Convert to radians if that's what is passed
    if (angle_type==CEAngleType::DEGREES) {
        input_ra  *= DD2R ;
        input_dec *= DD2R ;
    }
    
    // Use the sofa library to convert these coordinates
    double eo ; // Equation of the origins
    double tdb1(0.0);
    double tdb2(0.0);
    CEDate::UTC2TDB(date.MJD(), &tdb1, &tdb2);
    iauAtic13(input_ra, input_dec, tdb1, tdb2, return_ra, return_dec, &eo) ;

    // Subtract the eo from RA if J2000 coordinates are desired
    //*return_ra -= eo ;
    
    // Return the coordinates in the requested units
    if (angle_type == CEAngleType::DEGREES) {
        *return_ra  *= DR2D ;
        *return_dec *= DR2D ;
    }
}

/**********************************************************************//**
 * CIRS -> Galactic coordinate conversion.
 * 
 * @param[in]  input_ra     CIRS right ascension
 * @param[in]  input_dec    CIRS declination
 * @param[out] glon         Galactic longitude
 * @param[out] glat         Galactic latitude
 * @param[in]  date         Date information
 * @param[in]  angle_type   Angle format (DEGREES or RADIANS)
 *************************************************************************/
void CECoordinates::CIRS2Galactic(double input_ra, double input_dec, double *glon, double *glat,
                                  const CEDate& date, const CEAngleType& angle_type)
{
    // Convert to radians if that's what is passed
    if (angle_type==CEAngleType::DEGREES) {
        input_ra  *= DD2R ;
        input_dec *= DD2R ;
    }
    
    // In order to do this with the sofa package, we must first
    // convert from CIRS -> ICRS
    double ICRS_ra(0.0), ICRS_dec(0.0) ;
    CIRS2ICRS(input_ra, input_dec, &ICRS_ra, &ICRS_dec, date, CEAngleType::RADIANS);
    
    // Now we can convert to galactic
    ICRS2Galactic(ICRS_ra, ICRS_dec, glon, glat, CEAngleType::RADIANS) ;
    
    // Convert to the desired units
    if (angle_type == CEAngleType::DEGREES) {
        *glon *= DR2D ;
        *glat *= DR2D ;
    }
}

/**********************************************************************//**
 * CIRS -> Observed (or observer specific) coordinate conversion
 * This function takes in verious observation parameters
 * The integer returned is a status code with the following meanings:
 *     +1 = dubious year (too far into the past/future to be trusted)
 *      0 = OK status
 *     -1 = unacceptable date
 *
 * @param[in]  ra           Right ascension in CIRS coordinates
 * @param[in]  dec          Declination in CIRS coordinates
 * @param[out] az           Azimuth (returned)
 * @param[out] zen          Zenith angle (returned)
 * @param[in]  observer     Observer information
 * @param[in]  angle_type   Angle type (see CEAngleType)
 * @param[in]  wavelength   Wavelength of the light being observed (micrometers)
 * @param[out] observed_ra  Observed right ascension (returned)
 * @param[out] observed_dec Observed declination (returned)
 * @param[out] hour_angle   Hour angle of object being observed (returned)
 *************************************************************************/
int CECoordinates::CIRS2Observed(double ra, double dec,
                                 double *az, double *zen,
                                 const CEDate&     date,
                                 const CEObserver& observer,
                                 const CEAngleType& angle_type,
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
    double temp_ra, temp_dec, temp_hour_angle ;
    // If values were passed, point these variables at the passed ones
    if (observed_ra  == nullptr) observed_ra  = &temp_ra;
    if (observed_dec == nullptr) observed_dec = &temp_dec;
    if (hour_angle   == nullptr) hour_angle   = &temp_hour_angle;
    
    int err_code = CIRS2Observed(ra, dec,
                                 az, zen,
                                 date.JD(),
                                 observer.Longitude_Rad(),
                                 observer.Latitude_Rad(),
                                 observer.Elevation_m(),
                                 observer.Pressure_hPa(),
                                 observer.Temperature_C(),
                                 observer.RelativeHumidity(),
                                 date.dut1(),
                                 date.xpolar(),
                                 date.ypolar(),
                                 wavelength,
                                 observed_ra, 
                                 observed_dec, 
                                 hour_angle) ;
    
    // Now convert back to degrees if that's what we were passed
    if (angle_type == CEAngleType::DEGREES) {
        *az              *= DR2D ;
        *zen             *= DR2D ;
        *observed_ra     *= DR2D ;
        *observed_dec    *= DR2D ;
        *hour_angle      *= DR2D ;
    }
    
    return err_code ;
}

/**********************************************************************//**
 * ICRS -> CIRS coordinate conversion.
 * Uses the SOFA function 'iauAtci13'
 * 
 * @param[in]  input_ra     Right ascension to be converted
 * @param[in]  input_dec    Declination to be converted
 * @param[out] return_ra    CIRS Right ascension (returned)
 * @param[out] return_dec   CIRS Declination (returned)
 * @param[in]  date         Date object
 * @param[in]  angle_type   Angle type (either DEGREES or RADIANS)
 *************************************************************************/
void CECoordinates::ICRS2CIRS(double input_ra, double input_dec,
                              double *return_ra, double *return_dec,
                              const CEDate& date, const CEAngleType& angle_type)
{
    // Convert to radians if necessary
    if (angle_type==CEAngleType::DEGREES) {
        input_ra  *= DD2R ;
        input_dec *= DD2R ;
    }
    
    // Store the equation of the origins
    double eo ; // Equation of the origins
    
    // Use the sofa library to convert these coordinates
    try {
        double tdb1(0.0);
        double tdb2(0.0);
        CEDate::UTC2TDB(date.MJD(), &tdb1, &tdb2);
        iauAtci13(input_ra, input_dec, 
                  0.0, 0.0, 0.0, 0.0, 
                  tdb1, tdb2, 
                  return_ra, return_dec, &eo) ;
        
        // Subtract the equation of the origins if J2000 coordinates are desired
        //*return_ra -= eo ;
    } catch (std::exception &e) {
        throw CEException::sofa_exception("CECoordinates::ICRS2CIRS",
                                          "iauAtci13", 
                                          "Exception thrown");
    }
    
    // Convert the returned coordinates to the correct angle type
    if (angle_type == CEAngleType::DEGREES) {
        *return_ra *= DR2D ;
        *return_dec *= DR2D ;
    }
    
    return ;
}

/**********************************************************************//**
 * ICRS -> Galactic coordinate conversion (uses the SOFA 'iauIcrs2g' function)
 * 
 * @param[in]  input_ra     ICRS Right ascension
 * @param[in]  input_dec    ICRS Declination
 * @param[out] glon         Galactic longitude
 * @param[out] glat         Galactic latitude
 * @param[in]  angle_type   Angle type
 *************************************************************************/
void CECoordinates::ICRS2Galactic(double input_ra, double input_dec, 
                                  double *glon, double *glat,
                                  const CEAngleType& angle_type)
{
    // Convert to radians if necessary
    if (angle_type == CEAngleType::DEGREES) {
        input_ra *= DD2R ;
        input_dec *= DD2R ;
    }
    
    // Use the sofa method to convert the coordinates
    iauIcrs2g(input_ra, input_dec, glon, glat) ;
    
    // Convert back to degrees if necessary
    if (angle_type == CEAngleType::DEGREES) {
        *glon *= DR2D ;
        *glat *= DR2D ;
    }
}

/**********************************************************************//**
 * ICRS -> Observed coordinate conversion.
 * The integer returned is a status code with the following meanings:
 *     +1 = dubious year (too far into the past/future to be trusted)
 *      0 = OK status
 *     -1 = unacceptable date
 *
 * @param[in]  ra           ICRS right ascension
 * @param[in]  dec          ICRS declination
 * @param[out] az           Azimuth
 * @param[out] zen          Zenith
 * @param[in]  observer     Observer information
 * @param[in]  angle_type   Angle type
 * @param[out] observed_ra  Apparent ICRS Right Ascension
 * @param[out] observed_dec Apparent ICRS Declination
 * @param[out] hour_angle   Observed hour angle of object
 *************************************************************************/
int CECoordinates::ICRS2Observed(double ra, double dec,
                                 double *az, double *zen,
                                 const CEDate&     date,
                                 const CEObserver& observer,
                                 const CEAngleType& angle_type,
                                 double wavelength,
                                 double *observed_ra,
                                 double *observed_dec,
                                 double *hour_angle)
{
    // Setup the observed RA, Dec and hour_angle variables
    double temp_ra(0.0);
    double temp_dec(0.0);
    double temp_hour_angle(0.0);

    // If values were not passed, point them at the temporary ones
    if (observed_ra  == nullptr) observed_ra  = &temp_ra;
    if (observed_dec == nullptr) observed_dec = &temp_dec;
    if (hour_angle   == nullptr) hour_angle   = &temp_hour_angle;

    // If we've passed in angles that are in degrees we need to convert to radians
    if (angle_type == CEAngleType::DEGREES) {
        ra  *= DD2R ;
        dec *= DD2R ;
    }

    // Call the necessary sofa method
    int err_code = ICRS2Observed(ra, dec,
                                 az, zen,
                                 date.JD(),
                                 observer.Longitude_Rad(),
                                 observer.Latitude_Rad(),
                                 observer.Elevation_m(),
                                 observer.Pressure_hPa(),
                                 observer.Temperature_C(),
                                 observer.RelativeHumidity(),
                                 date.dut1(),
                                 date.xpolar(),
                                 date.ypolar(),
                                 wavelength,
                                 observed_ra, observed_dec, hour_angle) ;

    // Now convert back to degrees if that's what we were passed
    if (angle_type == CEAngleType::DEGREES) {
        *az           *= DR2D ;
        *zen          *= DR2D ;
        *observed_ra  *= DR2D ;
        *observed_dec *= DR2D ;
        *hour_angle   *= DR2D ;
    }
    
    return err_code ;
}


/*********************************************************
 * Convert GALACTIC Coordinates
 *********************************************************/

/**********************************************************************//**
 * Galactic -> CIRS coordinate conversion
 * 
 * @param[in]  glon     Galactic longitude
 * @param[in]  glat     Galactic latitude
 * @param[out] ra       CIRS right ascension
 * @param[out] dec      CIRS declination
 * @param[in]  date     Date information
 * @param[in]  angle_type   Angle type (RADIANS or DEGREES)
 *************************************************************************/
void CECoordinates::Galactic2CIRS(double glon, double glat, double *ra, double *dec,
                                  const CEDate& date, const CEAngleType& angle_type)
{
    // Check for degrees
    if (angle_type == CEAngleType::DEGREES) {
        glon *= DD2R ;
        glat *= DD2R ;
    }
    
    // Do the Galactic -> ICRS converstion
    Galactic2ICRS(glon, glat, ra, dec, CEAngleType::RADIANS) ;
    
    // Now convert ICRS -> CIRS
    double tmp_ra(*ra);
    double tmp_dec(*dec);
    ICRS2CIRS(tmp_ra, tmp_dec, ra, dec, date, CEAngleType::RADIANS) ;
    
    // Now make sure to return the coordinates in the correct format
    if (angle_type == CEAngleType::DEGREES) {
        *ra  *= DR2D ;
        *dec *= DR2D ;
    }
}

/**********************************************************************//**
 * Galactic -> ICRS coordinate conversion
 * 
 * @param[in]  glon         Galactic longitude
 * @param[in]  glat         Galactic latitude
 * @param[out] ra           ICRS right ascension (returned)
 * @param[out] dec          ICRS declinaton (returned)
 * @param[in]  angle_type   Angle type (DEGREES or RADIANS)
 *************************************************************************/
void CECoordinates::Galactic2ICRS(double glon, double glat, 
                                  double *ra, double *dec,
                                  const CEAngleType& angle_type)
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

/**********************************************************************//**
 * Galactic -> Observed (i.e. observer specific) coordinate conversion.
 * For the raw version of this method see CECoordinates::Galactic2Observed().
 * 
 * @param[in]  glon             Galactic longitude
 * @param[in]  glat             Galactic latitude
 * @param[out] az               Azimuth angle (returned)
 * @param[out] zen              Zenith angle (returned)
 * @param[in]  observer         Observer information
 * @param[in]  wavelength       Wavelength
 * @param[out] observed_glon    Observed galactic longitude (returned)
 * @param[out] observed_glat    Observed galactic latitude (returned)
 * @param[out] hour_angle       Observed hour angle of object
 *************************************************************************/
int CECoordinates::Galactic2Observed(double glon, double glat,
                                     double *az, double *zen,
                                     const CEDate&     date,
                                     const CEObserver& observer,
                                     const CEAngleType& angle_type,
                                     double wavelength,
                                     double *observed_glon,
                                     double *observed_glat,
                                     double *hour_angle)
{
    // Convert angles to radians if necessary
    if (angle_type == CEAngleType::DEGREES) {
        glon *= DD2R ;
        glat *= DD2R ;
    }
    
    // Do the conversion
    int error_code = Galactic2Observed(glon, glat,
                                       az, zen,
                                       date.JD(),
                                       observer.Longitude_Rad(),
                                       observer.Latitude_Rad(),
                                       observer.Elevation_m(),
                                       observer.Pressure_hPa(),
                                       observer.Temperature_C(),
                                       observer.RelativeHumidity(),
                                       date.dut1(),
                                       date.xpolar(),
                                       date.ypolar(),
                                       wavelength,
                                       observed_glon, observed_glat, hour_angle) ;
    
    // Convert back to degrees if necessary
    if (angle_type == CEAngleType::DEGREES) {
        *az  *= DR2D ;
        *zen *= DR2D ;
        if (observed_glon != nullptr) *observed_glon *= DR2D ;
        if (observed_glat != nullptr) *observed_glat *= DR2D ;
        if (hour_angle    != nullptr) *hour_angle    *= DR2D ;
    }
    
    return error_code;
}

/*********************************************************
 * Convert OBSERVED Coordinates
 *********************************************************/


/**********************************************************************//**
 * Convert Observed -> CIRS coordinates
 * 
 * @param[in]  az              Azimuth of the coordinates
 * @param[in]  zen             Zenith angle of the coordinates
 * @param[out] ra              CIRS right ascension (returned)
 * @param[out] dec             CIRS declination (returned)
 * @param[in]  observer        CEObserver object describing observer
 * @param[in]  angle_type      Angle type of input/output coordinates
 *************************************************************************/
int CECoordinates::Observed2CIRS(double az, double zen,
                            double *ra, double *dec,
                            const CEDate&     date,
                            const CEObserver& observer,
                            const CEAngleType& angle_type)
{
    // Convert to radians if necessary
    if (angle_type == CEAngleType::DEGREES) {
        az *= DD2R ;
        zen *= DD2R ;
    }
    
    // Call the raw method
    int err_code = Observed2CIRS(az, zen, ra, dec, date.JD(),
                  observer.Longitude_Rad(), observer.Latitude_Rad(),
                  observer.Elevation_m(),
                  observer.Pressure_hPa(), observer.Temperature_C(),
                  observer.RelativeHumidity(),
                  date.dut1(),
                  date.xpolar(), 
                  date.ypolar(),
                  observer.Wavelength_um()) ;
    
    // Convert back to degrees if necessary
    if (angle_type == CEAngleType::DEGREES) {
        *ra *= DR2D ;
        *dec *= DR2D ;
    }
    
    return err_code ;
}

/**********************************************************************//**
 * Convert Observed -> ICRS coordinates
 * 
 * @param[in]  az              Azimuth of the coordinates
 * @param[in]  zen             Zenith angle of the coordinates
 * @param[out] ra              CIRS right ascension (returned)
 * @param[out] dec             CIRS declination (returned)
 * @param[in]  observer        CEObserver object describing observer
 * @param[in]  angle_type      Angle type of input/output coordinates
 *************************************************************************/
int CECoordinates::Observed2ICRS(double az, double zen,
                                 double *ra, double *dec,
                                 const CEDate&     date,
                                 const CEObserver& observer,
                                 const CEAngleType& angle_type)
{
    // Convert to radians if necessary
    if (angle_type == CEAngleType::DEGREES) {
        az *= DD2R ;
        zen *= DD2R ;
    }
    
    // Call the raw method
    int err_code = Observed2ICRS(az, zen, ra, dec, 
                  date.JD(),
                  observer.Longitude_Rad(), 
                  observer.Latitude_Rad(),
                  observer.Elevation_m(),
                  observer.Pressure_hPa(), 
                  observer.Temperature_C(),
                  observer.RelativeHumidity(),
                  date.dut1(),
                  date.xpolar(), 
                  date.ypolar(),
                  observer.Wavelength_um()) ;
    
    // Convert back to degrees if necessary
    if (angle_type == CEAngleType::DEGREES) {
        *ra *= DR2D ;
        *dec *= DR2D ;
    }
    
    return err_code ;
}

/**********************************************************************//**
 * Convert Observed -> Galactic coordinates
 * 
 * @param[in]  az              Azimuth of the coordinates
 * @param[in]  zen             Zenith angle of the coordinates
 * @param[out] glon            Galactic longitude (returned)
 * @param[out] glat            Galactic latitude (returned)
 * @param[in]  observer        CEObserver object describing observer
 * @param[in]  angle_type      Angle type of input/output coordinates
 *************************************************************************/
int CECoordinates::Observed2Galactic(double az, double zen,
                                 double *glon, double *glat,
                                 const CEDate&     date,
                                 const CEObserver& observer,
                                 const CEAngleType& angle_type)
{
    // Convert to radians if necessary
    if (angle_type == CEAngleType::DEGREES) {
        az *= DD2R ;
        zen *= DD2R ;
    }
    
    // Call the raw method
    int err_code = Observed2Galactic(az, zen, glon, glat, date.JD(),
                  observer.Longitude_Rad(), observer.Latitude_Rad(),
                  observer.Elevation_m(),
                  observer.Pressure_hPa(), observer.Temperature_C(),
                  observer.RelativeHumidity(),
                  date.dut1(),
                  date.xpolar(), date.ypolar(),
                  observer.Wavelength_um()) ;
    
    // Convert back to degrees if necessary
    if (angle_type == CEAngleType::DEGREES) {
        *glon *= DR2D ;
        *glat *= DR2D ;
    }
    
    return err_code ;
}


/*********************************************************
 * Main routines for converting TO OBSERVED Coordinates
 *********************************************************/

/**********************************************************************//**
 * Raw method for converting CIRS -> Observed (observer specific) coordinates
 * (uses the SOFA 'iauAtio13' function)
 * Note: All angles are expected to be in radians.
 * 
 * @param[in]  ra                   CIRS right ascension
 * @param[in]  dec                  CIRS declination
 * @param[out] az                   Observed azimuth angle (returned)
 * @param[out] zen                  Observed zenith angle (returned)
 * @param[in]  julian_date          Julian date for conversion
 * @param[in]  latitude             Observer geographic latitude
 * @param[in]  longitude            Observer geographic longitude
 * @param[in]  elevation_m          Observer elevation (meters)
 * @param[in]  pressure_hPa         Atmospheric pressure (HPa)
 * @param[in]  temperature_celsius  Temperature (degrees Celsius)
 * @param[in]  relative_humidity    Relative humidity (0.0 - 1.0)
 * @param[in]  dut1                 UT1 - UTC
 * @param[in]  xp                   "x" polar motion
 * @param[in]  yp                   "y" polar motion
 * @param[in]  wavelength_um        Wavelength (micrometers)
 * @param[out] observed_ra          Apparent right ascension (returned)
 * @param[out] observed_dec         Apparent declination (returned)
 * @param[out] hour_angle           Hour angle
 *************************************************************************/
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
                                 double wavelength_um,
                                 double *observed_ra,
                                 double *observed_dec,
                                 double *hour_angle)
{
    // Setup the observed RA, Dec and hour_angle variables
    double temp_ra(0.0);
    double temp_dec(0.0);
    double temp_hour_angle(0.0);

    // If values were not passed, point them at the temporary ones
    if (observed_ra  == nullptr) observed_ra  = &temp_ra;
    if (observed_dec == nullptr) observed_dec = &temp_dec;
    if (hour_angle   == nullptr) hour_angle   = &temp_hour_angle;

    // Call the necessary sofa method
    int err_code = 0;
    try {
        err_code = iauAtio13(ra, dec,
                             julian_date, 0.0,
                             dut1,
                             longitude,
                             latitude,
                             elevation_m,
                             xp, yp,
                             pressure_hPa,
                             temperature_celsius,
                             relative_humidity,
                             wavelength_um,
                             az, zen,
                             hour_angle, 
                             observed_dec, 
                             observed_ra) ;
    } catch (std::exception &e) {
        throw CEException::sofa_exception("CECoordinates::CIRS2Observed",
                                          "iauAtio13",
                                          e.what());
    }

    return err_code ;
}

/**********************************************************************//**
 * Raw method for converting Observed (observer specific) -> CIRS coordinates
 * (uses the SOFA 'iauAtoi13' function)
 * Note: All angles are expected to be in radians.
 * 
 * @param[in]  az                   Observed azimuth angle
 * @param[in]  zen                  Observed zenith angle
 * @param[out] ra                   CIRS right ascension (returned)
 * @param[out] dec                  CIRS declination (returned)
 * @param[in]  julian_date          Julian date for conversion
 * @param[in]  latitude             Observer geographic latitude
 * @param[in]  longitude            Observer geographic longitude
 * @param[in]  elevation_m          Observer elevation (meters)
 * @param[in]  pressure_hPa         Atmospheric pressure (HPa)
 * @param[in]  temperature_celsius  Temperature (degrees Celsius)
 * @param[in]  relative_humidity    Relative humidity (0.0 - 1.0)
 * @param[in]  dut1                 UTC - UT1
 * @param[in]  xp                   "x" polar motion
 * @param[in]  yp                   "y" polar motion
 * @param[in]  wavelength_um        Wavelength (micrometers)
 *************************************************************************/
int CECoordinates::Observed2CIRS(double az, double zen,
                                 double *ra, double *dec,
                                 double julian_date,
                                 double longitude,
                                 double latitude,
                                 double elevation_m,
                                 double pressure_hPa,
                                 double temperature_celsius,
                                 double relative_humidity,
                                 double dut1,
                                 double xp, double yp,
                                 double wavelength_um)
{
    int err_code = iauAtoi13("A", az, zen,
                             julian_date, 0.0,
                             dut1,
                             longitude, latitude,
                             elevation_m,
                             xp, yp,
                             pressure_hPa,
                             temperature_celsius,
                             relative_humidity,
                             wavelength_um,
                             ra, dec) ;
    return err_code ;
}

/**********************************************************************//**
 * Raw method for converting CIRS -> Observed (observer specific) coordinates.
 * Note: All angles are expected to be in radians.
 * 
 * @param[in]  ra                   CIRS right ascension
 * @param[in]  dec                  CIRS declination
 * @param[out] az                   Observed azimuth angle (returned)
 * @param[out] zen                  Observed zenith angle (returned)
 * @param[in]  julian_date          Julian date for conversion
 * @param[in]  latitude             Observer geographic latitude
 * @param[in]  longitude            Observer geographic longitude
 * @param[in]  elevation_m          Observer elevation (meters)
 * @param[in]  pressure_hPa         Atmospheric pressure (HPa)
 * @param[in]  temperature_celsius  Temperature (degrees Celsius)
 * @param[in]  relative_humidity    Relative humidity (0.0 - 1.0)
 * @param[in]  dut1                 UTC - UT1
 * @param[in]  xp                   "x" polar motion
 * @param[in]  yp                   "y" polar motion
 * @param[in]  wavelength           Wavelength (micrometers)
 * @param[out] observed_ra          Apparent right ascension (returned)
 * @param[out] observed_dec         Apparent declination (returned)
 * @param[out] hour_angle           Hour angle
 *************************************************************************/
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
                                 double wavelength_um,
                                 double *observed_ra,
                                 double *observed_dec,
                                 double *hour_angle)
{
    // Setup the observed RA, Dec and hour_angle variables
    double temp_ra(0.0);
    double temp_dec(0.0);
    double temp_hour_angle(0.0);

    // If values were passed, point these variables at the passed ones
    if (observed_ra  == nullptr) observed_ra  = &temp_ra;
    if (observed_dec == nullptr) observed_dec = &temp_dec;
    if (hour_angle   == nullptr) hour_angle   = &temp_hour_angle;
    
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
                                 wavelength_um,
                                 observed_ra,
                                 observed_dec,
                                 hour_angle) ;
    
    // Convert the apparent CIRS RA,Dec to ICRS RA,Dec
    CIRS2ICRS(*observed_ra, *observed_dec, observed_ra, observed_dec, 
              date, CEAngleType::RADIANS) ;
    
    return err_code ;
}

/**********************************************************************//**
 * Raw method for converting Observed (observer specific) -> ICRS coordinates
 * Note: All angles are expected to be in radians.
 * 
 * @param[in]  az                   Observed azimuth angle
 * @param[in]  zen                  Observed zenith angle
 * @param[out] ra                   ICRS right ascension (returned)
 * @param[out] dec                  ICRS declination (returned)
 * @param[in]  julian_date          Julian date for conversion
 * @param[in]  latitude             Observer geographic latitude
 * @param[in]  longitude            Observer geographic longitude
 * @param[in]  elevation_m          Observer elevation (meters)
 * @param[in]  pressure_hPa         Atmospheric pressure (HPa)
 * @param[in]  temperature_celsius  Temperature (degrees Celsius)
 * @param[in]  relative_humidity    Relative humidity (0.0 - 1.0)
 * @param[in]  dut1                 UTC - UT1
 * @param[in]  xp                   "x" polar motion
 * @param[in]  yp                   "y" polar motion
 * @param[in]  wavelength_um        Wavelength (micrometers)
 *************************************************************************/
int CECoordinates::Observed2ICRS(double az, double zen,
                                 double *ra, double *dec,
                                 double julian_date,
                                 double longitude,
                                 double latitude,
                                 double elevation_m,
                                 double pressure_hPa,
                                 double temperature_celsius,
                                 double relative_humidity,
                                 double dut1,
                                 double xp, double yp,
                                 double wavelength_um)
{
    // Convert from Observed -> CIRS
    int err_code = Observed2CIRS(az, zen,
                                 ra, dec,
                                 julian_date,
                                 longitude, latitude,
                                 elevation_m, pressure_hPa,
                                 temperature_celsius, relative_humidity,
                                 dut1, xp, yp, wavelength_um) ;
    if (err_code == 0) {
        // Convert from CIRS -> ICRS
        CIRS2ICRS(*ra, *dec, ra, dec,
                  CEDate(julian_date,CEDateType::JD),
                  CEAngleType::RADIANS) ;
    }
    
    return err_code ;
}

/**********************************************************************//**
 * Raw method for converting Galactic -> Observed (observer specific) coordinates.
 * Note: All angles are expected to be in radians.
 * 
 * @param[in]  glon                 Galactic longitude
 * @param[in]  glat                 Galactic latitude
 * @param[out] az                   Observed azimuth angle (returned)
 * @param[out] zen                  Observed zenith angle (returned)
 * @param[in]  julian_date          Julian date for conversion
 * @param[in]  latitude             Observer geographic latitude
 * @param[in]  longitude            Observer geographic longitude
 * @param[in]  elevation_m          Observer elevation (meters)
 * @param[in]  pressure_hPa         Atmospheric pressure (HPa)
 * @param[in]  temperature_celsius  Temperature (degrees Celsius)
 * @param[in]  relative_humidity    Relative humidity (0.0 - 1.0)
 * @param[in]  dut1                 UTC - UT1
 * @param[in]  xp                   "x" polar motion
 * @param[in]  yp                   "y" polar motion
 * @param[in]  wavelength           Wavelength
 * @param[out] observed_glon        Apparent galactic longitude (returned)
 * @param[out] observed_glat        Apparent galactic latitude (returned)
 * @param[out] hour_angle           Hour angle
 *************************************************************************/
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
                                     double *observed_glat,
                                     double *hour_angle)
{
    // Setup the observed RA, Dec and hour_angle variables
    double temp_glon(0.0);
    double temp_glat(0.0);
    double temp_ra(0.0);
    double temp_dec(0.0);
    double temp_hour_angle(0.0);

    // If values were passed, point these variables at the passed ones
    if (observed_glon == nullptr) observed_glon = &temp_glon;
    if (observed_glat == nullptr) observed_glat = &temp_glat;
    if (hour_angle    == nullptr) hour_angle    = &temp_hour_angle;
    
    // Convert GALACTIC to ICRS
    double ra(0.0);
    double dec(0.0);
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
                                 &temp_hour_angle) ;

    // Convert the apparent RA,Dec to galactic longitude,latitude
    CIRS2Galactic(temp_ra, temp_dec, observed_glon, observed_glat, date) ;
    
    return err_code ;
}

/**********************************************************************//**
 * Raw method for converting Observed (observer specific) -> Galactic coordinates
 * Note: All angles are expected to be in radians.
 * 
 * @param[in]  az                   Observed azimuth angle (radians)
 * @param[in]  zen                  Observed zenith angle (radians)
 * @param[out] glon                 Galactic longitude (returned)
 * @param[out] glat                 Galactic latitude (returned)
 * @param[in]  julian_date          Julian date for conversion
 * @param[in]  latitude             Observer geographic latitude
 * @param[in]  longitude            Observer geographic longitude
 * @param[in]  elevation_m          Observer elevation (meters)
 * @param[in]  pressure_hPa         Atmospheric pressure (HPa)
 * @param[in]  temperature_celsius  Temperature (degrees Celsius)
 * @param[in]  relative_humidity    Relative humidity (0.0 - 1.0)
 * @param[in]  dut1                 UTC - UT1
 * @param[in]  xp                   "x" polar motion
 * @param[in]  yp                   "y" polar motion
 * @param[in]  wavelength_um        Wavelength (micrometers)
 *************************************************************************/
int CECoordinates::Observed2Galactic(double az, double zen,
                                 double *glon, double *glat,
                                 double julian_date,
                                 double longitude,
                                 double latitude,
                                 double elevation_m,
                                 double pressure_hPa,
                                 double temperature_celsius,
                                 double relative_humidity,
                                 double dut1,
                                 double xp, double yp,
                                 double wavelength_um)
{
    // Convert from Observed -> ICRS
    int err_code = Observed2ICRS(az, zen,
                                 glon, glat,
                                 julian_date,
                                 longitude, latitude,
                                 elevation_m, pressure_hPa,
                                 temperature_celsius, relative_humidity,
                                 dut1, xp, yp, wavelength_um) ;
    if (err_code == 0) {
        // Convert from ICRS -> Galactic
        ICRS2Galactic(*glon, *glat, glon, glat,
                      CEAngleType::RADIANS) ;
    }
    
    return err_code ;
}

/**********************************************************************//**
 * Return the local sky coordinates of this object as a CECoordinates object
 * 
 * @param[in]  julian_date          Julian date of the observation
 * @param[in]  longitude            Observer longitude (radians)
 * @param[in]  latitude             Observer latitude (radians)
 * @param[in]  elevation_m          Observer elevation (meters)
 * @param[in]  pressure_hPa         Observer atmospheric pressure (hPa)
 * @param[in]  temerature_celsius   Temperature (degrees Celsius)
 * @param[in]  relative_humidity    Relative humidity
 * @param[in]  dut1                 'UTC-UT1'
 * @param[in]  xp                   x-polar motion
 * @param[in]  yp                   y-polar motion
 * @param[in]  wavelength_um        Wavelength being observed (micrometers)
 * @return These coordinates converted into the observed coordinates of the observer specified
 *************************************************************************/
CECoordinates CECoordinates::GetObservedCoords(const double& julian_date,
                                const double& longitude,
                                const double& latitude,
                                const double& elevation_m,
                                const double& pressure_hPa,
                                const double& temperature_celsius,
                                const double& relative_humidity,
                                const double& dut1,
                                const double& xp, const double& yp,
                                const double& wavelength) const
{
    // Preliminary variables
    double azimuth(0);
    double zenith(0);
    double observed1(0);
    double observed2(0);
    double observed3(0);
    
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
                      &observed1, &observed2, &observed3) ;
    } else if (coord_type_ == CECoordinateType::GALACTIC) {
        // Convert Galactic to Observed
        Galactic2Observed(xcoord_, ycoord_, &azimuth, &zenith,
                          julian_date, longitude, latitude,
                          elevation_m, pressure_hPa, temperature_celsius,
                          relative_humidity, dut1, xp, yp, wavelength,
                          &observed1, &observed2, &observed3) ;
    } else if (coord_type_ == CECoordinateType::OBSERVED) {
        azimuth = xcoord_;
        zenith  = ycoord_; 
    } else {
        std::string msg = "Unrecognized coordinate type";
        throw CEException::invalid_value("CECoordinates::GetObservedCoords", msg);
    }
    
    // Create the CECoordinates object to be returned
    return CECoordinates(azimuth, zenith, CECoordinateType::OBSERVED) ;
}

/**********************************************************************//**
 * Return the observed coordinates using an observer object (CEObserver)
 * 
 * @param[in] date                 Julian date of the observation
 * @param[in] observer             Observer information
 * @param[in] wavelength_um        Wavelength being observed (micrometers)
 * @return These coordinates converted into the observed coordinates of 'observer'
 *************************************************************************/
CECoordinates CECoordinates::GetObservedCoords(const CEDate& date,
                                               const CEObserver& observer) const
{
    return GetObservedCoords(date,
                             observer.Longitude_Rad(),
                             observer.Latitude_Rad(),
                             observer.Elevation_m(),
                             observer.Pressure_hPa(),
                             observer.Temperature_C(),
                             observer.RelativeHumidity(),
                             date.dut1(), 
                             date.xpolar(), 
                             date.ypolar(), 
                             observer.Wavelength_um());
}


/**********************************************************************//**
 * Get the angular separation between the coordinates represented
 * by this object and another coordinate object. NOTE: The coordinates
 * are both expected to be in the same coordinate system! If they
 * are in different coordinate systems, use "ConvertTo()" first.
 * 
 * @param[in] coords               Another set of coordinates
 * @param[in] return_angle_type    Specify whether to return angle as DEGREES or RADIANS
 * @return Angular separation between these coordinates and 'coords'
 *************************************************************************/
double CECoordinates::AngularSeparation(const CECoordinates& coords,
                                        const CEAngleType& return_angle_type) const
{
    return AngularSeparation(*this, coords, return_angle_type) ;
}

/**********************************************************************//**
 * Get the angular separation between two coordinate objects.
 * NOTE: The coordinates are both expected to be in the same
 * coordinate system! If they are in different coordinate systems,
 * use "ConvertTo()" first.
 * 
 * @param[in] coords1              First set of coordinates
 * @param[in] coords2              Second set of coordinates
 * @param[in] return_angle_type    Specify whether to return angle as DEGREES or RADIANS
 * @return Angular separation between two coordiantes
 * 
 * Note that the x-coordinates are expected in the range [0, 2pi] and the
 * y-coordinates are expected in the range [-pi, pi]. Because of this, you
 * need to pass:
 *  - ICRS: RA, Dec
 *  - CIRS: RA, Dec
 *  - GALACTIC: G.Lon, G.Lat
 *  - OBSERVED: Az, Alt (by default the y-coordinate is zenith)
 *************************************************************************/
double CECoordinates::AngularSeparation(const CECoordinates& coords1, 
                                        const CECoordinates& coords2,
                                        const CEAngleType& return_angle_type)
{
    // Make sure the coordinates are in the same frame
    if (coords1.GetCoordSystem() != coords2.GetCoordSystem()) {
        throw CEException::invalid_value("CECoordinates::AngularSeparation(CECoords&, CECoords&)",
                                         "Supplied coordinates are in different frames");
    }

    // Get the appropriate Y-Coordinates
    double y1 = coords1.YCoordinate_Rad();
    double y2 = coords2.YCoordinate_Rad();
    if (coords1.GetCoordSystem() == CECoordinateType::OBSERVED) {
        y1 = M_PI_2 - y1;
        y2 = M_PI_2 - y2;
    }

    // Convert the second coordinates to be the same type as the first set of coordinates
    double angsep = AngularSeparation(coords1.XCoordinate_Rad(), y1,
                                      coords2.XCoordinate_Rad(), y2,
                                      CEAngleType::RADIANS) ;
    // Convert radians to degrees if a return type of degrees is requested
    if (return_angle_type == CEAngleType::DEGREES) {
        angsep *= DR2D ;
    }
    
    return angsep ;
}

/**********************************************************************//**
 * Get the angular separation between two sets of coordinates.
 * Method utilizes the SOFA 'iauSeps' algorithm.
 * NOTE: The coordinates are both expected to be in the same
 * coordinate system!
 * 
 * @param[in] xcoord_first         X-value for first set of coordinates
 * @param[in] ycoord_first         Y-value for first set of coordinates
 * @param[in] xcoord_second        X-value for second set of coordiantes
 * @param[in] ycoord_second        Y-value for second set of coordinates
 * @param[in] return_angle_type    Specify whether input angles are DEGREES or RADIANS.
 *                                 (output angle will be in the same format)
 * @return Angular separation between two coordinates
 * 
 * Note that the x-coordinates are expected in the range [0, 2pi] and the
 * y-coordinates are expected in the range [-pi, pi]. Because of this, you
 * need to pass:
 *  - ICRS: RA, Dec
 *  - CIRS: RA, Dec
 *  - GALACTIC: G.Lon, G.Lat
 *  - OBSERVED: Az, Alt (by default the y-coordinate is zenith)
 *************************************************************************/
double CECoordinates::AngularSeparation(double xcoord_first, 
                                        double ycoord_first,
                                        double xcoord_second, 
                                        double ycoord_second,
                                        const  CEAngleType& angle_type)
{
    // Note that the 'iauSeps' algorithm expects angles in radians,
    // so we need to convert if angles were passed in degrees
    if (angle_type == CEAngleType::DEGREES) {
        // Convert the coordinates to radians
        xcoord_first *= DD2R ;
        ycoord_first *= DD2R ;
        xcoord_second *= DD2R ;
        ycoord_second *= DD2R ;
    }
    
    // Call the 'iauSeps' SOFA algorithm
    double angsep = iauSeps(xcoord_first, ycoord_first,
                            xcoord_second, ycoord_second) ;
    
    // Convert back to degrees if requested
    if (angle_type == CEAngleType::DEGREES) {
        angsep *= DR2D ;
    }
    return angsep ;
}

/**********************************************************************//**
 * Convert these coordinates to another coordinate system
 * NOTE: If this object is not OBSERVED coordinates, only JD is needed.
 * NOTE: If 'observer' is not nulltr then 'jd' will be ignored and the
 *       date will instead be derived from the observers date.
 * 
 * @param[in] output_coord_type    Output coordinate type (see CECoordinateType)
 * @param[in] date                 Julian date for conversion
 * @param[in] observer             If these coordinates are OBSERVED, then observer
 *                                 represents the observer for these coordinates.
 *                                 Otherwise they represent the observer to convert
 *                                 the coordinates to.
 * @return Coordinates object that represents coordinates we're converting to
 *************************************************************************/
CECoordinates CECoordinates::ConvertTo(CECoordinateType output_coord_type,
                                       const CEObserver& observer, 
                                       const CEDate& date)
{
    CEDate tmpdate(date);
    return ConvertTo(output_coord_type,
                     tmpdate,
                     observer.Longitude_Rad(),
                     observer.Latitude_Rad(),
                     observer.Elevation_m(),
                     observer.Pressure_hPa(),
                     observer.Temperature_C(),
                     observer.RelativeHumidity(),
                     tmpdate.dut1(),
                     tmpdate.xpolar(),
                     tmpdate.ypolar(),
                     observer.Wavelength_um()) ;
}


/**********************************************************************//**
 * Convert these coordinates to another coordinate system
 * NOTE: If neither this object or output_coord_type are OBSERVED
 *       coordinates, only JD is needed.
 * 
 * @param[in] output_coord_type    Output coordinate type (see CECoordinateType)
 * @param[in] jd                   Julian date for conversion
 * @param[in] longitude            Observer longitude (radians, east-positive)
 * @param[in] latitude             Observer latitude (radians)
 * @param[in] elevation_m          Elevation (meters above sea level)
 * @param[in] pressure_hPa         Atmospheric pressure (hPa)
 * @param[in] temperature_celsius  Atmospheric temperature (degrees celsius)
 * @param[in] relative_humidity    Relative humidity
 * @param[in] dut1                 DUT1 value representing UTC-UT1
 * @param[in] xp                   x-polar motion
 * @param[in] yp                   y-polar motion
 * @param[in] wavelength_um        wavelength (micrometers)
 * @return CECoordinates object representing CIRS right ascension, declination
 *************************************************************************/
CECoordinates CECoordinates::ConvertTo(CECoordinateType output_coord_type,
                                       double jd,
                                       double longitude,
                                       double latitude,
                                       double elevation_m,
                                       double pressure_hPa,
                                       double temperature_celsius,
                                       double relative_humidity,
                                       double dut1,
                                       double xp, double yp,
                                       double wavelength_um)
{
    // Do conversion to CIRS
    if (output_coord_type == CECoordinateType::CIRS) {
        return ConvertToCIRS(jd, longitude, latitude,
                             elevation_m, pressure_hPa,
                             temperature_celsius, relative_humidity,
                             dut1, xp, yp, wavelength_um) ;
    } else if (output_coord_type == CECoordinateType::ICRS) {
        return ConvertToICRS(jd, longitude, latitude,
                             elevation_m, pressure_hPa,
                             temperature_celsius, relative_humidity,
                             dut1, xp, yp, wavelength_um) ;
    } else if (output_coord_type == CECoordinateType::GALACTIC) {
        return ConvertToGalactic(jd, longitude, latitude,
                                 elevation_m, pressure_hPa,
                                 temperature_celsius, relative_humidity,
                                 dut1, xp, yp, wavelength_um) ;
    } else if (output_coord_type == CECoordinateType::OBSERVED) {
        return ConvertToObserved(jd, longitude, latitude,
                                 elevation_m, pressure_hPa,
                                 temperature_celsius, relative_humidity,
                                 dut1, xp, yp, wavelength_um) ;
    } else {
        std::cerr << "[ERROR] Unknown coordinate type passed to CECoordinates::ConvertTo()!\n" ;
        return CECoordinates() ;
    }
}

/**********************************************************************//**
 * Convert these coordinates to CIRS coordinates.
 * NOTE: If this object is not OBSERVED coordinates, only JD is needed.
 * 
 * @param[in] jd                   Julian date for conversion
 * @param[in] longitude            Observer longitude (radians, east-positive)
 * @param[in] latitude             Observer latitude (radians)
 * @param[in] elevation_m          Elevation (meters above sea level)
 * @param[in] pressure_hPa         Atmospheric pressure (hPa)
 * @param[in] temperature_celsius  Atmospheric temperature (degrees celsius)
 * @param[in] relative_humidity    Relative humidity
 * @param[in] dut1                 DUT1 value representing UTC-UT1
 * @param[in] xp                   x-polar motion
 * @param[in] yp                   y-polar motion
 * @param[in] wavelength_um        wavelength (micrometers)
 * @return CECoordinates object representing CIRS right ascension, declination
 *************************************************************************/
CECoordinates CECoordinates::ConvertToCIRS(double jd,
                            double longitude,
                            double latitude,
                            double elevation_m,
                            double pressure_hPa,
                            double temperature_celsius,
                            double relative_humidity,
                            double dut1,
                            double xp, double yp,
                            double wavelength_um)
{
    double xcoord_new(0.0);
    double ycoord_new(0.0) ;
    if (coord_type_ == CECoordinateType::CIRS) {
        // CIRS -> CIRS
        xcoord_new = XCoordinate_Rad() ;
        ycoord_new = YCoordinate_Rad() ;
    } else if (coord_type_ == CECoordinateType::ICRS) {
        // ICRS -> CIRS
        ICRS2CIRS(XCoordinate_Rad(), YCoordinate_Rad(),
                  &xcoord_new, &ycoord_new,
                  jd, CEAngleType::RADIANS) ;
    } else if (coord_type_ == CECoordinateType::GALACTIC) {
        // Galactic -> CIRS
        Galactic2CIRS(XCoordinate_Rad(), YCoordinate_Rad(),
                      &xcoord_new, &ycoord_new,
                      jd, CEAngleType::RADIANS) ;
    } else if (coord_type_ == CECoordinateType::OBSERVED) {
        // Observed -> CIRS
        Observed2CIRS(XCoordinate_Rad(), YCoordinate_Rad(),
                      &xcoord_new, &ycoord_new,
                      jd, longitude, latitude,
                      elevation_m, pressure_hPa,
                      temperature_celsius, relative_humidity,
                      dut1, xp, yp, wavelength_um) ;
    } else {
        std::cerr << "[ERROR] Unknown coordinate type!\n" ;
    }
    
    return CECoordinates(xcoord_new, 
                         ycoord_new,
                         CECoordinateType::CIRS,
                         CEAngleType::RADIANS) ;
}

/**********************************************************************//**
 * Convert these coordinates to ICRS coordinates.
 * NOTE: If this object is not OBSERVED coordinates, only JD is needed.
 * NOTE: If this object is GALACTIC coordinates, JD is not necessary
 * 
 * @param[in] jd                   Julian date for conversion
 * @param[in] longitude            Observer longitude (radians, east-positive)
 * @param[in] latitude             Observer latitude (radians)
 * @param[in] elevation_m          Elevation (meters above sea level)
 * @param[in] pressure_hPa         Atmospheric pressure (hPa)
 * @param[in] temperature_celsius  Atmospheric temperature (degrees celsius)
 * @param[in] relative_humidity    Relative humidity
 * @param[in] dut1                 DUT1 value representing UTC-UT1
 * @param[in] xp                   x-polar motion
 * @param[in] yp                   y-polar motion
 * @param[in] wavelength_um        wavelength (micrometers)
 * @return CECoordinates object representing ICRS right ascension, declination
 *************************************************************************/
CECoordinates CECoordinates::ConvertToICRS(double jd,
                            double longitude,
                            double latitude,
                            double elevation_m,
                            double pressure_hPa,
                            double temperature_celsius,
                            double relative_humidity,
                            double dut1,
                            double xp, double yp,
                            double wavelength_um)
{
    double xcoord_new(0.0);
    double ycoord_new(0.0);
    if (coord_type_ == CECoordinateType::CIRS) {
        CIRS2ICRS(XCoordinate_Rad(), YCoordinate_Rad(),
                  &xcoord_new, &ycoord_new, jd, CEAngleType::RADIANS) ;
    } else if (coord_type_ == CECoordinateType::ICRS) {
        xcoord_new = XCoordinate_Rad() ;
        ycoord_new = YCoordinate_Rad() ;
    } else if (coord_type_ == CECoordinateType::GALACTIC) {
        Galactic2ICRS(XCoordinate_Rad(), YCoordinate_Rad(),
                      &xcoord_new, &ycoord_new, CEAngleType::RADIANS) ;
    } else if (coord_type_ == CECoordinateType::OBSERVED) {
        Observed2ICRS(XCoordinate_Rad(), YCoordinate_Rad(),
                      &xcoord_new, &ycoord_new,
                      jd, longitude, latitude,
                      elevation_m, pressure_hPa,
                      temperature_celsius, relative_humidity,
                      dut1, xp, yp, wavelength_um) ;
    } else {
        std::cerr << "[ERROR] Unknown coordinate type!\n" ;
    }
    
    return CECoordinates(xcoord_new, ycoord_new,
                         CECoordinateType::ICRS,
                         CEAngleType::RADIANS) ;
}

/**********************************************************************//**
 * Convert these coordinates to GALACTIC coordinates.
 * NOTE: If this object is not OBSERVED coordinates, only JD is needed.
 * NOTE: If this object is ICRS coordinates, JD is not necessary
 * 
 * @param[in] jd                   Julian date for conversion
 * @param[in] longitude            Observer longitude (radians, east-positive)
 * @param[in] latitude             Observer latitude (radians)
 * @param[in] elevation_m          Elevation (meters above sea level)
 * @param[in] pressure_hPa         Atmospheric pressure (hPa)
 * @param[in] temperature_celsius  Atmospheric temperature (degrees celsius)
 * @param[in] relative_humidity    Relative humidity
 * @param[in] dut1                 DUT1 value representing UTC-UT1
 * @param[in] xp                   x-polar motion
 * @param[in] yp                   y-polar motion
 * @param[in] wavelength_um        wavelength (micrometers)
 * @return CECoordinates object representing galactic longitude,latitude
 *************************************************************************/
CECoordinates CECoordinates::ConvertToGalactic(double jd,
                                double longitude,
                                double latitude,
                                double elevation_m,
                                double pressure_hPa,
                                double temperature_celsius,
                                double relative_humidity,
                                double dut1,
                                double xp, double yp,
                                double wavelength_um)
{
    double xcoord_new(0.0);
    double ycoord_new(0.0);
    if (coord_type_ == CECoordinateType::CIRS) {
        CIRS2Galactic(XCoordinate_Rad(), YCoordinate_Rad(),
                      &xcoord_new, &ycoord_new, jd, CEAngleType::RADIANS) ;
    } else if (coord_type_ == CECoordinateType::ICRS) {
        ICRS2Galactic(XCoordinate_Rad(), YCoordinate_Rad(),
                      &xcoord_new, &ycoord_new, CEAngleType::RADIANS) ;
    } else if (coord_type_ == CECoordinateType::GALACTIC) {
        xcoord_new = XCoordinate_Rad() ;
        ycoord_new = YCoordinate_Rad() ;
    } else if (coord_type_ == CECoordinateType::OBSERVED) {
        Observed2Galactic(XCoordinate_Rad(), YCoordinate_Rad(),
                          &xcoord_new, &ycoord_new,
                          jd, longitude, latitude,
                          elevation_m, pressure_hPa,
                          temperature_celsius, relative_humidity,
                          dut1, xp, yp, wavelength_um) ;
    } else {
        std::cerr << "[ERROR] Unknown coordinate type!\n" ;
    }
    
    return CECoordinates(xcoord_new, ycoord_new, CECoordinateType::GALACTIC,
                         CEAngleType::RADIANS) ;
}

/**********************************************************************//**
 * Convert these coordinates to observed coordinates.
 * Note: For accurate positions you MUST supply at least jd, longitude, and latitude
 * @param[in] jd                   Julian date for conversion
 * @param[in] longitude            Observer longitude (radians, east-positive)
 * @param[in] latitude             Observer latitude (radians)
 * @param[in] elevation_m          Elevation (meters above sea level)
 * @param[in] pressure_hPa         Atmospheric pressure (hPa)
 * @param[in] temperature_celsius  Atmospheric temperature (degrees celsius)
 * @param[in] relative_humidity    Relative humidity
 * @param[in] dut1                 DUT1 value representing UTC-UT1
 * @param[in] xp                   x-polar motion
 * @param[in] yp                   y-polar motion
 * @param[in] wavelength_um        wavelength (micrometers)
 * @return CECoordinates object representing the observed altitude,
 *             zenith angle of these coordinates.
 *************************************************************************/
CECoordinates CECoordinates::ConvertToObserved(double jd,
                                double longitude,
                                double latitude,
                                double elevation_m,
                                double pressure_hPa,
                                double temperature_celsius,
                                double relative_humidity,
                                double dut1,
                                double xp, double yp,
                                double wavelength_um)
{
    double xcoord_new(0.0);
    double ycoord_new(0.0);
    double apparent_x, apparent_y, apparent_hourangle;
    if (coord_type_ == CECoordinateType::CIRS) {
        CIRS2Observed(XCoordinate_Rad(), YCoordinate_Rad(),
                      &xcoord_new, &ycoord_new,
                      jd,
                      longitude, latitude,
                      elevation_m, pressure_hPa,
                      temperature_celsius,
                      relative_humidity,
                      dut1, xp, yp,
                      wavelength_um,
                      &apparent_x, &apparent_y, &apparent_hourangle) ;
    } else if (coord_type_ == CECoordinateType::ICRS) {
        ICRS2Observed(XCoordinate_Rad(), YCoordinate_Rad(),
                      &xcoord_new, &ycoord_new,
                      jd,
                      longitude, latitude,
                      elevation_m, pressure_hPa,
                      temperature_celsius,
                      relative_humidity,
                      dut1, xp, yp,
                      wavelength_um,
                      &apparent_x, &apparent_y, &apparent_hourangle) ;
    } else if (coord_type_ == CECoordinateType::GALACTIC) {
        Galactic2Observed(XCoordinate_Rad(), YCoordinate_Rad(),
                      &xcoord_new, &ycoord_new,
                      jd,
                      longitude, latitude,
                      elevation_m, pressure_hPa,
                      temperature_celsius,
                      relative_humidity,
                      dut1, xp, yp,
                      wavelength_um,
                      &apparent_x, &apparent_y, &apparent_hourangle) ;
    } else if (coord_type_ == CECoordinateType::OBSERVED) {
        xcoord_new = XCoordinate_Rad() ;
        ycoord_new = YCoordinate_Rad() ;
    } else {
        std::cerr << "[ERROR] Unknown coordinate type!\n" ;
    }
 
    return CECoordinates(xcoord_new, ycoord_new, CECoordinateType::OBSERVED,
                         CEAngleType::RADIANS) ;
}

/**********************************************************************//**
 * Convert a given angle into degrees, arcminutes, arcseconds.
 * @param[in] angle            What is the angle value
 * @param[in] angle_type       Specifies what type of angle
 * @return Vector containing the following values
 *         -[0] = Degrees
 *         -[1] = Arcminutes
 *         -[2] = Arcseconds
 *         -[3] = Arcsec fraction
 *************************************************************************/
std::vector<double> CECoordinates::GetDMS(const double& angle,
                                          const CEAngleType& angle_type)
{
    // Convert to degrees if passed radians
    double ang = angle;
    if (angle_type == CEAngleType::DEGREES) 
        ang *= DD2R ;

    // Variables for holding the results
    char sign;
    std::vector<int> DMS_i(4, 0.0);

    // Run the appropriate SOFA value
    iauA2af(9, ang, &sign, &DMS_i[0]);

    // Fill the values into a double vector
    std::vector<double> DMS_d(4, 0.0);
    DMS_d[0] = DMS_i[0] * (sign == '-' ? -1.0 : 1.0);
    DMS_d[1] = DMS_i[1];
    DMS_d[2] = DMS_i[2];
    DMS_d[3] = DMS_i[3] * 1.0e-9;

    return DMS_d ;
}

/**********************************************************************//**
 * Convert a given angle into hours, minutes, seconds.
 * This method is almost exclusively only useful for right ascension
 * @param[in] angle            What is the angle value
 * @param[in] angle_type       Specifies what type of angle
 * @return Vector containing the following values
 *         -[0] = Hours
 *         -[1] = Minutes
 *         -[2] = Seconds
 *         -[3] = Fractions of a second
 *************************************************************************/
std::vector<double> CECoordinates::GetHMS(const double& angle,
                                          const CEAngleType& angle_type)
{
    // Convert to degrees if passed radians
    double ang(angle);

    if (angle_type == CEAngleType::DEGREES) 
        ang *= DD2R ;
    
    // Stores the resulting values
    std::vector<int> HMS_i(4);
    // Stores the sign
    char pm;
    iauA2tf(9, ang, &pm, &HMS_i[0]);
    
    // Convert the values into doubles
    std::vector<double> HMS_d(HMS_i.size());
    // Scale the fraction to be a true fraction
    HMS_d[0] = double(HMS_i[0]);
    HMS_d[1] = double(HMS_i[1]);
    HMS_d[2] = double(HMS_i[2]);
    HMS_d[3] = double(HMS_i[3])/1.0e9;
    // Multiply the degree by a negative
    if (pm == '-') HMS_d[0] *= -1;

    return HMS_d;
}


/**********************************************************************//**
 * Convert from {hours, minutes, seconds} to an angle
 * @param[in] angle            What is the angle value
 * @param[in] return_type      Specifies angle type for returned value
 * @return Angle
 * 
 * The supplied @p angle value should be of length 3 with the indices having
 * the following values
 *         -[0] = Hours
 *         -[1] = Minutes
 *         -[2] = Seconds
 *         -[3] = Fraction of a second (not absolutely necessary)
 *************************************************************************/
double CECoordinates::HMSToAngle(const std::vector<double>& angle,
                                 const CEAngleType& return_type)
{
    // Get the angle sign
    char pm = (angle[0] >= 0.0) ? '+' : '-';

    // Do the conversion
    double ang(0.0);
    double sec = (angle.size() == 4) ? angle[2]+angle[3] : angle[2];
    int err = iauTf2a(pm, std::abs(angle[0]), int(angle[1]), sec, &ang);

    // Handle the error
    if (err != 0) {
        std::string msg = "[ERROR] ";
        switch(err) {
            // Hours out of bounds
            case 1:
                msg += "Hour value \'" + std::to_string(angle[0]) + "\' not in range 0-23";
                throw CEException::invalid_value("CECoordinates::HMSToAngle", msg);
            // Minutes out of bounds
            case 2:
                msg += "Minutes value \'" + std::to_string(angle[1]) + "\' not in range 0-59";
                throw CEException::invalid_value("CECoordinates::HMSToAngle", msg);
            // Seconds out of bounds
            case 3:
                msg += "Seconds value \'" + std::to_string(sec) + "\' not in range 0-59.999...";
                throw CEException::invalid_value("CECoordinates::HMSToAngle", msg);
        }
    }

    // Convert to appropriate return_type
    if (return_type == CEAngleType::DEGREES) ang *= DR2D;

    return ang;
}


/**********************************************************************//**
 * Convert a given angle vector from {degrees, minutes, seconds} to an angle
 * @param[in] angle            What is the angle value
 * @param[in] return_type      Specifies angle type for returned value
 * @return Angle
 * 
 * The supplied @p angle value should be of length 3 with the indices having
 * the following values
 *         -[0] = Degrees
 *         -[1] = Arcminutes
 *         -[2] = Arcseconds
 *         -[3] = Arcsec fraction (can be omitted if [2] is the decimal value)
 *************************************************************************/
double CECoordinates::DMSToAngle(const std::vector<double>& angle,
                                 const CEAngleType& return_type)
{
    // Convert the values to an angle in degrees
    double ret_angle;
    char   sign = (angle[0] < 0.0) ? '-' : '+';
    double sec  = (angle.size() == 3) ? angle[2] : angle[2]+angle[3];

    // Pass the values to the appropriate SOFA algorithm
    iauAf2a(sign, int(angle[0]), int(angle[1]), sec, &ret_angle);
    
    if (return_type == CEAngleType::DEGREES) {
        ret_angle *= DR2D;
    }

    return ret_angle;
}


/**********************************************************************//**
 * Set the coordinates of this object
 * @param[in] xcoord           X-coordinate
 * @param[in] ycoord           Y-coordinate
 * @param[in] coord_type       Coordinate type (see ::CECoordinateType)
 * @param[in] angle_type       Specifies whether xcoord & ycoord are in RADIANS or DEGREES (see ::CEAngleType)
 *************************************************************************/
void CECoordinates::SetCoordinates(const double& xcoord, 
                                   const double& ycoord,
                                   const CECoordinateType& coord_type,
                                   const CEAngleType& angle_type)
{
    if (angle_type == CEAngleType::RADIANS) {
        xcoord_ = xcoord ;
        ycoord_ = ycoord ;
    } else {
        xcoord_ = xcoord * DD2R ;
        ycoord_ = ycoord * DD2R ;
    }
    coord_type_ = coord_type ;
}

/**********************************************************************//**
 * Set the coordinates from another CECoordinates object
 * @param[in] coords       Another coordinates object to copy
 *************************************************************************/
void CECoordinates::SetCoordinates(const CECoordinates& coords)
{
    copy_members(coords);
}


/**********************************************************************//**
 * Generate a message string that specifies the information about this coordinate
 * @return String describing this object
 *************************************************************************/
std::string CECoordinates::print(void) const
{
    std::string msg = "Coordinates:\n";
    msg += "   - System : " + std::to_string(int(coord_type_)) + "\n";
    msg += "   - X-coord: " + std::to_string(XCoordinate_Deg()) + " deg\n";
    msg += "   - Y-coord: " + std::to_string(YCoordinate_Deg()) + " deg\n";
    return msg;
}


/**********************************************************************//**
 * Copy data members from another CECoordinates object
 * @param[in] other         Another coordinates objec to copy
 *************************************************************************/
void CECoordinates::copy_members(const CECoordinates& other)
{
    coord_type_ = other.coord_type_;
    xcoord_     = other.xcoord_;
    ycoord_     = other.ycoord_;
}


/**********************************************************************//**
 * Cleanup data members that need to be freed or cleared
 *************************************************************************/
void CECoordinates::free_members(void)
{
    // Nothing to do here
}


/**********************************************************************//**
 * Set initial values and allocate memory for data members
 *************************************************************************/
void CECoordinates::init_members(void)
{
    coord_type_ = CECoordinateType::ICRS;
    xcoord_     = 0.0;
    ycoord_     = 0.0;
}


/**********************************************************************//**
 * Compare two coordinate objects
 *  @return True if the two coordinates are equal to each other
 *************************************************************************/
bool operator==(const CECoordinates& lhs, const CECoordinates& rhs)
{
    bool are_equal = true;
    
    // Check the coordinate systems are equivalent
    if (lhs.GetCoordSystem() != rhs.GetCoordSystem()) {
        are_equal = false;
    }
    // Check that the x-coordinate and the y-coordinate are the same
    else {
        // Check how far appart the coordinates are from each other
        double angsep(CECoordinates::AngularSeparation(lhs, rhs, CEAngleType::RADIANS));
        // Currently require separation < 0.03 arcsec
        double marcsec_rad = 4.848e-6;
        if (angsep > 3.0*marcsec_rad) {
            are_equal = false;
        }
    }

    return are_equal;
}


/**********************************************************************//**
 * Compare two coordinate objects
 *  @return True if two coordinates are NOT equal to each other
 *************************************************************************/
bool operator!=(const CECoordinates& lhs, const CECoordinates& rhs)
{
    return !(lhs == rhs);
}

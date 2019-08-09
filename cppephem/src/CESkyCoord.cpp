/***************************************************************************
 *  CESkyCoord.cpp: CppEphem                                               *
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

/** \class CESkyCoord
 CESkyCoord class is responsible for doing all of the celestial coordinate
 conversions. Command line tools are also available for "on the fly" conversions.
 
 The different coordinate systems used:
 - CIRS: Earth-centric coordinate systems
 - ICRS: Solar system barycentric coordinate system
 - Galactic: Galactic coordinates
 - Observed: Observer specific coordinate system
 */

#include "CESkyCoord.h"

/**********************************************************************//**
 * Default constructor
 *************************************************************************/
CESkyCoord::CESkyCoord()
{
    init_members();
}


/**********************************************************************//**
 * Primary constructor
 * 
 * @param[in] xcoord X-Coordinate
 * @param[in] ycoord Y-Coordinate
 * @param[in] coord_type Coordinate type (see CESkyCoordType)
 *************************************************************************/
CESkyCoord::CESkyCoord(const CEAngle&        xcoord, 
                       const CEAngle&        ycoord,
                       const CESkyCoordType& coord_type)
{
    init_members();

    // Setup the coordinates
    xcoord_     = xcoord;
    ycoord_     = ycoord;
    coord_type_ = coord_type;
}


/**********************************************************************//**
 * Copy constructor
 *************************************************************************/
CESkyCoord::CESkyCoord(const CECoordinates& other)
{
    // Set coordinates
    xcoord_ = other.XCoord();
    ycoord_ = other.YCoord();
    coord_type_ = CESkyCoordType(int(other.GetCoordSystem()));
}


/**********************************************************************//**
 * Copy constructor
 *************************************************************************/
CESkyCoord::CESkyCoord(const CESkyCoord& other)
{
    free_members();
    init_members();
    copy_members(other);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
CESkyCoord::~CESkyCoord()
{
    free_members();
}


/**********************************************************************//**
 * Overloaded '=' (assignment) operator
 *************************************************************************/
CESkyCoord& CESkyCoord::operator=(const CESkyCoord& other)
{
    if (this != &other) {
        free_members();
        init_members();
        copy_members(other);
    }
    return *this;
}


/**********************************************************************//**
 * Get the angular separation between the coordinates represented
 * by this object and another coordinate object. NOTE: The coordinates
 * are both expected to be in the same coordinate system! If they
 * are in different coordinate systems, use "ConvertTo()" first.
 * 
 * @param[in] coords               Another set of coordinates
 * @return Angular separation between these coordinates and 'coords'
 *************************************************************************/
CEAngle CESkyCoord::AngularSeparation(const CESkyCoord& coords) const
{
    return AngularSeparation(*this, coords);
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
 * y-coordinates are expected in the range [-pi, pi]. Because of this, OBSERVED
 * coordinates first convert the zenith angle to altitude
 *************************************************************************/
CEAngle CESkyCoord::AngularSeparation(const CESkyCoord& coords1, 
                                      const CESkyCoord& coords2)
{
    // Make sure the coordinates are in the same frame
    if (coords1.GetCoordSystem() != coords2.GetCoordSystem()) {
        throw CEException::invalid_value("CESkyCoord::AngularSeparation(CESkyCoord&, CESkyCoord&)",
                                         "Supplied coordinates are in different frames");
    }

    // Get the appropriate Y-Coordinates
    double y1 = coords1.YCoord().Rad();
    double y2 = coords2.YCoord().Rad();
    if (coords1.GetCoordSystem() == CESkyCoordType::OBSERVED) {
        y1 = M_PI_2 - y1;
        y2 = M_PI_2 - y2;
    }

    // Convert the second coordinates to be the same type as the first set of coordinates
    return AngularSeparation(coords1.XCoord(), y1,
                             coords2.XCoord(), y2);
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
CEAngle CESkyCoord::AngularSeparation(const CEAngle& xcoord_first, 
                                      const CEAngle& ycoord_first,
                                      const CEAngle& xcoord_second, 
                                      const CEAngle& ycoord_second)
{
    // Call the 'iauSeps' SOFA algorithm
    return CEAngle::Rad(iauSeps(xcoord_first, ycoord_first,
                                xcoord_second, ycoord_second));
}


/**********************************************************************//**
 * Convert CIRS to ICRS coordinates
 * 
 * @param[in]  in_cirs          Input CIRS coordinates
 * @param[out] out_icrs         Output ICRS coordinates
 * @param[in]  date             Date for conversion
 *************************************************************************/
void CESkyCoord::CIRS2ICRS(const CESkyCoord& in_cirs, 
                           CESkyCoord*       out_icrs,
                           const CEDate&     date)
{
    double eo; // Equation of the origins

    // Compute the TDB date
    double tdb1(0.0);
    double tdb2(0.0);
    CEDate::UTC2TDB(date.MJD(), &tdb1, &tdb2);

    // Use the SOFA library to compute the new date
    double return_ra(0.0);
    double return_dec(0.0);
    iauAtic13(in_cirs.XCoord(), in_cirs.YCoord(), 
              tdb1, tdb2, 
              &return_ra, &return_dec, &eo);

    // Subtract the eo from RA if J2000 coordinates are desired
    //*return_ra -= eo;

    out_icrs->SetCoordinates(CEAngle(return_ra), CEAngle(return_dec),
                             CESkyCoordType::ICRS);
    return;
}


/**********************************************************************//**
 * Convert CIRS to Galactic coordinates
 * 
 * @param[in]  in_cirs          Input CIRS coordinates
 * @param[out] out_galactic     Output Galactic coordinates
 * @param[in]  date             Date for conversion
 *************************************************************************/
void CESkyCoord::CIRS2Galactic(const CESkyCoord& in_cirs,
                               CESkyCoord*       out_galactic,
                               const CEDate&     date)
{
    // In order to do this with the sofa package, we must first
    // convert from CIRS -> ICRS
    CIRS2ICRS(in_cirs, out_galactic, date);
    
    // Now we can convert to galactic
    ICRS2Galactic(*out_galactic, out_galactic);
}


/**********************************************************************//**
 * CIRS -> Observed (or observer specific) coordinate conversion
 * 
 * @param[in]  in_cirs          Input CIRS coordinates
 * @param[out] out_observed     Output Observed coordinates
 * @param[in]  date             Date for conversion
 * @param[in]  observer         Observer information
 * @param[out] observed_cirs    'Observed' CIRS coordinates
 * @param[out] hour_angle       Hour angle of coordinates for observer
 *************************************************************************/
void CESkyCoord::CIRS2Observed(const CESkyCoord& in_cirs,
                               CESkyCoord*       out_observed,
                               const CEDate&     date,
                               const CEObserver& observer,
                               CESkyCoord*       observed_cirs,
                               CEAngle*          hour_angle)
{
    // Setup the observed RA, Dec and hour_angle variables
    double temp_ra(0.0);
    double temp_dec(0.0);
    double temp_hour_angle(0.0);

    // Call the necessary sofa method
    double az(0.0);
    double zen(0.0);
    int err_code = iauAtio13(in_cirs.XCoord(), 
                             in_cirs.YCoord(),
                             CEDate::GetMJD2JDFactor(), date.MJD(),
                             date.dut1(),
                             observer.Longitude_Rad(),
                             observer.Latitude_Rad(),
                             observer.Elevation_m(),
                             date.xpolar(), date.ypolar(),
                             observer.Pressure_hPa(),
                             observer.Temperature_C(),
                             observer.RelativeHumidity(),
                             observer.Wavelength_um(),
                             &az, &zen,
                             &temp_hour_angle, 
                             &temp_ra, &temp_dec);
    
    // Handle the error code
    if (err_code == -1) {
        throw CEException::sofa_error("CESkyCoord::CIRS2Observed",
                                      "iauAtio13", -1,
                                      "SOFA method was passed an unacceptable date");
    }

    // Set the output coordinates
    out_observed->SetCoordinates(CEAngle::Rad(az), CEAngle::Rad(zen), 
                                 CESkyCoordType::OBSERVED);

    // If passed, set the observed CIRS coordinates
    if (observed_cirs != nullptr) {
        observed_cirs->SetCoordinates(CEAngle::Rad(temp_ra), CEAngle::Rad(temp_dec),
                                      CESkyCoordType::CIRS);
    }

    // Set the hour angle
    if (hour_angle != nullptr) {
        hour_angle->SetAngle(temp_hour_angle, CEAngleType::RADIANS);
    }

    return;
}


/**********************************************************************//**
 * Convert CIRS to ECLIPTIC coordinates
 * 
 * @param[in]  in_cirs          Input CIRS coordinates
 * @param[out] out_ecliptic     Output ECLIPTIC coordinates
 * @param[in]  date             Date for conversion
 *************************************************************************/
void CESkyCoord::CIRS2Galactic(const CESkyCoord& in_cirs,
                               CESkyCoord*       out_ecliptic,
                               const CEDate&     date)
{
    // CIRS -> ICRS
    CESkyCoord tmp_icrs;
    CIRS2ICRS(in_cirs, &tmp_icrs, date);
    
    // ICRS -> ECLIPTIC
    ICRS2Ecliptic(tmp_icrs, out_ecliptic, date);
}


/**********************************************************************//**
 * ICRS -> CIRS coordinate conversion
 * 
 * @param[in]  in_icrs          Input ICRS coordinates
 * @param[out] out_cirs         Output CIRS coordinates
 * @param[in]  date             Date for conversion
 *************************************************************************/
void CESkyCoord::ICRS2CIRS(const CESkyCoord& in_icrs,
                           CESkyCoord*       out_cirs,
                           const CEDate&     date)
{
    // Store the equation of the origins
    double eo; // Equation of the origins
    
    // Use the sofa library to convert these coordinates
    double tdb1(0.0);
    double tdb2(0.0);
    CEDate::UTC2TDB(date.MJD(), &tdb1, &tdb2);
    double return_ra(0.0);
    double return_dec(0.0);
    iauAtci13(in_icrs.XCoord().Rad(), 
                in_icrs.YCoord().Rad(),
                0.0, 0.0, 0.0, 0.0, 
                tdb1, tdb2, 
                &return_ra, &return_dec, &eo);
    
    // Subtract the equation of the origins if J2000 coordinates are desired
    //*return_ra -= eo;

    // Set the output cirs coordinates
    out_cirs->SetCoordinates(CEAngle::Rad(return_ra),
                             CEAngle::Rad(return_dec),
                             CESkyCoordType::CIRS);

    return;
}


/**********************************************************************//**
 * ICRS -> GALACTIC coordinate conversion
 * 
 * @param[in]  in_icrs          Input ICRS coordinates
 * @param[out] out_galactic     Output GALACTIC coordinates
 *************************************************************************/
void CESkyCoord::ICRS2Galactic(const CESkyCoord& in_icrs,
                               CESkyCoord*       out_galactic)
{
    // Use the sofa method to convert the coordinates
    double glon(0.0);
    double glat(0.0);
    iauIcrs2g(in_icrs.XCoord().Rad(), in_icrs.YCoord().Rad(), &glon, &glat);
    out_galactic->SetCoordinates(CEAngle::Rad(glon), 
                                 CEAngle::Rad(glat),
                                 CESkyCoordType::GALACTIC);

    return;
}


/**********************************************************************//**
 * ICRS -> OBSERVED coordinate conversion
 * 
 * @param[in]  in_icrs          Input ICRS coordinates
 * @param[out] out_observed     Output OBSERVED coordinates
 * @param[in]  date             Date for conversion
 * @param[in]  observer         Observer information
 * @param[out] observed_cirs    'Observed' CIRS coordinates
 * @param[out] hour_angle       Hour angle of coordinates for observer
 * 
 * This method takes in an optional @p observed_cirs parameter that stores
 * the CIRS coordinates of a given object as they would be observed
 *************************************************************************/
void CESkyCoord::ICRS2Observed(const CESkyCoord& in_icrs,
                               CESkyCoord*       out_observed,
                               const CEDate&     date,
                               const CEObserver& observer,
                               CESkyCoord*       observed_icrs,
                               CEAngle*          hour_angle)
{
    // First convert the ICRS coordinates to CIRS coordinates
    CESkyCoord tmp_cirs;
    ICRS2CIRS(in_icrs, &tmp_cirs, date);

    // Now convert CIRS to Observed
    CESkyCoord obs_cirs;
    CIRS2Observed(tmp_cirs, out_observed,
                  date, observer, 
                  &obs_cirs, hour_angle);
    
    // Convert the apparent CIRS RA,Dec to ICRS RA,Dec
    if (observed_icrs != nullptr) {
        CIRS2ICRS(obs_cirs, observed_icrs, date);
    }

    return;
}


/**********************************************************************//**
 * ICRS -> ECLIPTIC coordinate conversion
 * 
 * @param[in]  in_icrs          Input ICRS coordinates
 * @param[out] out_ecliptic     Output ECLIPTIC coordinates
 * @param[in]  date             Date for conversion
 *************************************************************************/
void CESkyCoord::ICRS2Ecliptic(const CESkyCoord& in_icrs,
                               CESkyCoord*       out_ecliptic,
                               const CEDate&     date)
{
    // Use the sofa method to convert the coordinates
    double elon(0.0);
    double elat(0.0);

    // Get the time in TT
    double tt1(0.0);
    double tt2(0.0);
    CEDate::UTC2TT(date.MJD(), &tt1, &tt2);

    // Convert ICRS -> ECLIPTIC
    iauEqec06(tt1, tt2, in_icrs.XCoord().Rad(), in_icrs.YCoord().Rad(), 
              &elon, &elat);
    out_ecliptic->SetCoordinates(CEAngle::Rad(elon), 
                                 CEAngle::Rad(elat),
                                 CESkyCoordType::ECLIPTIC);

    return;
}


/**********************************************************************//**
 * GALACTIC -> CIRS coordinate conversion
 * 
 * @param[in]  in_galactic      Input GALACTIC coordinates
 * @param[out] out_cirs         Output CIRS coordinates
 * @param[in]  date             Date for conversion
 *************************************************************************/
void CESkyCoord::Galactic2CIRS(const CESkyCoord& in_galactic,
                               CESkyCoord*       out_cirs,
                               const CEDate&     date)
{
    // Do the Galactic -> ICRS converstion
    CESkyCoord tmp_icrs;
    Galactic2ICRS(in_galactic, &tmp_icrs);
    
    // Now convert ICRS -> CIRS
    ICRS2CIRS(tmp_icrs, out_cirs, date);
    
    return;
}


/**********************************************************************//**
 * GALACTIC -> ICRS coordinate conversion
 * 
 * @param[in]  in_galactic      Input GALACTIC coordinates
 * @param[out] out_icrs         Output ICRS coordinates
 *************************************************************************/
void CESkyCoord::Galactic2ICRS(const CESkyCoord& in_galactic,
                               CESkyCoord*       out_icrs)
{
    // Do the Galactic -> ICRS converstion
    double ra(0.0);
    double dec(0.0);
    iauG2icrs(in_galactic.XCoord().Rad(), 
              in_galactic.YCoord().Rad(), 
              &ra, &dec);
    out_icrs->SetCoordinates(ra, dec, CESkyCoordType::ICRS);
    
    return;
}


/**********************************************************************//**
 * GALACTIC -> OBSERVED coordinate conversion
 * 
 * @param[in]  in_galactic       Input GALACTIC coordinates
 * @param[out] out_observed      Output OBSERVED coordinates
 * @param[in]  date              Date for conversion
 * @param[in]  observer          Observer information
 * @param[out] observed_galactic 'Observed' GALACTIC coordinates
 * @param[out] hour_angle        Hour angle of coordinates for observer
 * 
 * This method takes in an optional @p observed_galactic parameter that stores
 * the GALACTIC coordinates of a given object as they would be observed
 *************************************************************************/
void CESkyCoord::Galactic2Observed(const CESkyCoord& in_galactic,
                                   CESkyCoord*       out_observed,
                                   const CEDate&     date,
                                   const CEObserver& observer,
                                   CESkyCoord*       observed_galactic,
                                   CEAngle*          hour_angle)
{
    // Galactic -> CIRS
    CESkyCoord tmp_cirs;
    Galactic2CIRS(in_galactic, &tmp_cirs, date);

    // CIRS -> OBSERVED
    CESkyCoord obs_cirs;
    CIRS2Observed(tmp_cirs, out_observed, date, 
                  observer, &obs_cirs, hour_angle);

    // Observed CIRS -> observed Galactic
    if (observed_galactic != nullptr) {
        CIRS2Galactic(obs_cirs, observed_galactic, date);
    }

    return;
}


/**********************************************************************//**
 * GALACTIC -> ECLIPTIC coordinate conversion
 * 
 * @param[in]  in_galactic       Input GALACTIC coordinates
 * @param[out] out_ecliptic      Output ECLIPTIC coordinates
 * @param[in]  date              Date for conversion
 *************************************************************************/
void CESkyCoord::Galactic2Ecliptic(const CESkyCoord& in_galactic,
                                   CESkyCoord*       out_ecliptic,
                                   const CEDate&     date)
{
    // Galactic -> ICRS
    CESkyCoord tmp_icrs;
    Galactic2ICRS(in_galactic, &tmp_icrs);

    // ICRS -> ECLIPTIC
    ICRS2Ecliptic(tmp_icrs, out_ecliptic, date);
}


/**********************************************************************//**
 * OBSERVED -> CIRS coordinate conversion
 * 
 * @param[in]  in_observed       Input OBSERVED coordinates
 * @param[out] out_cirs          Output CIRS coordinates
 * @param[in]  date              Date for conversion
 * @param[in]  observer          Observer information
 *************************************************************************/
void CESkyCoord::Observed2CIRS(const CESkyCoord& in_observed,
                               CESkyCoord*       out_cirs,
                               const CEDate&     date,
                               const CEObserver& observer)
{
    // Preliminary coordinates
    double ra(0.0);
    double dec(0.0);

    // Run the SOFA method
    int err_code = iauAtoi13("A", 
                             in_observed.XCoord().Rad(), 
                             in_observed.YCoord().Rad(),
                             CEDate::GetMJD2JDFactor(), date.MJD(),
                             date.dut1(),
                             observer.Longitude_Rad(), 
                             observer.Latitude_Rad(),
                             observer.Elevation_m(),
                             date.xpolar(), date.ypolar(),
                             observer.Pressure_hPa(),
                             observer.Temperature_C(),
                             observer.RelativeHumidity(),
                             observer.Wavelength_um(),
                             &ra, &dec);

    // Handle unacceptable date
    if (err_code == -1) {
        throw CEException::sofa_error("CESkyCoord::CIRS2Observed",
                                      "iauAtio13", -1,
                                      "SOFA method was passed an unacceptable date");
    }

    // Set ICRS coordinates
    out_cirs->SetCoordinates(CEAngle::Rad(ra), CEAngle::Rad(dec),
                             CESkyCoordType::CIRS);

    return;
}


/**********************************************************************//**
 * OBSERVED -> ICRS coordinate conversion
 * 
 * @param[in]  in_observed       Input OBSERVED coordinates
 * @param[out] out_icrs          Output ICRS coordinates
 * @param[in]  date              Date for conversion
 * @param[in]  observer          Observer information
 *************************************************************************/
void CESkyCoord::Observed2ICRS(const CESkyCoord& in_observed,
                               CESkyCoord*       out_icrs,
                               const CEDate&     date,
                               const CEObserver& observer)
{
    // Convert from Observed -> CIRS
    CESkyCoord tmp_cirs;
    Observed2CIRS(in_observed, &tmp_cirs, date, observer);

    // Convert from CIRS -> ICRS
    CIRS2ICRS(tmp_cirs, out_icrs, date);
}


/**********************************************************************//**
 * OBSERVED -> GALACTIC coordinate conversion
 * 
 * @param[in]  in_observed       Input OBSERVED coordinates
 * @param[out] out_galactic      Output GALACTIC coordinates
 * @param[in]  date              Date for conversion
 * @param[in]  observer          Observer information
 *************************************************************************/
void CESkyCoord::Observed2Galactic(const CESkyCoord& in_observed,
                                   CESkyCoord*       out_galactic,
                                   const CEDate&     date,
                                   const CEObserver& observer)
{
    // Convert from Observed -> ICRS
    CESkyCoord tmp_icrs;
    Observed2ICRS(in_observed, &tmp_icrs, date, observer);

    // Convert from ICRS -> Galactic
    ICRS2Galactic(tmp_icrs, out_galactic);
}


/**********************************************************************//**
 * OBSERVED -> ECLIPTIC coordinate conversion
 * 
 * @param[in]  in_observed       Input OBSERVED coordinates
 * @param[out] out_ecliptic      Output ECLIPTIC coordinates
 * @param[in]  date              Date for conversion
 * @param[in]  observer          Observer information
 *************************************************************************/
void CESkyCoord::Observed2Ecliptic(const CESkyCoord& in_observed,
                                   CESkyCoord*       out_ecliptic,
                                   const CEDate&     date,
                                   const CEObserver& observer)
{
    // Convert from Observed -> ICRS
    CESkyCoord tmp_icrs;
    Observed2ICRS(in_observed, &tmp_icrs, date, observer);

    // Convert from ICRS -> ECLIPTIC
    ICRS2Ecliptic(tmp_icrs, out_ecliptic);
}


/**********************************************************************//**
 * ECLIPTIC -> CIRS coordinate conversion
 * 
 * @param[in]  in_ecliptic       Input ECLIPTIC coordinates
 * @param[out] out_cirs          Output CIRS coordinates
 * @param[in]  date              Date for conversion
 *************************************************************************/
void CESkyCoord::Ecliptic2CIRS(const CESkyCoord& in_ecliptic,
                               CESkyCoord*       out_cirs,
                               const CEDate&     date)
{
    // ECLIPTIC -> ICRS
    CESkyCoord tmp_icrs;
    Ecliptic2ICRS(in_ecliptic, &tmp_icrs, date);

    // ICRS -> CIRS
    ICRS2CIRS(tmp_icrs, out_cirs, date);
}


/**********************************************************************//**
 * ECLIPTIC -> ICRS coordinate conversion
 * 
 * @param[in]  in_ecliptic       Input ECLIPTIC coordinates
 * @param[out] out_icrs          Output ICRS coordinates
 * @param[in]  date              Date for conversion
 *************************************************************************/
void CESkyCoord::Ecliptic2ICRS(const CESkyCoord& in_ecliptic,
                               CESkyCoord*       out_icrs,
                               const CEDate&     date)
{
    // Create the variables used for returning ICRS
    double ra(0.0);
    double dec(0.0);

    // Get current time in TT
    double tt1(0.0);
    double tt2(0.0);
    CEDate::UTC2TT(date.MJD(), &tt1, &tt2);

    // Convert ECLIPTIC to ICRS
    iauEceq06(tt1, tt2, 
              in_ecliptic.XCoord().Rad(), in_ecliptic.YCoord().Rad(), 
              &ra, &dec);

    // Set the output ICRS
    out_icrs->SetCoordinates(CEAngle::Rad(ra), CEAngle::Rad(dec),
                             CESkyCoordType::ICRS);
}


/**********************************************************************//**
 * ECLIPTIC -> GALACTIC coordinate conversion
 * 
 * @param[in]  in_ecliptic       Input ECLIPTIC coordinates
 * @param[out] out_galactic      Output GALACTIC coordinates
 * @param[in]  date              Date for conversion
 *************************************************************************/
void CESkyCoord::Ecliptic2Galactic(const CESkyCoord& in_ecliptic,
                                   CESkyCoord*       out_galactic,
                                   const CEDate&     date)
{
    // ECLIPTIC -> ICRS
    CESkyCoord tmp_icrs;
    Ecliptic2ICRS(in_ecliptic, &tmp_icrs, date);

    // ICRS -> GALACTIC
    ICRS2Galactic(tmp_icrs, out_galactic);
}


/**********************************************************************//**
 * ECLIPTIC -> OBSERVED coordinate conversion
 * 
 * @param[in]  in_ecliptic       Input ECLIPTIC coordinates
 * @param[out] out_observed      Output OBSERVED coordinates
 * @param[in]  date              Date for conversion
 * @param[in]  observer          Observer information
 *************************************************************************/
void CESkyCoord::Ecliptic2Observed(const CESkyCoord& in_ecliptic,
                                   CESkyCoord*       out_observed,
                                   const CEDate&     date,
                                   const CEObserver& observer)
{
    // ECLIPTIC -> CIRS
    CESkyCoord tmp_cirs;
    Ecliptic2CIRS(in_ecliptic, &tmp_cirs, date);

    // CIRS -> OBSERVED
    CIRS2Observed(tmp_cirs, out_observed, date, observer);
}


/**********************************************************************//**
 * Convert these coordinates to another coordinate system
 * NOTE: If this object is not OBSERVED coordinates, only JD is needed.
 * NOTE: If 'observer' is not nulltr then 'jd' will be ignored and the
 *       date will instead be derived from the observers date.
 * 
 * @param[in] output_coord_type    Output coordinate type (see CESkyCoordType)
 * @param[in] observer             If these coordinates are OBSERVED, then observer
 *                                 represents the observer for these coordinates.
 *                                 Otherwise they represent the observer to convert
 *                                 the coordinates to.
 * @param[in] date                 Julian date for conversion
 * @return Coordinates object that represents coordinates we're converting to
 *************************************************************************/
CESkyCoord CESkyCoord::ConvertTo(const CESkyCoordType&  output_coord_type, 
                                 const CEDate&     date,
                                 const CEObserver& observer)
{
    // Do conversion to CIRS
    CESkyCoord coord;
    if (output_coord_type == CESkyCoordType::CIRS) {
        coord = ConvertToCIRS(date, observer);
    } else if (output_coord_type == CESkyCoordType::ICRS) {
        coord = ConvertToICRS(date, observer);
    } else if (output_coord_type == CESkyCoordType::GALACTIC) {
        coord = ConvertToGalactic(date, observer);
    } else if (output_coord_type == CESkyCoordType::OBSERVED) {
        coord = ConvertToObserved(date, observer);
    } else if (output_coord_type == CESkyCoordType::ECLIPTIC) {
        coord = ConvertToEcliptic(date, observer);
    }

    return coord;
}


/**********************************************************************//**
 * Convert this coordinate to CIRS coordinates
 * 
 * @param[in]  date              Date for conversion
 * @param[in]  observer          Observer information
 * 
 * Note that the @p date and @p observer parameters are only necessary if
 * they are need to convert this object
 *************************************************************************/
CESkyCoord CESkyCoord::ConvertToCIRS(const CEDate&     date,
                                     const CEObserver& observer)
{
    // Create the coordinate to be returned
    CESkyCoord cirs;

    // Convert
    if (coord_type_ == CESkyCoordType::CIRS) {
        // CIRS -> CIRS
        cirs.SetCoordinates(*this);
    } else if (coord_type_ == CESkyCoordType::ICRS) {
        // ICRS -> CIRS
        ICRS2CIRS(*this, &cirs, date);
    } else if (coord_type_ == CESkyCoordType::GALACTIC) {
        // Galactic -> CIRS
        Galactic2CIRS(*this, &cirs, date);
    } else if (coord_type_ == CESkyCoordType::OBSERVED) {
        // Observed -> CIRS
        Observed2CIRS(*this, &cirs, date, observer);
    } else if (coord_type_ == CESkyCoordType::ECLIPTIC) {
        // ECLIPTIC -> CIRS
        Ecliptic2CIRS(*this, &cirs, date);
    }
    
    return cirs;
}


/**********************************************************************//**
 * Convert this coordinate to ICRS coordinates
 * 
 * @param[in]  date              Date for conversion
 * @param[in]  observer          Observer information
 * 
 * Note that the @p date and @p observer parameters are only necessary if
 * they are need to convert this object
 *************************************************************************/
CESkyCoord CESkyCoord::ConvertToICRS(const CEDate&     date,
                                     const CEObserver& observer)
{
    // Create return coordinate
    CESkyCoord icrs;

    // Convert
    if (coord_type_ == CESkyCoordType::CIRS) {
        // CIRS -> ICRS
        CIRS2ICRS(*this, &icrs, date);
    } else if (coord_type_ == CESkyCoordType::ICRS) {
        // ICRS -> ICRS
        icrs.SetCoordinates(*this);
    } else if (coord_type_ == CESkyCoordType::GALACTIC) {
        // GALACTIC -> ICRS
        Galactic2ICRS(*this, &icrs);
    } else if (coord_type_ == CESkyCoordType::OBSERVED) {
        // OBSERVED -> ICRS
        Observed2ICRS(*this, &icrs, date, observer);
    } else if (coord_type_ == CESkyCoordType::ECLIPTIC) {
        // ECLIPTIC -> ICRS
        Ecliptic2ICRS(*this, &icrs, date);
    }
    
    return icrs;
}


/**********************************************************************//**
 * Convert this coordinate to GALACTIC coordinates
 * 
 * @param[in]  date              Date for conversion
 * @param[in]  observer          Observer information
 * 
 * Note that the @p date and @p observer parameters are only necessary if
 * they are need to convert this object
 *************************************************************************/
CESkyCoord CESkyCoord::ConvertToGalactic(const CEDate&     date,
                                         const CEObserver& observer)
{
    // Create return coordiantes
    CESkyCoord galactic;

    // Convert
    if (coord_type_ == CESkyCoordType::CIRS) {
        // CIRS -> GALACTIC
        CIRS2Galactic(*this, &galactic, date);
    } else if (coord_type_ == CESkyCoordType::ICRS) {
        // ICRS -> GALACTIC
        ICRS2Galactic(*this, &galactic);
    } else if (coord_type_ == CESkyCoordType::GALACTIC) {
        // GALACTIC -> GALACTIC
        galactic.SetCoordinates(*this);
    } else if (coord_type_ == CESkyCoordType::OBSERVED) {
        // OBSERVED -> GALACTIC
        Observed2Galactic(*this, &galactic, date, observer);
    } else if (coord_type_ == CESkyCoordType::ECLIPTIC) {
        // ECLIPTIC -> GALACTIC
        Ecliptic2Galactic(*this, &galactic, date);
    }
    
    return galactic;
}


/**********************************************************************//**
 * Convert this coordinate to OBSERVED coordinates
 * 
 * @param[in]  date              Date for conversion
 * @param[in]  observer          Observer information
 * 
 * Note that the @p date and @p observer parameters are only necessary if
 * they are need to convert this object
 *************************************************************************/
CESkyCoord CESkyCoord::ConvertToObserved(const CEDate&     date,
                                         const CEObserver& observer)
{
    // Create return coordinates
    CESkyCoord observed;

    // Convert
    if (coord_type_ == CESkyCoordType::CIRS) {
        // CIRS -> OBSERVED
        CIRS2Observed(*this, &observed, date, observer);
    } else if (coord_type_ == CESkyCoordType::ICRS) {
        // ICRS -> OBSERVED
        ICRS2Observed(*this, &observed, date, observer);
    } else if (coord_type_ == CESkyCoordType::GALACTIC) {
        // GALACTIC -> OBSERVED
        Galactic2Observed(*this, &observed, date, observer);
    } else if (coord_type_ == CESkyCoordType::OBSERVED) {
        // OBSERVED -> OBSERVED
        observed.SetCoordinates(*this);
    } else if (coord_type_ == CESkyCoordType::ECLIPTIC) {
        // ECLIPTIC -> OBSERVED
        Ecliptic2Observed(*this, &observed, date, observer);
    }
    
    return observed;
}


/**********************************************************************//**
 * Convert this coordinate to ECLIPTIC coordinates
 * 
 * @param[in]  date              Date for conversion
 * @param[in]  observer          Observer information
 * 
 * Note that the @p date and @p observer parameters are only necessary if
 * they are need to convert this object
 *************************************************************************/
CESkyCoord CESkyCoord::ConvertToEcliptic(const CEDate&     date,
                                         const CEObserver& observer)
{
    // Create return coordinates
    CESkyCoord ecliptic;

    // Convert
    if (coord_type_ == CESkyCoordType::CIRS) {
        // CIRS -> ECLIPTIC
        CIRS2Ecliptic(*this, &ecliptic, date);
    } else if (coord_type_ == CESkyCoordType::ICRS) {
        // ICRS -> ECLIPTIC
        ICRS2Ecliptic(*this, &ecliptic, date);
    } else if (coord_type_ == CESkyCoordType::GALACTIC) {
        // GALACTIC -> ECLIPTIC
        Galactic2Ecliptic(*this, &ecliptic, date);
    } else if (coord_type_ == CESkyCoordType::OBSERVED) {
        // OBSERVED -> ECLIPTIC
        Observed2Ecliptic(*this, &ecliptic, date, observer);
    } else if (coord_type_ == CESkyCoordType::ECLIPTIC) {
        // ECLIPTIC -> ECLIPTIC
        ecliptic.SetCoordinates(*this);
    }
    
    return ecliptic;
}


/**********************************************************************//**
 * Set the coordinates of this object
 * 
 * @param[in] xcoord           X-coordinate
 * @param[in] ycoord           Y-coordinate
 * @param[in] coord_type       Coordinate type (see ::CESkyCoordType)
 *************************************************************************/
void CESkyCoord::SetCoordinates(const CEAngle& xcoord, 
                                const CEAngle& ycoord,
                                const CESkyCoordType& coord_type) const
{
    xcoord_     = xcoord;
    ycoord_     = ycoord;
    coord_type_ = coord_type;
}


/**********************************************************************//**
 * Set the coordinates from another CESkyCoord object
 * 
 * @param[in] coords       Another coordinates object to copy
 *************************************************************************/
void CESkyCoord::SetCoordinates(const CESkyCoord& coords)
{
    copy_members(coords);
}


/**********************************************************************//**
 * Generate a message string that specifies the information about this coordinate
 * 
 * @return String describing this object
 *************************************************************************/
std::string CESkyCoord::print(void) const
{
    std::string msg = "Coordinates:\n";
    msg += "   - System : " + std::to_string(int(coord_type_)) + "\n";
    msg += "   - X-coord: " + std::to_string(xcoord_.Deg()) + " deg\n";
    msg += "   - Y-coord: " + std::to_string(ycoord_.Deg()) + " deg\n";
    return msg;
}


/**********************************************************************//**
 * Copy data members from another CESkyCoord object
 * 
 * @param[in] other         Another coordinates objec to copy
 *************************************************************************/
void CESkyCoord::copy_members(const CESkyCoord& other)
{
    coord_type_ = other.coord_type_;
    xcoord_     = other.xcoord_;
    ycoord_     = other.ycoord_;
}


/**********************************************************************//**
 * Cleanup data members that need to be freed or cleared
 *************************************************************************/
void CESkyCoord::free_members(void)
{
    // Nothing to do here
}


/**********************************************************************//**
 * Set initial values and allocate memory for data members
 *************************************************************************/
void CESkyCoord::init_members(void)
{
    coord_type_ = CESkyCoordType::ICRS;
    xcoord_     = 0.0;
    ycoord_     = 0.0;
}


/**********************************************************************//**
 * Compare two coordinate objects
 *
 * @return True if the two coordinates are equal to each other
 *************************************************************************/
bool operator==(const CESkyCoord& lhs, const CESkyCoord& rhs)
{
    bool are_equal = true;
    
    // Check the coordinate systems are equivalent
    if (lhs.GetCoordSystem() != rhs.GetCoordSystem()) {
        are_equal = false;
    }
    // Check that the x-coordinate and the y-coordinate are the same
    else {
        // Check how far appart the coordinates are from each other
        CEAngle angsep = CESkyCoord::AngularSeparation(lhs, rhs);
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
 * 
 * @return True if two coordinates are NOT equal to each other
 *************************************************************************/
bool operator!=(const CESkyCoord& lhs, const CESkyCoord& rhs)
{
    return !(lhs == rhs);
}

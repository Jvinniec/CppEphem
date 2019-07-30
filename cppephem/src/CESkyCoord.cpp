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
#include "CEObserver.h"

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
 * @param[in] angle_type Angle type (either DEGREES or RADIANS)
 *************************************************************************/
CESkyCoord::CESkyCoord(const CEAngle&        xcoord, 
                       const CEAngle&        ycoord,
                       const CESkyCoordType& coord_type)
{
    init_members();
    this->SetCoordinates(xcoord, ycoord, coord_type);
}


/**********************************************************************//**
 * Primary constructor 
 * 
 * @param[in] xcoord X-Coordinate {hours,minutes,seconds}
 * @param[in] ycoord Y-Coordinate {degrees, arcmin, arcsec}
 * @param[in] coord_type Coordinate type (see CESkyCoordType)
 * @param[in] angle_type Angle type (either DEGREES or RADIANS)
 *************************************************************************/
CESkyCoord::CESkyCoord(const std::vector<double>& xcoord,
                       const std::vector<double>& ycoord,
                       const CESkyCoordType&      coord_type)
{
    init_members();
    CEAngle x;
    if ((coord_type == CESkyCoordType::GALACTIC) || 
        (coord_type == CESkyCoordType::OBSERVED)) {
        x = CEAngle::Dms(xcoord);
    } else {
        x = CEAngle::Hms(xcoord);
    }
    CEAngle y = CEAngle::Dms(ycoord);
    this->SetCoordinates(x, y, coord_type);
}


/**********************************************************************//**
 * Constructor from a coordinate type
 * 
 * @param[in] coord_type Coordinate type (see CESkyCoordType)
 *************************************************************************/
CESkyCoord::CESkyCoord(const CESkyCoordType& coord_type) :
    coord_type_(coord_type)
{
    init_members();
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
                             coords2.XCoord(), y2) ;
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
    double eo ; // Equation of the origins

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
    //*return_ra -= eo ;

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
    ICRS2Galactic(*out_galactic, out_galactic) ;
}


/**********************************************************************//**
 * CIRS -> Observed (or observer specific) coordinate conversion
 * This function takes in verious observation parameters
 * The integer returned is a status code with the following meanings:
 *     +1 = dubious year (too far into the past/future to be trusted)
 *      0 = OK status
 *     -1 = unacceptable date
 * 
 * @param[in]  in_cirs          Input CIRS coordinates
 * @param[out] out_observed     Output Observed coordinates
 * @param[in]  date             Date for conversion
 * @param[in]  observer         Observer information
 * @param[out] observed_cirs    'Observed' CIRS coordinates
 * @param[out] hour_angle       Hour angle of coordinates for observer
 *************************************************************************/
int  CESkyCoord::CIRS2Observed(const CESkyCoord& in_cirs,
                                  CESkyCoord*       out_observed,
                                  const CEDate&     date,
                                  const CEObserver& observer,
                                  CESkyCoord*       observed_cirs,
                                  double*           hour_angle)
{
    // Setup the observed RA, Dec and hour_angle variables
    double temp_ra(0.0);
    double temp_dec(0.0);
    double temp_hour_angle(0.0);

    // If values were not passed, point them at the temporary ones
    if (hour_angle == nullptr) hour_angle = &temp_hour_angle;

    // Call the necessary sofa method
    int err_code = 0;
    try {
        double az(0.0);
        double zen(0.0);
        err_code = iauAtio13(in_cirs.XCoord(), 
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
                             hour_angle, 
                             &temp_ra, 
                             &temp_dec);
        
        if (err_code == -1) {
            throw CEException::sofa_error("CESkyCoord::CIRS2Observed",
                                          "iauAtio13", -1,
                                          "SOFA method was passed an unacceptable date");
        }

        // Set the output coordinates
        out_observed->SetCoordinates(CEAngle(az), CEAngle(zen), 
                                     CESkyCoordType::OBSERVED);
    } catch (std::exception &e) {
        throw CEException::sofa_exception("CESkyCoord::CIRS2Observed",
                                          "iauAtio13",
                                          e.what());
    }

    return err_code ;
}


void CESkyCoord::ICRS2CIRS(const CESkyCoord& in_icrs,
                           CESkyCoord*       out_cirs,
                           const CEDate&     date)
{
    // Store the equation of the origins
    double eo ; // Equation of the origins
    
    // Use the sofa library to convert these coordinates
    try {
        double tdb1(0.0);
        double tdb2(0.0);
        CEDate::UTC2TDB(date.MJD(), &tdb1, &tdb2);
        iauAtci13(in_icrs.XCoord().Rad(), 
                  in_icrs.YCoord().Rad(),
                  0.0, 0.0, 0.0, 0.0, 
                  tdb1, tdb2, 
                  return_ra, return_dec, &eo) ;
        
        // Set the output cirs coordinates
        out_cirs->SetCoordinates(CEAngle::Rad(return_ra),
                                CEAngle::Rad(return_dec),
                                CESkyCoordType::CIRS);

        // Subtract the equation of the origins if J2000 coordinates are desired
        //*return_ra -= eo ;
    } catch (std::exception &e) {
        throw CEException::sofa_exception("CESkyCoord::ICRS2CIRS",
                                          "iauAtci13", 
                                          "Exception thrown");
    }

    return;
}

void CESkyCoord::ICRS2Galactic(const CESkyCoord& in_icrs,
                               CESkyCoord*       out_galactic)
{
    // Use the sofa method to convert the coordinates
    double glon(0.0);
    double glat(0.0);
    iauIcrs2g(in_icrs.XCoord().Rad(), in_icrs.YCoord().Rad(), glon, glat);
    out_galactic->SetCoordinates(CEAngle::Rad(glon),
                                 CEAngle::Rad(glat),
                                 CESkyCoordType::GALACTIC);
    return
}


int CESkyCoord::ICRS2Observed(const CESkyCoord&    in_icrs,
                                 CESkyCoord*       out_observed,
                                 const CEDate&     date,
                                 const CEObserver& observer,
                                 CESkyCoord*       observed_icrs,
                                 double*           hour_angle)
{
    // First convert the ICRS coordinates to CIRS coordinates
    CESkyCoord tmp_cirs;
    ICRS2CIRS(in_icrs, &tmp_cirs, date) ;

    // Now convert CIRS to Observed
    CESkyCoord obs_cirs;
    int err_code = CIRS2Observed(tmp_cirs, out_observed,
                                 date, observer, 
                                 obs_cirs, hour_angle);
    
    // Convert the apparent CIRS RA,Dec to ICRS RA,Dec
    CIRS2ICRS(obs_cirs, observed_icrs, date) ;

    return;
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
        coord = ConvertToCIRS(date, observer) ;
    } else if (output_coord_type == CESkyCoordType::ICRS) {
        coord = ConvertToICRS(date, observer) ;
    } else if (output_coord_type == CESkyCoordType::GALACTIC) {
        coord = ConvertToGalactic(date, observer) ;
    } else if (output_coord_type == CESkyCoordType::OBSERVED) {
        coord = ConvertToObserved(date, observer) ;
    } else {
        std::cerr << "[ERROR] Unknown coordinate type passed to CESkyCoord::ConvertTo()!\n" ;
    }

    return coord;
}


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
    }
    
    return cirs;
}


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
        Galactic2ICRS(*this, &icrs) ;
    } else if (coord_type_ == CESkyCoordType::OBSERVED) {
        // OBSERVED -> ICRS
        Observed2ICRS(*this, &icrs, date, observer);
    }
    
    return coord;
}


CESkyCoord CESkyCoord::ConvertToGalactic(const CEDate&     date,
                                         const CEObserver& observer)
{
    // Create return coordiantes
    CESkyCoord galactic;

    if (coord_type_ == CESkyCoordType::CIRS) {
        // CIRS -> GALACTIC
        CIRS2Galactic(*this, &galactic, date);
    } else if (coord_type_ == CESkyCoordType::ICRS) {
        // ICRS -> GALACTIC
        ICRS2Galactic(*this, &galactic, date);
    } else if (coord_type_ == CESkyCoordType::GALACTIC) {
        // GALACTIC -> GALACTIC
        galactic.SetCoordinates(*this);
    } else if (coord_type_ == CESkyCoordType::OBSERVED) {
        // OBSERVED -> GALACTIC
        Observed2Galactic((this, &galactic, date, observed);
    }
    
    return galactic;
}


CESkyCoord CESkyCoord::ConvertToObserved(const CEDate&     date,
                                         const CEObserver& observer)
{

}


/**********************************************************************//**
 * Set the coordinates of this object
 * @param[in] xcoord           X-coordinate
 * @param[in] ycoord           Y-coordinate
 * @param[in] coord_type       Coordinate type (see ::CESkyCoordType)
 *************************************************************************/
void CESkyCoord::SetCoordinates(const CEAngle& xcoord, 
                                const CEAngle& ycoord,
                                const CESkyCoordType& coord_type)
{
    xcoord_     = xcoord ;
    ycoord_     = ycoord ;
    coord_type_ = coord_type ;
}

/**********************************************************************//**
 * Set the coordinates from another CESkyCoord object
 * @param[in] coords       Another coordinates object to copy
 *************************************************************************/
void CESkyCoord::SetCoordinates(const CESkyCoord& coords)
{
    copy_members(coords);
}


/**********************************************************************//**
 * Generate a message string that specifies the information about this coordinate
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
 *  @return True if two coordinates are NOT equal to each other
 *************************************************************************/
bool operator!=(const CESkyCoord& lhs, const CESkyCoord& rhs)
{
    return !(lhs == rhs);
}

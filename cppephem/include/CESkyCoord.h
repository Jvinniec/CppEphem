/***************************************************************************
 *  CESkyCoord.h: CppEphem                                                 *
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

#ifndef CESkyCoord_h
#define CESkyCoord_h

#include <string>
#include <vector>

// CppEphem HEADERS
#include "CEBase.h"
#include "CEAngle.h"
#include "CEDate.h"
#include "CENamespace.h"
#include "CEException.h"
#include "CEObserver.h"
#include "CECoordinates.h"

// SOFA HEADER
#include "sofa.h"

/** The following enum specifies what coordinates this object represents */
enum class CESkyCoordType : unsigned int {
    CIRS=0,           ///< RA, Dec (referenced at the center of the Earth)
    ICRS=1,           ///< RA, Dec (referenced at the barycenter of the solarsystem)
    GALACTIC=2,       ///< Galacitc longitude, latitude
    OBSERVED=3,       ///< Azimuth, Zenith (requires additional observer information)
    ECLIPTIC=4        ///< Ecliptic longitude, latitude
};

// Class for handling sky coordinates
class CESkyCoord : public CEBase {

    friend bool operator==(const CESkyCoord& lhs, const CESkyCoord& rhs);
    friend bool operator!=(const CESkyCoord& lhs, const CESkyCoord& rhs);

public:

    /****** CONSTRUCTORS ******/
    CESkyCoord() ;
    CESkyCoord(const CEAngle& xcoord, 
               const CEAngle& ycoord,
               const CESkyCoordType& coord_type=CESkyCoordType::ICRS) ;
    explicit CESkyCoord(const CECoordinates& other);
    CESkyCoord(const CESkyCoord& other) ;
    virtual ~CESkyCoord() ;
    
    CESkyCoord& operator=(const CESkyCoord& other);

    /*********************************************************
     * Angular separation between two coordinate positions
     *********************************************************/
    virtual CEAngle Separation(const CESkyCoord& coords) const;
    static  CEAngle AngularSeparation(const CEAngle& xcoord_first, 
                                      const CEAngle& ycoord_first,
                                      const CEAngle& xcoord_second, 
                                      const CEAngle& ycoord_second);
    
    /**********************************************************
     * Methods for accessing the coordinate information
     **********************************************************/
    
    virtual CEAngle XCoord(const CEDate& jd=CppEphem::julian_date_J2000()) const;
    virtual CEAngle YCoord(const CEDate& jd=CppEphem::julian_date_J2000()) const;
    
    // Return coordinate system
    CESkyCoordType GetCoordSystem(void) const;
    
    /**********************************************************
     * Methods for converting between coordinate types
     **********************************************************/
    // Note that whenever a date is required, the default will be set to the
    // start of the J2000 epoch (January 1, 2000 at 12:00 GMT). This corresponds
    // to the Julian Date of 2451545.0.
    
    // Convert from CIRS to other coordinates
    static void CIRS2ICRS(const CESkyCoord& in_cirs, 
                          CESkyCoord*       out_icrs,
                          const CEDate&     date=CEDate());
    static void CIRS2Galactic(const CESkyCoord& in_cirs,
                              CESkyCoord*       out_galactic,
                              const CEDate&     date=CEDate());
    static void CIRS2Observed(const CESkyCoord& in_cirs,
                              CESkyCoord*       out_observed,
                              const CEDate&     date,
                              const CEObserver& observer,
                              CESkyCoord*       observed_cirs=nullptr,
                              CEAngle*          hour_angle=nullptr);
    static void CIRS2Ecliptic(const CESkyCoord& in_cirs,
                              CESkyCoord*       out_ecliptic,
                              const CEDate&     date=CEDate());

    // Convert from ICRS to other coordinates
    static void ICRS2CIRS(const CESkyCoord& in_icrs,
                          CESkyCoord*       out_cirs,
                          const CEDate&     date=CEDate());
    static void ICRS2Galactic(const CESkyCoord& in_icrs,
                              CESkyCoord*       out_galactic);
    static void ICRS2Observed(const CESkyCoord& in_icrs,
                              CESkyCoord*       out_observed,
                              const CEDate&     date,
                              const CEObserver& observer,
                              CESkyCoord*       observed_cirs=nullptr,
                              CEAngle*          hour_angle=nullptr);
    static void ICRS2Ecliptic(const CESkyCoord& in_icrs,
                              CESkyCoord*       out_ecliptic,
                              const CEDate&     date=CEDate());

    // Convert from GALACTIC to other coordinates
    static void Galactic2CIRS(const CESkyCoord& in_galactic,
                              CESkyCoord*       out_cirs,
                              const CEDate&     date=CEDate());
    static void Galactic2ICRS(const CESkyCoord& in_galactic,
                              CESkyCoord*       out_icrs);
    static void Galactic2Observed(const CESkyCoord& in_galactic,
                                  CESkyCoord*       out_observed,
                                  const CEDate&     date,
                                  const CEObserver& observer,
                                  CESkyCoord*       observed_galactic=nullptr,
                                  CEAngle*          hour_angle=nullptr);
    static void Galactic2Ecliptic(const CESkyCoord& in_galactic,
                                  CESkyCoord*       out_ecliptic,
                                  const CEDate&     date=CEDate());

    // Convert from OBSERVED to other coordinates
    static void Observed2CIRS(const CESkyCoord& in_observed,
                              CESkyCoord*       out_cirs,
                              const CEDate&     date,
                              const CEObserver& observer);
    static void Observed2ICRS(const CESkyCoord& in_observed,
                              CESkyCoord*       out_icrs,
                              const CEDate&     date,
                              const CEObserver& observer);
    static void Observed2Galactic(const CESkyCoord& in_observed,
                                  CESkyCoord*       out_galactic,
                                  const CEDate&     date,
                                  const CEObserver& observer);
    static void Observed2Ecliptic(const CESkyCoord& in_observed,
                                  CESkyCoord*       out_ecliptic,
                                  const CEDate&     date,
                                  const CEObserver& observer);

    // Convert from ECLIPTIC to other coordinates
    static void Ecliptic2CIRS(const CESkyCoord& in_ecliptic,
                              CESkyCoord*       out_cirs,
                              const CEDate&     date=CEDate());
    static void Ecliptic2ICRS(const CESkyCoord& in_ecliptic,
                              CESkyCoord*       out_icrs,
                              const CEDate&     date=CEDate());
    static void Ecliptic2Galactic(const CESkyCoord& in_ecliptic,
                                  CESkyCoord*       out_galactic,
                                  const CEDate&     date=CEDate());
    static void Ecliptic2Observed(const CESkyCoord& in_ecliptic,
                                  CESkyCoord*       out_observed,
                                  const CEDate&     date,
                                  const CEObserver& observer);

    /*********************************************************
     * More generic methods for converting between coordinate types
     *********************************************************/
    CESkyCoord ConvertTo(const CESkyCoordType&  output_coord_type,
                         const CEDate&          date=CEDate(),
                         const CEObserver&      observer=CEObserver());
    CESkyCoord ConvertToCIRS(const CEDate&     date=CEDate(),
                             const CEObserver& observer=CEObserver());
    CESkyCoord ConvertToICRS(const CEDate&     date=CEDate(),
                             const CEObserver& observer=CEObserver());
    CESkyCoord ConvertToGalactic(const CEDate&     date=CEDate(),
                                 const CEObserver& observer=CEObserver());
    CESkyCoord ConvertToObserved(const CEDate&     date=CEDate(),
                                 const CEObserver& observer=CEObserver());
    CESkyCoord ConvertToEcliptic(const CEDate&     date=CEDate(),
                                 const CEObserver& observer=CEObserver());

    /*********************************************************
     * Methods for setting the coordinates of this object
     *********************************************************/
    virtual void SetCoordinates(const CEAngle& xcoord, 
                                const CEAngle& ycoord,
                                const CESkyCoordType& coord_type=CESkyCoordType::ICRS) const;
    virtual void SetCoordinates(const CESkyCoord& coords);

    // Support methods
    const std::string print(void) const;
    const std::string ClassName(void) const;
    virtual const std::string describe(void) const;

private:
    /*********************************************************
     * Private methods
     *********************************************************/
    void copy_members(const CESkyCoord& other);
    void free_members(void);
    void init_members(void);

    // Coordinate variables
    mutable CEAngle         xcoord_;        //<! X coordinate
    mutable CEAngle         ycoord_;        //<! Y coordinate
    mutable CESkyCoordType  coord_type_;    //<! Coordinate system to which 'xcoord_' and 'ycoord_' belong.
};


/**********************************************************************//**
 * Return name of this class
 *************************************************************************/
inline
const std::string CESkyCoord::ClassName() const
{
    return std::string("CESkyCoord");
}


/**********************************************************************//**
 * Return x coordinate at given Julian date
 * 
 * @param[in] jd   Julian date (used only by derived classes)
 * @return X-coordinate as a CEAngle
 *************************************************************************/
inline
CEAngle CESkyCoord::XCoord(const CEDate& jd) const
{
    return xcoord_;
}


/**********************************************************************//**
 * Return y coordinate at given Julian date
 * 
 * @param[in] jd   Julian date (used only by derived classes)
 * @return X-coordinate as a CEAngle
 *************************************************************************/
inline
CEAngle CESkyCoord::YCoord(const CEDate& jd) const
{
    return ycoord_;
}


/**********************************************************************//**
 * Return coordinate system
 * @return Coordinate type of this object
 *************************************************************************/
inline
CESkyCoordType CESkyCoord::GetCoordSystem(void) const 
{
    return coord_type_;
}

#endif /* CESkyCoord_h */

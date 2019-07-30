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
#include "CEAngle.h"
#include "CEDate.h"
#include "CENamespace.h"
#include "CEException.h"
#include "CEObserver.h"

// SOFA HEADER
#include "sofa.h"

/** The following enum specifies what coordinates this object represents */
enum class CESkyCoordType
{
    CIRS=0,           ///< RA, Dec (referenced at the center of the Earth)
    ICRS=1,           ///< RA, Dec (referenced at the barycenter of the solarsystem)
    GALACTIC=2,       ///< Galacitc longitude, latitude
    OBSERVED=3        ///< Azimuth, Zenith (requires additional observer information)
};

// Initiate the class that holds the coordinate information
class CESkyCoord {

    friend bool operator==(const CESkyCoord& lhs, const CESkyCoord& rhs);
    friend bool operator!=(const CESkyCoord& lhs, const CESkyCoord& rhs);

public:

    /****** CONSTRUCTORS ******/
    CESkyCoord() ;
    CESkyCoord(const CEAngle& xcoord, 
               const CEAngle& ycoord,
               const CESkyCoordType& coord_type=CESkyCoordType::ICRS) ;
    CESkyCoord(const std::vector<double>& xcoord,
               const std::vector<double>& ycoord,
               const CESkyCoordType& coord_type=CESkyCoordType::ICRS);
    CESkyCoord(const CESkyCoordType& coord_type) ;
    CESkyCoord(const CESkyCoord& other) ;
    virtual ~CESkyCoord() ;
    
    CESkyCoord& operator=(const CESkyCoord& other);

    /*********************************************************
     * Angular separation between two coordinate positions
     *********************************************************/
    virtual CEAngle AngularSeparation(const CESkyCoord& coords) const;
    static  CEAngle AngularSeparation(const CESkyCoord& coords1, 
                                      const CESkyCoord& coords2);
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
    CECoordinateType GetCoordSystem(void) const;
    
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
    static int  CIRS2Observed(const CESkyCoord& in_cirs,
                              CESkyCoord*       out_observed,
                              const CEDate&     date,
                              const CEObserver& observer,
                              CESkyCoord*       observed_cirs=nullptr,
                              double*           hour_angle=nullptr);

    // Convert from ICRS to other coordinates
    static void ICRS2CIRS(const CESkyCoord& in_icrs,
                          CESkyCoord*       out_cirs,
                          const CEDate&     date=CEDate());
    static void ICRS2Galactic(const CESkyCoord& in_icrs,
                              CESkyCoord*       out_galactic);
    static int  ICRS2Observed(const CESkyCoord& in_icrs,
                              CESkyCoord*       out_observed,
                              const CEDate&     date,
                              const CEObserver& observer,
                              CESkyCoord*       observed_cirs=nullptr,
                              double*              hour_angle=nullptr);

    // // Convert from GALACTIC to other coordinates
    // static void Galactic2CIRS(const CECoordinates& in_galactic,
    //                           CECoordinates*       out_cirs,
    //                           const CEDate&        date=CEDate());
    // static void Galactic2ICRS(const CECoordinates& in_galactic,
    //                           CECoordinates*       out_icrs);
    // static int Galactic2Observed(const CECoordinates& in_galactic,
    //                              CECoordinates*       out_observed,
    //                              const CEDate&        date,
    //                              const CEObserver&    observer,
    //                              CECoordinates*       observed_galactic=nullptr,
    //                              double*              hour_angle=nullptr);

    // // Convert from OBSERVED to other coordinates
    // static int Observed2CIRS(const CECoordinates& in_observed,
    //                          CECoordinates*       out_cirs,
    //                          const CEDate&        date,
    //                          const CEObserver&    observer);
    // static int Observed2ICRS(const CECoordinates& in_observed,
    //                          CECoordinates*       out_icrs,
    //                          const CEDate&        date,
    //                          const CEObserver&    observer);
    // static int Observed2Galactic(const CECoordinates& in_observed,
    //                              CECoordinates*       out_galactic,
    //                              const CEDate&        date,
    //                              const CEObserver&    observer);

    /*********************************************************
     * More generic methods for converting between coordinate types
     *********************************************************/
    CESkyCoord ConvertTo(const CESkyCoordType&  output_coord_type,
                         const CEDate&          date=CEDate::CurrentJD(),
                         const CEObserver&      observer=CEObserver());
    CESkyCoord ConvertToCIRS(const CEDate&     date=CEDate::CurrentJD(),
                             const CEObserver& observer=nullptr);
    CESkyCoord ConvertToICRS(const CEDate&     date=CEDate::CurrentJD(),
                             const CEObserver& observer=nullptr);
    CESkyCoord ConvertToGalactic(const CEDate&     date=CEDate::CurrentJD(),
                                 const CEObserver& observer=nullptr);
    CESkyCoord ConvertToObserved(const CEDate&     date=CEDate::CurrentJD(),
                                 const CEObserver& observer=nullptr);

    /*********************************************************
     * Methods for setting the coordinates of this object
     *********************************************************/
    virtual void SetCoordinates(const CEAngle& xcoord, 
                                const CEAngle& ycoord,
                                const CESkyCoordType& coord_type = CESkyCoordType::ICRS);
    virtual void SetCoordinates(const CESkyCoord& coords);

    // Support methods
    std::string print(void) const;

protected:
    // Coordinate variables
    mutable CEAngle xcoord_;        //<! X coordinate
    mutable CEAngle ycoord_;        //<! Y coordinate
    CESkyCoordType  coord_type_;    //<! Coordinate system to which 'xcoord_' and 'ycoord_' belong.

private:
    /*********************************************************
     * Private methods
     *********************************************************/
    void copy_members(const CESkyCoordType& other);
    void free_members(void);
    void init_members(void);
};


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

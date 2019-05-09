/***************************************************************************
 *  CEObservation.h: CppEphem                                              *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2017 JCardenzana                                           *
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

#ifndef CEObservation_h
#define CEObservation_h

// CppEphem HEAD
#include "CECoordinates.h"
#include "CEBody.h"
#include "CEDate.h"
#include "CEObserver.h"
#include "CENamespace.h"


class CEObservation {
public:
    // Constructors
    CEObservation() ;
    CEObservation(CEObserver* observer, CEBody* body, CEDate* date);
    CEObservation(const CEObservation& other);
    virtual ~CEObservation() ;
    
    // Copy assignment operator
    CEObservation& operator=(const CEObservation& other);

    // Setting underlying object pointers
    virtual void   SetObserver(CEObserver* new_observer);
    virtual void   SetBody(CEBody* new_body);
    virtual void   SetDate(CEDate* new_date);
    
    /// Access the underlying objects
    CEObserver*    Observer();
    CEBody*        Body();
    CEDate*        Date();
    virtual double GetAzimuth_Rad();
    virtual double GetAzimuth_Deg();
    virtual double GetZenith_Rad();
    virtual double GetZenith_Deg();
    virtual double GetAltitude_Rad();
    virtual double GetAltitude_Deg();
    virtual double GetHourAngle_Rad();
    virtual double GetHourAngle_Deg();
    virtual double GetApparentXCoordinate_Rad();
    virtual double GetApparentXCoordinate_Deg();
    virtual double GetApparentYCoordinate_Rad();
    virtual double GetApparentYCoordinate_Deg();
    virtual void   GetAzimuthZenith_Rad(double *azimuth, double *zenith);
    virtual void   GetAzimuthZenith_Deg(double *azimuth, double *zenith);
    virtual void   GetApparentXYCoordinate_Rad(double *apparent_X, double *apparent_Y);
    virtual void   GetApparentXYCoordinate_Deg(double *apparent_X, double *apparent_Y);
    bool           UpdateCoordinates();
    
private:
    
    // Member functions for setup and tear down
    void copy_members(const CEObservation& other);
    void init_members(void);
    void free_members(void);

    // Updates the coordinates if the date, observer or object has changed
    bool NeedsUpdate(void);
    
    CEBody*     body_;                  //<! Object being observed
    CEDate*     date_;                  //<! Store a pointer to a CEDate object
    CEObserver* observer_;              //<! Store a pointer to a CEObserver object
    
    // Cached the observed parameters to make subsequent calls faster,
    // i.e. since they all get computed at the same time there's no need
    // to recompute them unless something changes.
    double        cached_date_;   //<! Cached date, observed params recomputed if this has changed
    CECoordinates cached_coords_; //<! Cached observed coordinates
    
    // Cache the apparent coordinates
    double cached_hour_angle_;   //<! Cached hour angle (radians)
    double cached_apparentxcoord_;
    double cached_apparentycoord_;
};


/**********************************************************************//**
 * Access the underlying objects
 * @return Pointer to current observer object
 *************************************************************************/
inline
CEObserver* CEObservation::Observer() 
{
    return observer_;
}

/**********************************************************************//**
 * Access underlying CEBody object
 * @return Pointer to current CEBody object
 *************************************************************************/
inline
CEBody* CEObservation::Body() 
{
    return body_;
}


/**********************************************************************//**
 * Access underlying CEDate object
 * @return Pointer to current CEDate object
 *************************************************************************/
inline
CEDate* CEObservation::Date() 
{
    return date_;
}


/**********************************************************************//**
 * Set underlying CEObserver object
 * @param[in] new_observer      Pointer to new CEObserver object
 *************************************************************************/
inline
void CEObservation::SetObserver(CEObserver* new_observer) 
{
    observer_ = new_observer;
}


/**********************************************************************//**
 * Set underlying CEBody object
 * @param[in] new_body          Pointer to new CEBody object
 *************************************************************************/
inline
void CEObservation::SetBody(CEBody* new_body) 
{
    body_ = new_body;
}


/**********************************************************************//**
 * Set underlying CEDate object
 * @param[in] new_date          Pointer to new CEDate object
 *************************************************************************/
inline
void CEObservation::SetDate(CEDate* new_date) 
{
    date_ = new_date;
}


/**********************************************************************//**
 * @return azimuth in radians
 *************************************************************************/
inline
double CEObservation::GetAzimuth_Rad()
{
    UpdateCoordinates();
    return cached_coords_.XCoordinate_Rad();
}


/**********************************************************************//**
 * @return azimuth in degrees
 *************************************************************************/
inline
double CEObservation::GetAzimuth_Deg() 
{
    return GetAzimuth_Rad() * DR2D;
}


/**********************************************************************//**
 * @return zenith angle in radians
 *************************************************************************/
inline
double CEObservation::GetZenith_Rad()
{
    UpdateCoordinates();
    return cached_coords_.YCoordinate_Rad();
}


/**********************************************************************//**
 * @return zenith angle in degrees
 *************************************************************************/
inline
double CEObservation::GetZenith_Deg() 
{
    return GetZenith_Rad() * DR2D;
}


/**********************************************************************//**
 * @return altitude angle in radians
 *************************************************************************/
inline
double CEObservation::GetAltitude_Rad()
{
    UpdateCoordinates() ;
    return (DPI/2.0) - GetZenith_Rad() ;
}


/**********************************************************************//**
 * @return altitude angle in degrees
 *************************************************************************/
inline
double CEObservation::GetAltitude_Deg()
{
    return GetAltitude_Rad() * DR2D;
}


/**********************************************************************//**
 * @return hour angle in radians
 *************************************************************************/
inline
double CEObservation::GetHourAngle_Rad()
{
    UpdateCoordinates();
    return cached_hour_angle_;
}


/**********************************************************************//**
 * @return hour angle in degrees
 *************************************************************************/
inline
double CEObservation::GetHourAngle_Deg()
{
    return GetHourAngle_Rad() * DR2D;
}


/**********************************************************************//**
 *************************************************************************/
inline
double CEObservation::GetApparentXCoordinate_Rad()
{
    UpdateCoordinates() ;
    return cached_apparentxcoord_ ;
}


/**********************************************************************//**
 * @return apparent x-coordinate value as observed by 'observer_' in degrees
 *************************************************************************/
inline
double CEObservation::GetApparentXCoordinate_Deg()
{
    return GetApparentXCoordinate_Rad() * DR2D;
}


/**********************************************************************//**
 * @return apparent y-coordinate value as observed by 'observer_' in radians
 *************************************************************************/
inline
double CEObservation::GetApparentYCoordinate_Rad()
{
    UpdateCoordinates() ;
    return cached_apparentycoord_ ;
}


/**********************************************************************//**
 * @return apparent y-coordinate value as observed by 'observer_' in degrees
 *************************************************************************/
inline
double CEObservation::GetApparentYCoordinate_Deg() 
{
    return GetApparentYCoordinate_Rad() * DR2D;
}

#endif /* CEObservation_h */

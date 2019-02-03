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


class CEObservation {
public:
    // Constructors
    CEObservation() ;
    CEObservation(CEObserver* observer, CEBody* body, CEDate* date=nullptr) ;
    virtual ~CEObservation() ;
    
    /// Set the observer object
    virtual void SetObserver(CEObserver* new_observer) {observer_ = new_observer;}
    /// Set the object being observed
    virtual void SetBody(CEBody* new_body) {body_ = new_body;}
    /// Set the date of the observation
    virtual void SetDate(CEDate* new_date) {date_ = new_date;}
    
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
    
protected:
    
    CEBody*     body_     = nullptr;    //<! Object being observed
    CEDate*     date_     = nullptr;    //<! Store a pointer to a CEDate object
    CEObserver* observer_ = nullptr;    //<! Store a pointer to a CEObserver object
    
    // Cached the observed parameters to make subsequent calls faster,
    // i.e. since they all get computed at the same time there's no need
    // to recompute them unless something changes.
    double cached_date_       = 0.0 ;   //<! Cached date, observed params recomputed if this has changed
    double cached_azimuth_    = 0.0 ;   //<! Cached azimuth (radians)
    double cached_zenith_     = 0.0 ;   //<! Cached zenith (radians)
    double cached_altitude_   = 0.0 ;   //<! Cached altitude (radians)
    double cached_hour_angle_ = 0.0 ;   //<! Cached hour angle (radians)
    
    // Cache the apparent coordinates fo
    double cached_apparentxcoord_ = 0.0 ;
    double cached_apparentycoord_ = 0.0 ;
    
    bool   DateHasChanged() ;
    
};


/**********************************************************************//**
 * Access the underlying objects
 *************************************************************************/
inline
CEObserver* CEObservation::Observer() 
{
    return observer_;
}

/**********************************************************************//**
 *************************************************************************/
inline
CEBody* CEObservation::Body() 
{
    return body_;
}


/**********************************************************************//**
 *************************************************************************/
inline
CEDate* CEObservation::Date() 
{
    return date_;
}


/**********************************************************************//**
 * @return azimuth in radians
 *************************************************************************/
inline
double CEObservation::GetAzimuth_Rad()
{
    if (DateHasChanged()) UpdateCoordinates();
    return cached_azimuth_;
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
    if (DateHasChanged()) UpdateCoordinates();
    return cached_zenith_;
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
    if (DateHasChanged()) UpdateCoordinates() ;
    return cached_altitude_ ;
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
    if (DateHasChanged()) UpdateCoordinates();
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
    if (DateHasChanged()) UpdateCoordinates() ;
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
    if (DateHasChanged()) UpdateCoordinates() ;
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

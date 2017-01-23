//
//  CELinkedCoordinates.h
//  CppEphem
//
//  Created by Josh Cardenzana on 1/19/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

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
    CEObserver* Observer() {return observer_;}
    CEBody* Body() {return body_;}
    CEDate* Date() {return date_;}
    
    
//    virtual CECoordinates* GetObservedCoordinates() ;
    ///////////////////////////////////////////////////////////
    /// @return azimuth in radians
    virtual double GetAzimuth_Rad()
    {
        if (DateHasChanged()) UpdateCoordinates() ;
        return cached_azimuth_ ;
    }
    /// @return azimuth in degrees
    virtual double GetAzimuth_Deg() {return GetAzimuth_Rad() * DR2D ;}
    
    ///////////////////////////////////////////////////////////
    /// @return zenith angle in radians
    virtual double GetZenith_Rad()
    {
        if (DateHasChanged()) UpdateCoordinates() ;
        return cached_zenith_ ;
    }
    /// @return zenith angle in degrees
    virtual double GetZenith_Deg() {return GetZenith_Rad() * DR2D ;}

    ///////////////////////////////////////////////////////////
    /// @return altitude angle in radians
    virtual double GetAltitude_Rad()
    {
        if (DateHasChanged()) UpdateCoordinates() ;
        return cached_altitude_ ;
    }
    /// @return altitude angle in degrees
    virtual double GetAltitude_Deg() {return GetAltitude_Rad() * DR2D ;}
    
    ///////////////////////////////////////////////////////////
    /// @return hour angle in radians
    virtual double GetHourAngle_Rad()
    {
        if (DateHasChanged()) UpdateCoordinates() ;
        return cached_hour_angle_ ;
    }
    /// @return hour angle in degrees
    virtual double GetHourAngle_Deg() {return GetHourAngle_Rad() * DR2D ;}
    
    ///////////////////////////////////////////////////////////
    // Return the apparent coordinates of the body being observed
    ///////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////
    /// @return apparent x-coordinate value as observed by 'observer_' in radians
    virtual double GetApparentXCoordinate_Rad()
    {
        if (DateHasChanged()) UpdateCoordinates() ;
        return cached_apparentxcoord_ ;
    }
    /// @return apparent x-coordinate value as observed by 'observer_' in degrees
    virtual double GetApparentXCoordinate_Deg() {return cached_apparentxcoord_ * DR2D ;}

    ///////////////////////////////////////////////////////////
    /// @return apparent y-coordinate value as observed by 'observer_' in radians
    virtual double GetApparentYCoordinate_Rad()
    {
        if (DateHasChanged()) UpdateCoordinates() ;
        return cached_apparentycoord_ ;
    }
    /// @return apparent y-coordinate value as observed by 'observer_' in degrees
    virtual double GetApparentYCoordinate_Deg() {return cached_apparentycoord_ * DR2D ;}
    
    virtual void GetAzimuthZenith_Rad(double *azimuth, double *zenith) ;
    virtual void GetAzimuthZenith_Deg(double *azimuth, double *zenith) ;
    virtual void GetApparentXYCoordinate_Rad(double *apparent_X, double *apparent_Y) ;
    virtual void GetApparentXYCoordinate_Deg(double *apparent_X, double *apparent_Y) ;
    
    bool UpdateCoordinates() ;
    
protected:
    
    CEObserver* observer_ = nullptr;    /// Store a pointer to a CEObserver object
    CEDate* date_ = nullptr;            /// Store a pointer to a CEDate object
    CEBody* body_ = nullptr;            /// Object being observed
    
    // Cached the observed parameters to make subsequent calls faster,
    // i.e. since they all get computed at the same time there's no need
    // to recompute them unless something changes.
    double cached_date_ = 0.0 ;          /// Cached date, observed params recomputed if this has changed
    double cached_azimuth_ = 0.0 ;       /// Cached azimuth (radians)
    double cached_zenith_ = 0.0 ;        /// Cached zenith (radians)
    double cached_altitude_ = 0.0 ;      /// Cached altitude (radians)
    double cached_hour_angle_ = 0.0 ;    /// Cached hour angle (radians)
    
    // Cache the apparent coordinates fo
    double cached_apparentxcoord_ = 0.0 ;
    double cached_apparentycoord_ = 0.0 ;
    
    bool DateHasChanged() ;
    
};

#endif /* CEObservation_h */

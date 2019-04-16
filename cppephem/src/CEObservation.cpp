/***************************************************************************
 *  CEObservation.cpp: CppEphem                                            *
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

/** \class CEObservation
 The CEObservation class serves as a constant link between an observer,
 a specified time, and a celestial body. This makes for a more convenient
 way of getting the current observed coordinates of an object. It also makes
 it easier when using a CERunningDate object as the input "date" object.
 */

#include <stdio.h>

#include "CEObservation.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
CEObservation::CEObservation()
{
    init_members();
}

/**********************************************************************//**
 * Constructor from a known observer, object, and date. If no date
 * is provided, the date of the observer will be used instead.
 *     @param observer         CEObserver linked with these coordinates
 *     @param body             CEBody being observed
 *     @param date             CEDate object linked with these coordinates.
 *************************************************************************/
CEObservation::CEObservation(CEObserver* observer, CEBody* body, CEDate* date)
{
    init_members();

    // Set the objects
    SetObserver(observer);
    SetBody(body);
    SetDate(date);
}


/**********************************************************************//**
 * Copy constructor
 * 
 * @param[in] other                CEObservation object to be copied
 *************************************************************************/
CEObservation::CEObservation(const CEObservation& other)
{
    init_members();
    copy_members(other);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
CEObservation::~CEObservation()
{
    free_members();
}


/**********************************************************************//**
 * Copy assignment operator
 * 
 * @param[in] other                CEObservation object to be copied
 * @return Pointer to this object post copy
 *************************************************************************/
CEObservation& CEObservation::operator=(const CEObservation& other)
{
    if (this != &other) {
        free_members();
        init_members();
        copy_members(other);
    }
    return *this;
}


/**********************************************************************//**
 * Returns both the azimuth and zenith angle of a given 'body_' as
 * observed by 'observer_' on the date given by 'date_'. This method
 * is the safest way of obtaining the azimuth,zenith coordinates when
 * using a CERunningDate object for 'date_'.
 *     @param[out] azimuth             Azimuth in degrees (radians)
 *     @param[out] zenith              Zenith in degrees (radians)
 *************************************************************************/
void CEObservation::GetAzimuthZenith_Rad(double *azimuth, double *zenith)
{
    *azimuth = GetAzimuth_Rad() ;
    *zenith  = GetZenith_Rad() ;
}

/**********************************************************************//**
 * Returns both the azimuth and zenith angle of a given 'body_' as
 * observed by 'observer_' on the date given by 'date_'. This method
 * is the safest way of obtaining the azimuth,zenith coordinates when
 * using a CERunningDate object for 'date_'.
 *     @param[out] azimuth             Azimuth in degrees (degrees)
 *     @param[out] zenith              Zenith in degrees (degrees)
 *************************************************************************/
void CEObservation::GetAzimuthZenith_Deg(double *azimuth, double *zenith)
{
    GetAzimuthZenith_Rad(azimuth, zenith) ;
    *azimuth *= DR2D ;
    *zenith  *= DR2D ;
}

/**********************************************************************//**
 * Returns both the observed x,y coordinates of a given 'body_' as
 * observed by 'observer_' on the date given by 'date_'. This method
 * is the safest way of obtaining these values when using a
 * CERunningDate object for 'date_'.
 * 
 * @param[out] apparent_X          Observed value of 'body_' x-coordinate (radians)
 * @param[out] apparent_Y          Observed value of 'body_' y-coordinate (radians)
 *************************************************************************/
void CEObservation::GetApparentXYCoordinate_Rad(double *apparent_X, double *apparent_Y)
{
    *apparent_X = GetApparentXCoordinate_Rad() ;
    *apparent_Y = cached_apparentycoord_ ;
}

/**********************************************************************//**
 * Returns both the observed x,y coordinates of a given 'body_' as
 * observed by 'observer_' on the date given by 'date_'. This method
 * is the safest way of obtaining these values when using a
 * CERunningDate object for 'date_'.
 * 
 * @param[out] apparent_X          Observed value of 'body_' x-coordinate (degrees)
 * @param[out] apparent_Y          Observed value of 'body_' y-coordinate (degrees)
 *************************************************************************/
void CEObservation::GetApparentXYCoordinate_Deg(double *apparent_X, double *apparent_Y)
{
    GetApparentXYCoordinate_Rad(apparent_X, apparent_Y) ;
    *apparent_X *= DR2D ;
    *apparent_Y *= DR2D ;
}


/**********************************************************************//**
 * Update the stored coordinates. Since all values need to be
 * computed at the same time, it only makes sense to update
 * all of the values every time they need updating.
 *************************************************************************/
bool CEObservation::UpdateCoordinates()
{
    if (NeedsUpdate()) {
        // Get the coordinates
        CECoordinates coords = observer_->ObservedPosition(*body_, *date_);
        
        // Update the cached parameters
        cached_date_     = *date_ ;
        cached_azimuth_  = coords.XCoordinate_Rad();
        cached_zenith_   = coords.YCoordinate_Rad();
    }    
    return true ;
}


/**********************************************************************//**
 * Copy data members from another object
 * 
 * @param[in] other             CEObservation object to copy from
 *************************************************************************/
void CEObservation::copy_members(const CEObservation& other)
{
    // Copy the pointers (this object does not own them)
    body_     = other.body_;
    date_     = other.date_;
    observer_ = other.observer_;

    // Copy the cached values
    cached_date_       = other.cached_date_;
    cached_azimuth_    = other.cached_azimuth_;
    cached_zenith_     = other.cached_zenith_;
    cached_hour_angle_ = other.cached_hour_angle_;
    cached_apparentxcoord_ = other.cached_apparentxcoord_;
    cached_apparentycoord_ = other.cached_apparentycoord_;
}


/**********************************************************************//**
 * Initialize the data members
 *************************************************************************/
void CEObservation::init_members(void)
{
    // Copy the pointers (this object does not own them)
    body_     = nullptr;
    date_     = nullptr;
    observer_ = nullptr;

    // Copy the cached values
    cached_date_           = 0.0;
    cached_azimuth_        = 0.0;
    cached_zenith_         = 0.0;

    // Note that these are not filled at the moment
    cached_hour_angle_     = 0.0;
    cached_apparentxcoord_ = 0.0;
    cached_apparentycoord_ = 0.0;
}


/**********************************************************************//**
 * Deallocate memory
 *************************************************************************/
void CEObservation::free_members(void)
{
    // This object is not responsible for its objects
}


/**********************************************************************//**
 * Check whether the date has changed since the last time
 * all of the parameters were updated (i.e. since the last
 * time UpdateCoordinates() was called)
 *************************************************************************/
bool CEObservation::NeedsUpdate(void)
{
    // Make sure the date object isnt nullptr
    if (date_ == nullptr) {
        return false ;
    }
    // Check if the date object has changed since the last time we querried it
    else if ((*date_) != cached_date_) {
        return true ;
    }
    // Otherwise we want to return false
    else {
        return false ;
    }
}

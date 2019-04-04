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
}

/**********************************************************************//**
 * Constructor from a known observer, object, and date. If no date
 * is provided, the date of the observer will be used instead.
 *     @param observer         CEObserver linked with these coordinates
 *     @param body             CEBody being observed
 *     @param date             CEDate object linked with these coordinates.
 *************************************************************************/
CEObservation::CEObservation(CEObserver* observer, CEBody* body, CEDate* date) :
    body_(body),
    date_(date),
    observer_(observer)
{
    UpdateCoordinates() ;
}

/**********************************************************************//**
 * Destructor
 *************************************************************************/
CEObservation::~CEObservation()
{
}

# pragma mark - Public Methods

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
    *zenith  = cached_zenith_ ;
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
    // Get the coordinate system of the body being observed
    CECoordinateType body_coords = body_->GetCoordSystem() ;
    
    // Compute the observed coordinates based on the coordinates of "body_"
    if (body_coords == CECoordinateType::CIRS) {
        // Convert from CIRS -> Observed
        CEBody::CIRS2Observed(body_->XCoordinate_Rad(*date_),
                              body_->YCoordinate_Rad(*date_),
                              &cached_azimuth_, &cached_zenith_,
                              *date_,
                              *observer_,
                              CEAngleType::RADIANS,
                              observer_->Wavelength_um(),
                              &cached_apparentxcoord_,
                              &cached_apparentycoord_,
                              &cached_hour_angle_) ;
    } else if (body_coords == CECoordinateType::GALACTIC) {
        // Convert Galactic -> Observerd
        CEBody::Galactic2Observed(body_->XCoordinate_Rad(*date_),
                              body_->YCoordinate_Rad(*date_),
                              &cached_azimuth_, &cached_zenith_,
                              *date_,
                              *observer_,
                              CEAngleType::RADIANS,
                              observer_->Wavelength_um(),
                              &cached_apparentxcoord_,
                              &cached_apparentycoord_,
                              &cached_hour_angle_) ;
    } else if (body_coords == CECoordinateType::ICRS) {
        // Convert CIRS -> Observed
        CEBody::ICRS2Observed(body_->XCoordinate_Rad(*date_),
                              body_->YCoordinate_Rad(*date_),
                              &cached_azimuth_, &cached_zenith_,
                              *date_,
                              *observer_,
                              CEAngleType::RADIANS,
                              observer_->Wavelength_um(),
                              &cached_apparentxcoord_,
                              &cached_apparentycoord_,
                              &cached_hour_angle_) ;
    } else {
        return false ;
    }
    
    // Update the cached_date_ object
    cached_date_ = *date_ ;
    
    return true ;
}

# pragma mark - Protected Methods

/**********************************************************************//**
 * Check whether the date has changed since the last time
 * all of the parameters were updated (i.e. since the last
 * time UpdateCoordinates() was called)
 *************************************************************************/
bool CEObservation::DateHasChanged()
{
    // Make sure the date object isnt nullptr
    if (date_ == nullptr) {
        return false ;
    }
    // Check if the date object has changed since the last time we querried it
    else if (*date_ != cached_date_) {
        return true ;
    }
    // Otherwise we want to return false
    else {
        return false ;
    }
}
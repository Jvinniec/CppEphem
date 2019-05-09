/***************************************************************************
 *  CEObserver.cpp: CppEphem                                               *
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

/** \class CEObserver
 CEObserver class is a self contained object for describing an observer.
 Information relevant to the observer includes:
 - Geographic longitude  (East positive)
 - Geographic latitude (North positive)
 - Elevation (meters above sea level)
 - Atmospheric pressure (hPa)
 - Temperature (Kelvin, Celsius, Fahrenheit)
 - Relative humidity (in range 0-1)
 
 The above are used in the SOFA software to more accurately calculate
 the affect of refraction through the atmosphere.
 */

// C++ HEADERS
#include <stdio.h>

// CPPEPHEM HEADERS
#include "CEObserver.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
CEObserver::CEObserver(void)
{
    init_members();
}


/**********************************************************************//**
 * Constructor from a geographic position (latitude, longitude, altitude)
 * Note that altitude defaults to sea-level
 * 
 * @param[in] longitude         Observer longitude (east positive)
 * @param[in] latitude          Observer latitude
 * @param[in] elevation         Observer elevation above sea-level (meters)
 * @param[in] angle_type        Angle type for lon,lat (default is degrees)
 *************************************************************************/
CEObserver::CEObserver(const double& longitude, 
                       const double& latitude,
                       const double& elevation, 
                       const CEAngleType& angle_type)
{
    init_members();

    // Use the internal methods for setting the values
    SetLongitude(longitude, angle_type) ;
    SetLatitude(latitude, angle_type) ;
    SetElevation(elevation) ;
}


/**********************************************************************//**
 * Copy constructor
 * 
 * @param[in] other            Another observer object that will be copied
 *************************************************************************/
CEObserver::CEObserver(const CEObserver& other)
{
    init_members();
    copy_members(other);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
CEObserver::~CEObserver(void)
{
    free_members();
}


/**********************************************************************//**
 * Copy assignment operator
 * 
 * @param[in] other             CEObserver object to be copied
 * @return Reference to this CEObserver object post-copy
 *************************************************************************/
CEObserver& CEObserver::operator=(const CEObserver& other)
{
    if (this != &other) {
        free_members();
        init_members();
        copy_members(other);
    }
    return *this;
}


/**********************************************************************//**
 * Get the local sky coordinates for an object as observed
 * by this observer
 * 
 * @param[in] object       Object that we want to find the local coordinates of
 * @return                 Local coordinates of the object
 *************************************************************************/
CECoordinates CEObserver::ObservedPosition(const CEBody& object,
                                           const CEDate& date)
{
    CECoordinates coords = object.ObservedCoords(date, *this);
    return this->ObservedPosition(coords, date);
}


/**********************************************************************//**
 * Get the local sky coordinates for an object as observed
 * by this observer
 * @param[in] coords       Coordinates of an object that we want to find the local coordinates of
 * @return                 'coords' converted into the local coordinates of this observer.
 *************************************************************************/
CECoordinates CEObserver::ObservedPosition(const CECoordinates& coords, 
                                           const CEDate&        date)
{
    // Compute the observed coordinates for these coordinates
    CECoordinates observed_coords = coords.GetObservedCoords(date,*this);
    return observed_coords;
}


/**********************************************************************//**
 * Returns the observers geocentric position in meters
 * @return Geocentric position of observer (in meters)
 * 
 * The position returned is in reference to the WGS84 coordinates
 *************************************************************************/
std::vector<double> CEObserver::PositionGeo(void) const
{
    // Get the position
    double xyzm[3];
    iauGd2gc(1, longitude_, latitude_, elevation_m_, xyzm);

    return std::vector<double>(&xyzm[0], &xyzm[0]+3);
}

/**********************************************************************//**
 * Get the position vector for this observer relative to CIRS (Earth center)
 * @param[in] date          Date object for computing position
 * @return Position vector relative to earth center position (AU)
 *************************************************************************/
std::vector<double> CEObserver::PositionCIRS(const CEDate& date) const
{
    UpdatePosVel(date);
    return pos_cirs_;
}


/**********************************************************************//**
 * Get the position vector for this observer relative to ICRS (solar system barycenter)
 * @param[in] date          Date object for computing position
 * @return Position vector relative to solar system barycenter position (AU)
 *************************************************************************/
std::vector<double> CEObserver::PositionICRS(const CEDate& date) const
{
    UpdatePosVel(date);
    return pos_icrs_;
}


/**********************************************************************//**
 * Get the velocity vector for this observer relative to CIRS (solar system barycenter)
 * @param[in] date          Date object for computing position
 * @return Velocity vector relative to Earth center position (AU)
 *************************************************************************/
std::vector<double> CEObserver::VelocityCIRS(const CEDate& date) const
{
    UpdatePosVel(date);
    return vel_cirs_;
}


/**********************************************************************//**
 * Get the velocity vector for this observer relative to ICRS (solar system barycenter)
 * @param[in] date          Date object for computing position
 * @return Velocity vector relative to solar system barycenter position (AU)
 *************************************************************************/
std::vector<double> CEObserver::VelocityICRS(const CEDate& date) const
{
    UpdatePosVel(date);
    return vel_icrs_;
}


/**********************************************************************//**
 * Returns a string containing information about this object
 * @return Formatted string containing information about this observer
 *************************************************************************/
std::string CEObserver::print(void) const
{
    // Fill in the returned string with formatted strings
    std::string msg("Observer:\n");
    msg += "   (Lon,Lat) = (" + std::to_string(Longitude_Deg()) + ", "  + 
                                std::to_string(Latitude_Deg()) + ") deg\n";
    msg += "   Elevation = " + std::to_string(Elevation_m()) + " m\n";
    msg += "   Temp      = " + std::to_string(Temperature_C()) + " C\n";
    msg += "   Pressure  = " + std::to_string(Pressure_hPa()) + " hPa\n";
    msg += "   Humidity  = " + std::to_string(RelativeHumidity()) + " %%\n";
    msg += "   Wavelength= " + std::to_string(Wavelength_um()) + " um\n";
    return msg;
}


/**********************************************************************//**
 * Initialize the data members
 *************************************************************************/
void CEObserver::free_members(void)
{
    // Nothing to do here
}


/**********************************************************************//**
 * Copy data members from another CEObserver object
 * 
 * @param[in] other         CEObserver object to copy from
 *************************************************************************/
void CEObserver::copy_members(const CEObserver& other)
{
    longitude_           = other.longitude_;
    latitude_            = other.latitude_;
    elevation_m_         = other.elevation_m_;
    pressure_hPa_        = other.pressure_hPa_;
    temperature_celsius_ = other.temperature_celsius_;
    wavelength_um_       = other.wavelength_um_;
    relative_humidity_   = other.relative_humidity_;
    utc_offset_          = other.utc_offset_;

    // Copy cached parameters
    cache_date_ = other.cache_date_;
    pos_cirs_   = other.pos_cirs_;
    pos_icrs_   = other.pos_icrs_;
    vel_cirs_   = other.vel_cirs_;
    vel_icrs_   = other.vel_icrs_;
}


/**********************************************************************//**
 * Initialize the data members
 *************************************************************************/
void CEObserver::init_members(void)
{
    longitude_           = 0.0;
    latitude_            = 0.0;
    elevation_m_         = 0.0;
    temperature_celsius_ = CppEphem::SeaLevelTemp_C();
    pressure_hPa_        = CppEphem::EstimatePressure_hPa(elevation_m_);
    relative_humidity_   = 0.0;
    wavelength_um_       = 0.5;
    utc_offset_          = CETime::SystemUTCOffset_hrs();

    // cached pos/vel parameters
    cache_date_ = -1.0e30;
    pos_cirs_   = std::vector<double>(3, 0.0);
    pos_icrs_   = std::vector<double>(3, 0.0);
    vel_cirs_   = std::vector<double>(3, 0.0);
    vel_icrs_   = std::vector<double>(3, 0.0);
}


/**********************************************************************//**
 * Update cached position/velocity vectors
 * 
 * @param[in] date          Date for computation
 *************************************************************************/
void CEObserver::UpdatePosVel(const CEDate& date) const
{
    // Check if the cached date has changed
    if (date.MJD() != cache_date_) {

        // Convert UTC to UT1 and TDB
        double ut11(0.0);
        double ut12(0.0);
        double tdb1(0.0);
        double tdb2(0.0);
        double tt1(0.0);
        double tt2(0.0);
        CEDate::UTC2UT1(date.MJD(), &ut11, &ut12);
        CEDate::UTC2TDB(date.MJD(), &tdb1, &tdb2);
        CEDate::UTC2TT(date.MJD(), &tt1, &tt2);

        // Compute the Earth rotation angle at UT1
        double theta = iauEra00(ut11, ut12);
        double sp    = iauSp00(tt1, tt2);

        // Vectors for intermediate position/velocities
        double pvc[2][3];   // CIRS pos,vel

        // Get the position and velocity values in CIRS
        iauPvtob(longitude_, 
                 latitude_, 
                 elevation_m_, 
                 date.xpolar(),
                 date.ypolar(),
                 sp,
                 theta,
                 pvc);

        // Earth centric distance/velocity in ICRS (AU and AU/day)
        double ebpv[2][3];
        double ehpv[2][3];
        iauEpv00(tdb1, tdb2, ehpv, ebpv);

        // Get the CIRS and ICRS pos,vel
        double mps_apd = CppEphem::sec_per_day() / CppEphem::m_per_au();
        for (int i=0; i<3; i++) {
            // CIRS
            pos_cirs_[i] = pvc[0][i] / CppEphem::m_per_au();
            vel_cirs_[i] = pvc[1][i] * mps_apd;

            // ICRS (Earth barycenter + CIRS offset)
            pos_icrs_[i] = ebpv[0][i] + pos_cirs_[i];
            vel_icrs_[i] = ebpv[1][i] + vel_cirs_[i];
        }
    }
    return;
}

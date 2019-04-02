/***************************************************************************
 *  CEObserver.h: CppEphem                                                 *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016 JCardenzana                                           *
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

#ifndef CEObserver_h
#define CEObserver_h

#include "CEBody.h"
#include "CEDate.h"
#include "CENamespace.h"

class CEObserver {
public:
//    CEObserver() ;
    CEObserver(const double& longitude  = 0.0, 
               const double& latitude   = 51.4778, 
               const double& elevation  = 0.0,
               CEAngleType   angle_type = CEAngleType::DEGREES) ;
    CEObserver(const CEObserver& other) ;
    virtual ~CEObserver() ;
    
    /****************************************************
     * Methods for accessing the underlying observer info
     ****************************************************/
    double              Longitude_Rad() const;
    double              Longitude_Deg() const;
    double              Latitude_Rad() const;
    double              Latitude_Deg() const;
    double              Elevation_m() const;
    double              Pressure_hPa() const;
    double              Temperature_C() const;
    double              Temperature_K() const;
    double              Temperature_F() const;
    double              RelativeHumidity() const;
    double              Wavelength_um() const;
    void                SetUTCOffset(const double& utc_offset);
    double              UTCOffset() const;
    std::vector<double> Time(const CEDate& date);
    std::vector<double> Time_UTC(const CEDate& date);
    
    /****************************************************
     * Methods for setting the underlying observer info
     ****************************************************/
    void SetElevation(const double& elevation=0.0);
    void SetLongitude(const double& longitude, 
                      CEAngleType angle_type=CEAngleType::RADIANS);
    void SetLatitude(const double& latitude, 
                     CEAngleType angle_type=CEAngleType::RADIANS);
    void SetGeographicCoordinates(const double& longitude,
                                  const double& latitude,
                                  CEAngleType angle_type=CEAngleType::RADIANS);
    void SetPressure(const double& pressure=CppEphem::EstimatePressure_hPa(CppEphem::SeaLevelTemp_C()));
    void SetRelativeHumidity(const double& humidity=0.0);
    void SetTemperature_C(const double& temp_C=CppEphem::SeaLevelTemp_C());
    void SetTemperature_K(const double& temp_K=CppEphem::SeaLevelTemp_K());
    void SetTemperature_F(const double& temp_F=CppEphem::SeaLevelTemp_F());
    void SetWavelength_um(const double& new_wavelength_um);
        
    /****************************************************
     * Methods for interacting with the sofa functions
     ****************************************************/
    
    CECoordinates ObservedPosition(const CEBody& object,
                                   const CEDate& date) ;
    CECoordinates ObservedPosition(const CECoordinates& coords,
                                   const CEDate&        date) ;
    
protected:
    // Variables which define the observers location on Earth
    double longitude_;              ///< Geographic longitude (radians)
    double latitude_;               ///< Geographic latitude (radians)
    double elevation_m_;            ///< Elevation (in meters) above sea-level

    // Variables defining the observers atmospheric conditions
    double pressure_hPa_;           ///< Atmospheric pressure (in units of hPa)
    double temperature_celsius_;    ///< Temperature in degrees celsius
    double relative_humidity_;      ///< Relative humidity (in range 0-1)
    double wavelength_um_;          ///< Observing wavelength (micrometers)
    
    // Variables defining the time of the observer
    double  utc_offset_;            ///< UTC offset in hours (set by default to system offset)
};


/**********************************************************************//**
 * Return observer geographic longitude in radians
 *************************************************************************/
inline
double CEObserver::Longitude_Rad() const
{
    return longitude_ ;
}


/**********************************************************************//**
 * Return observer geographic longitude in degrees
 *************************************************************************/
inline
double CEObserver::Longitude_Deg() const
{
    return longitude_ * DR2D ;
}


/**********************************************************************//**
 * Return geographic latitude in radians
 *************************************************************************/
inline
double CEObserver::Latitude_Rad() const
{
    return latitude_ ;
}


/**********************************************************************//**
 * Return geographic latitude in degrees
 *************************************************************************/
inline
double CEObserver::Latitude_Deg() const
{
    return latitude_ * DR2D ;
}


/**********************************************************************//**
 * Return altitude in meters above sea level
 *************************************************************************/
inline
double CEObserver::Elevation_m() const
{
    return elevation_m_ ;
}


/**********************************************************************//**
 * Return atmospheric pressure in units of hPa.
 *************************************************************************/
inline
double CEObserver::Pressure_hPa() const
{
    return pressure_hPa_ ;
}


/**********************************************************************//**
 * Return temperature in degrees Celsius.
 *************************************************************************/
inline
double CEObserver::Temperature_C() const
{
    return temperature_celsius_ ;
}


/**********************************************************************//**
 * Return temperature in Kelvin
 *************************************************************************/
inline
double CEObserver::Temperature_K() const
{
    return temperature_celsius_ + 273.15 ;
}


/**********************************************************************//**
 * Return temperature in degrees Fahrenheit
 *************************************************************************/
inline
double CEObserver::Temperature_F() const
{
    return temperature_celsius_ * (9.0/5.0) + 32.0 ;
}


/**********************************************************************//**
 * Return relative humidity
 *************************************************************************/
inline
double CEObserver::RelativeHumidity() const
{
    return relative_humidity_ ;
}


/**********************************************************************//**
 * Return the wavelength in units of micrometers
 *************************************************************************/
inline
double CEObserver::Wavelength_um() const
{
    return wavelength_um_ ;
}


/**********************************************************************//**
 * Set the UTC offset for the observers time
 *************************************************************************/
inline
void CEObserver::SetUTCOffset(const double& utc_offset) 
{
    utc_offset_ = utc_offset ;
}


/**********************************************************************//**
 *************************************************************************/
inline
double CEObserver::UTCOffset() const
{
    return utc_offset_ ;
}


/**********************************************************************//**
 * Get the current local time information (see CETime)
 * @param[in] date      A date object to convert to local time of this observer
 * @return Local time as a vector [H,M,S,seconds-fraction]
 *************************************************************************/
inline
std::vector<double> CEObserver::Time(const CEDate& date)
{
    double utc = CETime::UTC(date.JD() - utc_offset_);
    return CETime::TimeDbl2Vect( CETime::TimeSec2Time(utc) ) ;
}


/**********************************************************************//**
 * Get the current local time information (see CETime)
 * @param[in] date      A date object to convert to local time of this observer* @return time as vector
 *************************************************************************/
inline
std::vector<double> CEObserver::Time_UTC(const CEDate& date) 
{
    return CETime::TimeDbl2Vect(date.GetTime_UTC()) ;
}


/**********************************************************************//**
 * Set elevation in meters above sea level.
 *************************************************************************/
inline
void CEObserver::SetElevation(const double& elevation)
{
    elevation_m_ = elevation ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetLongitude(const double& longitude, 
                              CEAngleType angle_type)
{
    longitude_ = longitude * ((angle_type==CEAngleType::RADIANS) ? 1 : DD2R) ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetLatitude(const double& latitude, 
                             CEAngleType angle_type)
{
    latitude_ = latitude * ((angle_type==CEAngleType::RADIANS) ? 1 : DD2R) ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetPressure(const double& pressure)
{
    pressure_hPa_ = pressure ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetRelativeHumidity(const double& humidity)
{
    relative_humidity_ = humidity ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetTemperature_C(const double& temp_C)
{
    temperature_celsius_ = temp_C ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetTemperature_K(const double& temp_K)
{
    temperature_celsius_ = temp_K - 273.15 ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetTemperature_F(const double& temp_F)
{
    temperature_celsius_ = (temp_F - 32.0) * (5.0/9.0) ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetWavelength_um(const double& new_wavelength_um)
{
    wavelength_um_ = new_wavelength_um ;
}

#endif /* CEObserver_h */

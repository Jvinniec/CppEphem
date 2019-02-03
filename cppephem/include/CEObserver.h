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
    CEObserver(double longitude=0.0, double latitude=51.4778, double elevation=0.0,
               CEAngleType angle_type=CEAngleType::DEGREES,
               CEDate* date = nullptr) ;
    CEObserver(const CEObserver& other) ;
    virtual ~CEObserver() ;
    
    /****************************************************
     * Methods for accessing the underlying observer info
     ****************************************************/

    double              Longitude_Rad();
    double              Longitude_Deg();
    double              Latitude_Rad();
    double              Latitude_Deg();
    double              Elevation_m();
    double              Pressure_hPa();
    double              Temperature_C();
    double              Temperature_K();
    double              Temperature_F();
    double              RelativeHumidity();
    double              Wavelength_um();
    CEDate*             Date();
    void                SetUTCOffset(double utc_offset);
    double              UTCOffset();
    std::vector<double> Time();
    std::vector<double> Time_UTC();
    
    /****************************************************
     * Methods for setting the underlying observer info
     ****************************************************/

    void SetElevation(double elevation=0.0);
    void SetLongitude(double longitude, 
                      CEAngleType angle_type=CEAngleType::RADIANS);
    void SetLatitude(double latitude, 
                     CEAngleType angle_type=CEAngleType::RADIANS);
    void SetGeographicCoordinates(double longitude, double latitude,
                                  CEAngleType angle_type=CEAngleType::RADIANS);
    void SetPressure(double pressure=CppEphem::EstimatePressure_hPa(CppEphem::SeaLevelTemp_C()));
    void SetRelativeHumidity(double humidity=0.0);
    void SetTemperature_C(double temp_C=CppEphem::SeaLevelTemp_C());
    void SetTemperature_K(double temp_K=CppEphem::SeaLevelTemp_K());
    void SetTemperature_F(double temp_F=CppEphem::SeaLevelTemp_F());
    void SetWavelength_um(double new_wavelength_um);
    
    // Set the date
    void SetDate(CEDate* date=nullptr);
    
    /****************************************************
     * Methods for interacting with the sofa functions
     ****************************************************/
    
    CECoordinates ObservedPosition(CEBody& object) ;
    CECoordinates ObservedPosition(CECoordinates& coords) ;
    
protected:
    // Variables which define the observers location on Earth
    double longitude_ = 0.0 ;           ///< Geographic longitude (radians)
    double latitude_ = 0.0 ;            ///< Geographic latitude (radians)
    double elevation_m_ = 0.0;          ///< Elevation (in meters) above sea-level

    // Variables defining the observers atmospheric conditions
    double pressure_hPa_ = CppEphem::EstimatePressure_hPa(CppEphem::SeaLevelTemp_C()); ///< Atmospheric pressure (in units of hPa)
    double temperature_celsius_ = CppEphem::SeaLevelTemp_C();   ///< Temperature in degrees celsius
    double relative_humidity_  = 0.0;                           ///< Relative humidity (in range 0-1)
    double wavelength_um_ = 0.5 ;                               ///< Observing wavelength (micrometers)
    
    // Variables defining the time of the observer
    double utc_offset_ = CETime::SystemUTCOffset_hrs() ;          ///< UTC offset in hours (set by default to system offset)
    CEDate* current_date_ = nullptr;    ///< Current date for this object
    bool date_is_owned_ = false ;       ///< Boolean for whether 'current_date_' can be safely
                                        ///< deleted when this object is deleted
    
private:
    
};


/**********************************************************************//**
 * Return observer geographic longitude in radians
 *************************************************************************/
inline
double CEObserver::Longitude_Rad() 
{
    return longitude_ ;
}


/**********************************************************************//**
 * Return observer geographic longitude in degrees
 *************************************************************************/
inline
double CEObserver::Longitude_Deg() 
{
    return longitude_ * DR2D ;
}


/**********************************************************************//**
 * Return geographic latitude in radians
 *************************************************************************/
inline
double CEObserver::Latitude_Rad() 
{
    return latitude_ ;
}


/**********************************************************************//**
 * Return geographic latitude in degrees
 *************************************************************************/
inline
double CEObserver::Latitude_Deg()
{
    return latitude_ * DR2D ;
}


/**********************************************************************//**
 * Return altitude in meters above sea level
 *************************************************************************/
inline
double CEObserver::Elevation_m() 
{
    return elevation_m_ ;
}


/**********************************************************************//**
 * Return atmospheric pressure in units of hPa.
 *************************************************************************/
inline
double CEObserver::Pressure_hPa() 
{
    return pressure_hPa_ ;
}


/**********************************************************************//**
 * Return temperature in degrees Celsius.
 *************************************************************************/
inline
double CEObserver::Temperature_C() 
{
    return temperature_celsius_ ;
}


/**********************************************************************//**
 * Return temperature in Kelvin
 *************************************************************************/
inline
double CEObserver::Temperature_K()
{
    return temperature_celsius_ + 273.15 ;
}


/**********************************************************************//**
 * Return temperature in degrees Fahrenheit
 *************************************************************************/
inline
double CEObserver::Temperature_F()
{
    return temperature_celsius_ * (9.0/5.0) + 32.0 ;
}


/**********************************************************************//**
 * Return relative humidity
 *************************************************************************/
inline
double CEObserver::RelativeHumidity()
{
    return relative_humidity_ ;
}


/**********************************************************************//**
 * Return the wavelength in units of micrometers
 *************************************************************************/
inline
double CEObserver::Wavelength_um()
{
    return wavelength_um_ ;
}


/**********************************************************************//**
 * Get the date information (see CEDate)
 *************************************************************************/
inline
CEDate* CEObserver::Date()
{
    return current_date_ ;
}


/**********************************************************************//**
 * Set the UTC offset for the observers time
 *************************************************************************/
inline
void CEObserver::SetUTCOffset(double utc_offset) 
{
    utc_offset_ = utc_offset ;
}


/**********************************************************************//**
 *************************************************************************/
inline
double CEObserver::UTCOffset() 
{
    return utc_offset_ ;
}


/**********************************************************************//**
 * Get the current time information (see CETime)
 *************************************************************************/
inline
std::vector<double> CEObserver::Time()
{
    return CETime::TimeDbl2Vect( CETime::TimeSec2Time(CETime::UTC((*current_date_))) ) ;
}


/**********************************************************************//**
 * @return time as vector
 *************************************************************************/
inline
std::vector<double> CEObserver::Time_UTC() 
{
    return CETime::TimeDbl2Vect(current_date_->GetTime_UTC()) ;
}

/**********************************************************************//**
 * Set elevation in meters above sea level.
 *************************************************************************/
inline
void CEObserver::SetElevation(double elevation)
{
    elevation_m_ = elevation ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetLongitude(double longitude, CEAngleType angle_type)
{
    longitude_ = longitude * ((angle_type==CEAngleType::RADIANS) ? 1 : DD2R) ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetLatitude(double latitude, CEAngleType angle_type)
{
    latitude_ = latitude * ((angle_type==CEAngleType::RADIANS) ? 1 : DD2R) ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetPressure(double pressure)
{
    pressure_hPa_ = pressure ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetRelativeHumidity(double humidity)
{
    relative_humidity_ = humidity ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetTemperature_C(double temp_C)
{
    temperature_celsius_ = temp_C ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetTemperature_K(double temp_K)
{
    temperature_celsius_ = temp_K - 273.15 ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetTemperature_F(double temp_F)
{
    temperature_celsius_ = (temp_F - 32.0) * (5.0/9.0) ;
}


/**********************************************************************//**
 *************************************************************************/
inline
void CEObserver::SetWavelength_um(double new_wavelength_um)
{
    wavelength_um_ = new_wavelength_um ;
}

#endif /* CEObserver_h */

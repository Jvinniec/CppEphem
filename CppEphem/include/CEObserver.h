//
//  CEObserver.h
//  CppEphem
//
//  Created by Josh Cardenzana on 4/6/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#ifndef CEObserver_h
#define CEObserver_h

#include "CEBody.h"
#include "CEDate.h"
#include "CENamespace.h"

using namespace CppEphem ;

class CEObserver {
public:
    CEObserver() ;
//    CEObserver(double longitude, double latitude, double elevation=0.0,
//               CEAngleType angle_type=CEAngleType::RADIANS) ;
    CEObserver(double longitude, double latitude, double elevation=0.0,
               CEAngleType angle_type=CEAngleType::RADIANS,
               CEDate* date = nullptr) ;
    CEObserver(const CEObserver& other) ;
    virtual ~CEObserver() ;
    
    /****************************************************
     * Methods for accessing the underlying observer info
     ****************************************************/
    /// Return observer geographic longitude in radians
    double Longitude_Rad() {return longitude_ ;}
    /// Return observer geographic longitude in degrees
    double Longitude_Deg() {return longitude_ * DR2D ;}
    /// Return geographic latitude in radians
    double Latitude_Rad() {return latitude_ ;}
    /// Return geographic latitude in degrees
    double Latitude_Deg() {return latitude_ * DR2D ;}

    /// Return altitude in meters above sea level
    double Elevation_m() {return elevation_m_ ;}
    /// Return atmospheric pressure in units of hPa.
    double Pressure_hPa() {return pressure_hPa_ ;}
    /// Return temperature in degrees Celsius.
    double Temperature_C() {return temperature_celsius_ ;}
    /// Return temperature in Kelvin
    double Temperature_K() {return temperature_celsius_ + 273.15 ;}
    /// Return temperature in degrees Fahrenheit
    double Temperature_F() {return temperature_celsius_ * (9.0/5.0) + 32.0 ;}
    /// Return relative humidity
    double RelativeHumidity() {return relative_humidity_ ;}
    /// Return the wavelength in units of micrometers
    double Wavelength_um() {return wavelength_um_ ;}
    
    /// Get the date information (see CEDate)
    CEDate* Date() {return current_date_ ;}
    
    /// Get the current time information (see CETime)
    std::vector<double> Time() {return CETime::TimeDbl2Vect( CETime::TimeSec2Time(CETime::UTC((*current_date_))) ) ;}
    std::vector<double> Time_UTC() {return CETime::TimeDbl2Vect(current_date_->GetTime_UTC()) ;}
    
    /****************************************************
     * Methods for setting the underlying observer info
     ****************************************************/
    /// Set elevation in meters above sea level.
    void SetElevation(double elevation=0.0)
        {elevation_m_ = elevation ;}
    void SetLongitude(double longitude, CEAngleType angle_type=CEAngleType::RADIANS)
        {longitude_ = longitude * ((angle_type==CEAngleType::RADIANS) ? 1 : DD2R) ;}
    void SetLatitude(double latitude, CEAngleType angle_type=CEAngleType::RADIANS)
        {latitude_ = latitude * ((angle_type==CEAngleType::RADIANS) ? 1 : DD2R) ;}
    void SetGeographicCoordinates(double longitude, double latitude,
                                  CEAngleType angle_type=CEAngleType::RADIANS) ;

    // Weather related properties
    void SetPressure(double pressure=EstimatePressure_hPa(SeaLevelTemp_C()))
        {pressure_hPa_ = pressure ;}
    void SetRelativeHumidity(double humidity=0.0)
        {relative_humidity_ = humidity ;}
    void SetTemperature_C(double temp_C=SeaLevelTemp_C())
        {temperature_celsius_ = temp_C ;}
    void SetTemperature_K(double temp_K=SeaLevelTemp_K())
        {temperature_celsius_ = temp_K - 273.15 ;}
    void SetTemperature_F(double temp_F=SeaLevelTemp_F())
        {temperature_celsius_ = (temp_F - 32.0) * (5.0/9.0) ;}
    void SetWavelength_um(double new_wavelength_um)
        {wavelength_um_ = new_wavelength_um ;}
    
    // Set the date
    void SetDate(CEDate* date=nullptr) ;
    
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
    double pressure_hPa_ = EstimatePressure_hPa(SeaLevelTemp_C()); ///< Atmospheric pressure (in units of hPa)
    double temperature_celsius_ = SeaLevelTemp_C();                ///< Temperature in degrees celsius
    double relative_humidity_  = 0.0;                              ///< Relative humidity (in range 0-1)
    double wavelength_um_ = 0.5 ;                                  ///< Observing wavelength (micrometers)
    
    // Variables defining the time of the observer
    double utc_offset_ = 0.0 ;
    CEDate* current_date_ = nullptr;    ///< Current date for this object
    bool date_is_owned_ = false ;       ///< Boolean for whether 'current_date_' can be safely
                                        ///< deleted when this object is deleted
    
private:
    
};
#endif /* CEObserver_h */

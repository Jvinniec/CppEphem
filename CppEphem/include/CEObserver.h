//
//  CEObserver.h
//  CppEphem
//
//  Created by Josh Cardenzana on 4/6/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#ifndef CEObserver_h
#define CEObserver_h

#include "CEDate.h"
#include "CENamespace.h"

using namespace CppEphem ;

class CEObserver {
public:
    CEObserver() ;
    CEObserver(double longitude, double latitude, double elevation=0.0,
               CEAngleType angle_type=CEAngleType::RADIANS) ;
    CEObserver(double longitude, double latitude, double elevation,
               CEAngleType angle_type=CEAngleType::RADIANS,
               CEDate date=CEDate(DJ00, CEDateType::JD)) ;
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
    
    /// Get the date information (see CEDate)
    CEDate Date() {return current_date_ ;}
    
    /// Get the current time information (see CETime)
    CETime Time() {return current_time_ ;}
    
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
    void SetPressure(double pressure)
        {pressure_hPa_ = pressure ;}
    void SetRelativeHumidity(double humidity=0.0)
        {relative_humidity_ = humidity ;}
    void SetTemperature_C(double temp_C=SeaLevelTemp_C())
        {temperature_celsius_ = temp_C ;}
    void SetTemperature_K(double temp_K)
        {temperature_celsius_ = temp_K - 273.15 ;}
    void SetTemperature_F(double temp_F)
        {temperature_celsius_ = (temp_F - 32.0) * (5.0/9.0) ;}
    
    // Set the date
    void SetDate(double date, CEDateType date_type=CEDateType::JD)
        {current_date_ = CEDate(date, date_type) ;}
    void SetDate(const CEDate& date)
        {current_date_ = date ;}
    
    // Set the time
    void SetTime(double time, CETimeType time_type=CETimeType::UTC)
        {current_time_ = CETime(time, time_type) ;}
    void SetTime(std::vector<double> time, CETimeType time_type=CETimeType::UTC)
        {current_time_ = CETime(time, time_type) ;}
    void SetTime(const CETime& time)
        {current_time_ = time ;}
    
    /****************************************************
     * Methods for interacting with the sofa functions
     ****************************************************/
    
    
protected:
    // Variables which define the observer
    double longitude_ ;                 // Geographic longitude (radians)
    double latitude_ ;                  // Geographic latitude (radians)
    double elevation_m_ ;               // Elevation (in meters) above sea-level
    double pressure_hPa_ ;              // Atmospheric pressure (in units of hPa)
    double temperature_celsius_ ;       // Temperature in degrees celsius
    double relative_humidity_ ;         // Relative humidity (in range 0-1)
    CETime current_time_ ;              // Current UTC time at the position in question
    CEDate current_date_ ;              // Current date for this object
    
private:
    
};
#endif /* CEObserver_h */

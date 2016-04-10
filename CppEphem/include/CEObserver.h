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
    // Default constructor
    CEObserver() ;
    // Constructor from a geographic position (latitude, longitude, altitude)
    // Note that altitude defaults to sea-level
    CEObserver(double longitude, double latitude, double elevation=0.0,
               CEAngleType angle_type=CEAngleType::RADIANS) ;
    CEObserver(double longitude, double latitude, double elevation,
               CEAngleType angle_type=CEAngleType::RADIANS,
               CEDate date=CEDate(DJ00, CEDateType::JD)) ;
    // Copy constructor
    CEObserver(const CEObserver& other) ;
    // Destructor
    virtual ~CEObserver() ;
    
    /****************************************************
     * Methods for accessing the underlying observer info
     ****************************************************/
    // Get the Longitude/Latitude
    double Longitude_Rad() {return longitude_ ;}
    double Longitude_Deg() {return longitude_ * DR2D ;}
    double Latitude_Rad() {return latitude_ ;}
    double Latitude_Deg() {return latitude_ * DR2D ;}

    // Get the altitude (in meters) above sea-level
    double Elevation() {return elevation_ ;}
    double Pressure() {return pressure_hPa_ ;}
    double Temperature_C() {return temperature_celsius_ ;}
    double Temperature_K() {return temperature_celsius_ + 273.15 ;}
    double Temperature_F() {return temperature_celsius_ * (9.0/5.0) + 32.0 ;}
    double RelativeHumidity() {return relative_humidity_ ;}
    
    // Get the date information
    CEDate Date() {return current_date_ ;}
    double JD() {return current_date_.JD() ;}
    
    // Get the current time vector
    CETime Time() {return current_time_ ;}
    std::vector<double> TimeVect() {return std::vector<double>(0) ;}
//    std::vector<double> UTC() {return current_time_.UTC() ;}
    
    /****************************************************
     * Methods for setting the underlying observer info
     ****************************************************/
    // Geographic related properties
    void SetElevation(double elevation=0.0)
        {elevation_ = elevation ;}
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
    void SetTemperature_C(double temp_C=0.0)
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
    double elevation_ ;                 // Elevation (in meters) above sea-level
    double pressure_hPa_ ;              // Atmospheric pressure (in units of hPa)
    double temperature_celsius_ ;       // Temperature in degrees celsius
    double relative_humidity_ ;         // Relative humidity (in range 0-1)
    CETime current_time_ ;              // Current UTC time at the position in question
    CEDate current_date_ ;              // Current date for this object
    
private:
    
};
#endif /* CEObserver_h */

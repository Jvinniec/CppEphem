/***************************************************************************
 *  CEObserver.i: CppEphem                                                 *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2019 JCardenzana                                           *
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

/**
 * @file CEObserver.i
 * @brief CppEphem observer description class SWIG file
 * @author JCardenzana
 */
%{
/* Put headers and other declarations here that are needed for compilation */
// CppEphem HEADERS
#include "CEObserver.h"
%}


/***********************************************************************//**
 * @class CEObserver
 *
 * @brief CEObserver class SWIG interface definition
 ***************************************************************************/
class CEObserver {
public:
    CEObserver(void) ;
    CEObserver(const double& longitude, 
               const double& latitude, 
               const double& elevation,
               const CEAngleType& angle_type = CEAngleType::DEGREES) ;
    CEObserver(const CEObserver& other) ;
    virtual ~CEObserver(void) ;
    
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
                      const CEAngleType&  angle_type=CEAngleType::RADIANS);
    void SetLatitude(const double& latitude, 
                     const CEAngleType&  angle_type=CEAngleType::RADIANS);
    void SetGeoCoordinates(const double& longitude,
                           const double& latitude,
                           const CEAngleType&  angle_type=CEAngleType::RADIANS);
    void SetPressure_hPa(const double& pressure=CppEphem::EstimatePressure_hPa(CppEphem::SeaLevelTemp_C()));
    void SetRelativeHumidity(const double& humidity=0.0);
    void SetTemperature_C(const double& temp_C=CppEphem::SeaLevelTemp_C());
    void SetTemperature_K(const double& temp_K=CppEphem::SeaLevelTemp_K());
    void SetTemperature_F(const double& temp_F=CppEphem::SeaLevelTemp_F());
    void SetWavelength_um(const double& new_wavelength_um);
    
    /****************************************************
     * Methods for getting observer position and velocity
     * vectors relative to CIRS and ICRS coordinates
     ****************************************************/
    std::vector<double> PositionGeo(void) const;
    std::vector<double> PositionCIRS(const CEDate& date) const;
    std::vector<double> PositionICRS(const CEDate& date) const;
    std::vector<double> VelocityCIRS(const CEDate& date) const;
    std::vector<double> VelocityICRS(const CEDate& date) const;

    const std::string describe() const;
};


/***********************************************************************//**
 * @brief CEObserver class extension
 ***************************************************************************/
%extend CEObserver {

    %pythoncode {
        def __str__(self):
            """
            Returns a description of the observer
            """
            return (self.describe());
    }
}
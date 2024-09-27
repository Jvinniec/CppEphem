/***************************************************************************
 *  CEObservation.i: CppEphem                                              *
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
 * @file CEObservation.i
 * @brief CppEphem observer description class SWIG file
 * @author JCardenzana
 */
%{
/* Put headers and other declarations here that are needed for compilation */
// CppEphem HEADERS
#include "CEObservation.h"
%}


/***********************************************************************//**
 * @class CEObservation
 *
 * @brief CEObservation class SWIG interface definition
 ***************************************************************************/
class CEObservation : public CEBase {
public:
    // Constructors
    CEObservation() ;
    CEObservation(CEObserver* observer, CEBody* body, CEDate* date);
    CEObservation(const CEObservation& other);
    virtual ~CEObservation() ;
    
    // Setting underlying object pointers
    virtual void   SetObserver(CEObserver* new_observer);
    virtual void   SetBody(CEBody* new_body);
    virtual void   SetDate(CEDate* new_date);
    
    /// Access the underlying objects
    CEObserver*    Observer();
    CEBody*        Body();
    CEDate*        Date();
    virtual double GetAzimuth_Rad();
    virtual double GetAzimuth_Deg();
    virtual double GetZenith_Rad();
    virtual double GetZenith_Deg();
    virtual double GetAltitude_Rad();
    virtual double GetAltitude_Deg();
    virtual double GetHourAngle_Rad();
    virtual double GetHourAngle_Deg();
    virtual double GetApparentXCoordinate_Rad();
    virtual double GetApparentXCoordinate_Deg();
    virtual double GetApparentYCoordinate_Rad();
    virtual double GetApparentYCoordinate_Deg();
    virtual void   GetAzimuthZenith_Rad(double *azimuth, double *zenith);
    virtual void   GetAzimuthZenith_Deg(double *azimuth, double *zenith);
    virtual void   GetApparentXYCoordinate_Rad(double *apparent_X, double *apparent_Y);
    virtual void   GetApparentXYCoordinate_Deg(double *apparent_X, double *apparent_Y);
    bool           UpdateCoordinates();
    
    // Necessary methods
    const std::string ClassName(void) const;
    const std::string describe(void) const;
};
/***************************************************************************
 *  CEBody.i: CppEphem                                                     *
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
 * @file CEBody.i
 * @brief SWIG file for CEBody class
 * @author JCardenzana
 */
%{
/* Put headers and other declarations here that are needed for compilation */
// CppEphem HEADERS
#include "CEBody.h"
%}

/***********************************************************************//**
 * @class CEBody
 *
 * @brief CEBody class SWIG interface definition
 ***************************************************************************/
class CEBody : public CESkyCoord {
public:
    /************************************
     * Basic constructors & destructors
     ***********************************/
    CEBody() ;
    CEBody(const std::string&      name, 
           const CEAngle&          xcoord, 
           const CEAngle&          ycoord,
           const CESkyCoordType&   coord_type=CESkyCoordType::ICRS);
    CEBody(const CEBody&      other, 
           const std::string& name="");
    CEBody(const CESkyCoord&  coords, 
           const std::string& name="");
    virtual ~CEBody();
    
    /************************************
     * Public methods
     ***********************************/

    virtual CESkyCoord GetCoordinates(const CEDate& date=CEDate::CurrentJD()) const;
    virtual CESkyCoord ObservedCoords(const CEDate&     date,
                                      const CEObserver& observer) const;
    std::string   Name(void) const;
    void          SetName(const std::string& new_name);

    // Necessary methods
    const std::string ClassName(void) const;
    const std::string describe(void) const;
};

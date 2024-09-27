/***************************************************************************
 *  CERunningDate.i: CppEphem                                              *
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
 * @file CERunningDate.i
 * @brief SWIG file for CERunningDate class
 * @author JCardenzana
 */
%{
/* Put headers and other declarations here that are needed for compilation */
// CppEphem HEADERS
#include "CERunningDate.h"
%}

/***********************************************************************//**
 * @class CERunningDate
 *
 * @brief CERunningDate class SWIG interface definition
 ***************************************************************************/
class CERunningDate : public CEDate {
public:
    CERunningDate() ;
    CERunningDate(const CERunningDate& other);
    virtual ~CERunningDate() ;
    
    // Some overloaded methods to make sure that the current
    // values are obtained first
    virtual double JD() const;
    virtual double MJD() const;
    virtual double Gregorian() const;
    
    /// Method for getting the number of seconds since this object was created or reset
    virtual double RunTime(void) const;
    double         ScaledRunTime(void) const;
    void           ResetTime(void);
    virtual double GetTimerSpeed(void) const;
    virtual void   SetTimerSpeed(const double& scale=1.0);

};

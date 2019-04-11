/***************************************************************************
 *  CENamespace.cpp: CppEphem                                              *
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

#include <cmath>
#include <stdio.h>
#include "CENamespace.h"

void CppEphem::SetCorrFilename(const std::string& filename)
{
    CppEphem::corrections.SetFilename(filename);
}

/**********************************************************************//**
 * Return dut1 based on a given modified julian date
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return UT1-UTC correction for a given MJD
 *************************************************************************/
double CppEphem::dut1(const double& mjd) 
{
    double dut1(0.0);

    // Fill dut1 if support dir has been defined
    dut1 = CppEphem::corrections.dut1(mjd);

    return dut1;
}

/**********************************************************************//**
 *************************************************************************/
double CppEphem::dut1Error(const double& mjd) 
{
    return 0.0;
}

/**********************************************************************//**
 * Set the time from a vector representing Greenwich Apparent Sidereal Time.
 * The elements are as follows:
 * [0]=hours, [1]=minutes, [2]=whole seconds, [3]=fractional seconds
 *************************************************************************/
double CppEphem::dut1Calc(const double& mjd)
{
    // There's a formula that could be used for calculating this value,
    // but I'm a bit lazy at the moment and since the correction is
    // very small, I'm just going to return 0 for the time being.
    return 0.0 ;
}


/**********************************************************************//**
 * Polar motion (x) for a given modified julian date
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return x-polar motion correction for a given MJD
 *************************************************************************/
double CppEphem::xp(const double& mjd) 
{
    return corrections.xpolar(mjd) ;
}


/**********************************************************************//**
 * Polar motion (x) for a given modified julian date
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return x-polar motion correction for a given MJD
 *************************************************************************/
double CppEphem::yp(const double& mjd) 
{
    return corrections.ypolar(mjd);
}

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

/**********************************************************************//**
 * Return dut1 based on MJD
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 *************************************************************************/
double CppEphem::dut1(double mjd) {
    // Round the MJD down to the nearest integer
    mjd = std::floor(mjd) ;
    
    // If the passed mjd is greater than the mininmum mjd or
    // larger than the largest mjd stored, then use the formula
    if (mjd < MINMJDFORDUT1) return 0.0 ;
    if (mjd > MAXMJDFORDUT1) return dut1Calc(mjd) ;
    
    // Load the list of dut1 values
    return dut1_list()[mjd] ;
}

/**********************************************************************//**
 *************************************************************************/
double CppEphem::dut1Error(double mjd) {
    // If the passed mjd is greater than the mininmum mjd or
    // larger than the largest mjd stored, then use the formula
    if (mjd < MINMJDFORDUT1) return 0.0 ;
    if (mjd > MAXMJDFORDUT1) return 0.0 ;
    
    // Load the list of dut1 values
    std::map<int, double> dut1Error = dut1Error_list() ;
    
    // Round the MJD down to the nearest integer
    mjd = std::floor(mjd) ;
    
    return dut1Error[mjd] ;
}

/**********************************************************************//**
 * Set the time from a vector representing Greenwich Apparent Sidereal Time.
 * The elements are as follows:
 * [0]=hours, [1]=minutes, [2]=whole seconds, [3]=fractional seconds
 *************************************************************************/
double CppEphem::dut1Calc(double mjd)
{
    // There's a formula that could be used for calculating this value,
    // but I'm a bit lazy at the moment and since the correction is
    // very small, I'm just going to return 0 for the time being.
    return 0.0 ;
}

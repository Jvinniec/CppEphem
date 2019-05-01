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
 * Return the name of the file used for defining the corrections
 * 
 * @return Filename of corrections file
 *************************************************************************/
std::string CppEphem::NutationFile(void)
{
    return CppEphem::corrections.NutationFile();
}


/**********************************************************************//**
 * Return the name of the file used for defining the corrections
 * 
 * @return Filename of corrections file
 *************************************************************************/
std::string CppEphem::TtUt1HistFile(void)
{
    return CppEphem::corrections.TtUt1HistFile();
}


/**********************************************************************//**
 * Return the name of the file used for defining the corrections
 * 
 * @return Filename of corrections file
 *************************************************************************/
std::string CppEphem::TtUt1PredFile(void)
{
    return CppEphem::corrections.TtUt1PredFile();
}


/**********************************************************************//**
 * Set the name of the file to use for defining the nutation corrections
 * 
 * @param[in] filename          Filename of corrections file
 *************************************************************************/
void CppEphem::SetNutationFile(const std::string& filename)
{
    CppEphem::corrections.SetNutationFile(filename);
}


/**********************************************************************//**
 * Set the name of the file to use for defining the historic TT-UT1 corrections
 * 
 * @param[in] filename          Filename of corrections file
 *************************************************************************/
void CppEphem::SetTtUt1HistFile(const std::string& filename)
{
    CppEphem::corrections.SetTtUt1HistFile(filename);
}


/**********************************************************************//**
 * Set the name of the file to use for defining the predicted TT-UT1 corrections
 * 
 * @param[in] filename          Filename of corrections file
 *************************************************************************/
void CppEphem::SetTtUt1PredFile(const std::string& filename)
{
    CppEphem::corrections.SetTtUt1PredFile(filename);
}


/**********************************************************************//**
 * Set the corrections object to use interpolation
 * 
 * @param[in] set_interp        Specifiy whether or not to use interpolation
 *************************************************************************/
void CppEphem::CorrectionsInterp(bool set_interp)
{
    CppEphem::corrections.SetInterp(set_interp);
}


/**********************************************************************//**
 * Return dut1 based on a given modified julian date (seconds)
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return UT1-UTC correction for a given MJD (seconds)
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
 * Polar motion (x) for a given modified julian date (radians)
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return x-polar motion correction for a given MJD (radians)
 *************************************************************************/
double CppEphem::xp(const double& mjd) 
{
    return corrections.xpolar(mjd) ;
}


/**********************************************************************//**
 * Polar motion (x) for a given modified julian date (radians)
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return x-polar motion correction for a given MJD (radians)
 *************************************************************************/
double CppEphem::yp(const double& mjd) 
{
    return corrections.ypolar(mjd);
}


/**********************************************************************//**
 * Earth obliquity correction for a given modified julian date (radians)
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return Earth obliquity correction for a given MJD (radians)
 *************************************************************************/
double CppEphem::deps(const double& mjd)
{
    return corrections.deps(mjd);
}


/**********************************************************************//**
 * Earth longitude correction for a given modified julian date (radians)
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return Earth longitude correction for a given MJD (radians)
 *************************************************************************/
double CppEphem::dpsi(const double& mjd)
{
    return corrections.dpsi(mjd);
}


/**********************************************************************//**
 * TT-UT1 correction for a given MJD (seconds)
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return TT-UT1 correction for a given MJD (seconds)
 *************************************************************************/
double CppEphem::ttut1(const double& mjd)
{
    return corrections.ttut1(mjd);
}

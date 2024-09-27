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
#include <iomanip>
#include <stdio.h>
#include <sstream>
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
 * @param[in] set_interp        Specify whether or not to use interpolation
 *************************************************************************/
void CppEphem::CorrectionsInterp(bool set_interp)
{
    CppEphem::corrections.SetInterp(set_interp);
}


/**********************************************************************//**
 * Turn on/off application of nutation correction values
 * 
 * @param[in] use_nut          Whether or not to use nutation corrections
 *************************************************************************/
void CppEphem::UseNutation(bool use_nut)
{
    CppEphem::use_nutation_ = use_nut;
}


/**********************************************************************//**
 * Turn on/off application of TT-UT1 correction values
 * 
 * @param[in] use_ttut1          Whether or not to use TT-UT1 corrections
 *************************************************************************/
void CppEphem::UseTtUt1(bool use_ttut1)
{
    CppEphem::use_nutation_ = use_ttut1;
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
    if (use_nutation_) {
        dut1 = CppEphem::corrections.dut1(mjd);
    }

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
    double xpolar(0.0);
    if (use_nutation_) {
        xpolar = corrections.xpolar(mjd);
    }
    return xpolar;
}


/**********************************************************************//**
 * Polar motion (x) for a given modified julian date (radians)
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return x-polar motion correction for a given MJD (radians)
 *************************************************************************/
double CppEphem::yp(const double& mjd) 
{
    double ypolar(0.0);
    if (use_nutation_) {
        ypolar = corrections.ypolar(mjd);
    }
    return ypolar;
}


/**********************************************************************//**
 * Earth obliquity correction for a given modified julian date (radians)
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return Earth obliquity correction for a given MJD (radians)
 *************************************************************************/
double CppEphem::deps(const double& mjd)
{
    double deps(0.0);
    if (use_nutation_) {
        deps = corrections.deps(mjd);
    }
    return deps;
}


/**********************************************************************//**
 * Earth longitude correction for a given modified julian date (radians)
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return Earth longitude correction for a given MJD (radians)
 *************************************************************************/
double CppEphem::dpsi(const double& mjd)
{
    double dpsi(0.0);
    if (use_nutation_) {
        dpsi = corrections.dpsi(mjd);
    }
    return dpsi;
}


/**********************************************************************//**
 * TT-UT1 correction for a given MJD (seconds)
 * 
 * @param[in] mjd       Modified Julian Date (MJD)
 * @return TT-UT1 correction for a given MJD (seconds)
 *************************************************************************/
double CppEphem::ttut1(const double& mjd)
{
    double ttut1(0.0);
    if (use_ttut1_) {
        ttut1 = corrections.ttut1(mjd);
    }
    return ttut1;
}


/**********************************************************************//**
 * Method for splitting a string based on some delimiter into a vector of strings
 * 
 * @param[in]  s
 * @param[in]  delim
 * @param[out] elems
 *************************************************************************/
void CppEphem::StrOpt::split(const std::string&        s, 
                             const char&               delim, 
                             std::vector<std::string>* elems) 
{
    std::stringstream ss(s);
    std::string item=std::string();
    while (std::getline(ss, item, delim)) {
        elems->push_back(item);
    }
}


/**********************************************************************//**
 * Method for splitting a string based on some delimiter into a vector of strings
 *************************************************************************/
std::vector<std::string> CppEphem::StrOpt::split(const std::string& s, 
                                                 const char&        delim) 
{
    std::vector<std::string> elems;
    split(s, delim, &elems);
    return elems;
}


/**********************************************************************//**
 * Method for joining a vector of values based on some delimiter into a string
 * 
 * @param[in] values            Vector of values to be joined
 * @param[in] delim             Delimiter to use between values
 *************************************************************************/
template <typename T>
std::string CppEphem::StrOpt::join(const std::vector<T>& values,
                                   const char&           delim)
{
    // Assemble the first value in the string
    std::ostringstream ss;
    if (values.size() > 0) {
        ss << values[0];
    }

    // Now append the rest
    for (int i=1; i<values.size(); i++) { 
        ss << delim << values[i];
    }

    return ss.str();
}
// Define the typical use cases of CppEphem::StrOpt::join
template std::string CppEphem::StrOpt::join<std::string>(
    const std::vector<std::string>&, const char&);
template std::string CppEphem::StrOpt::join<double>(
    const std::vector<double>&, const char&);



/**********************************************************************//**
 * Method for splitting a string based on some delimiter into a vector of strings
 *************************************************************************/
std::string CppEphem::StrOpt::join_angle(const std::vector<double>& values,
                                         const char&                delim)
{
    // Make sure there are only three values
    double sec = values[2];
    if (values.size() == 4) {
        sec += values[3];
    }

    // Assemble the string using the specified delimiter
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << int(values[0]);
    ss << delim;
    ss << std::setfill('0') << std::setw(2) << int(values[1]);
    ss << delim;
    ss << std::setfill('0') << std::setw(11) << std::fixed 
       << std::setprecision(8) << sec;
    
    return std::string(ss.str());
}

/***************************************************************************
 *  test_CENamespace.cpp: CppEphem                                         *
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

/** \class test_CENamespace
 Defines tests for CENamespace
 */

#include <iostream>

#include "test_CENamespace.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CENamespace::test_CENamespace() :
    CETestSuite()
{}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CENamespace::~test_CENamespace()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CENamespace::runtests()
{
    std::cout << "\nTesting CENamespace:\n";

    // Run each of the tests
    test_SeaLevelVals();
    test_Conversions();
    test_Corrections();

    return pass();
}


/**********************************************************************//**
 * Tests return values for temperature at sea level
 *  @return Status of tests
 *************************************************************************/
bool test_CENamespace::test_SeaLevelVals(void)
{
    // Check the temperature at sea level
    double sea_level_temp_K = 288.2;
    test_double(CppEphem::SeaLevelTemp_K(), sea_level_temp_K, __func__, __LINE__);

    // Convert 
    double sea_level_temp_C = sea_level_temp_K - 273.15;
    test_double(CppEphem::SeaLevelTemp_C(), sea_level_temp_C, __func__, __LINE__);

    double sea_level_temp_F = (9.0/5.0) * sea_level_temp_C + 32.0;
    test_double(CppEphem::SeaLevelTemp_F(), sea_level_temp_F, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Tests return values for basic unit conversion constants
 *  @return Status of tests
 *************************************************************************/
bool test_CENamespace::test_Conversions()
{
    // Test julian date 2000
    test_double(CppEphem::julian_date_J2000(), DJ00, __func__, __LINE__);

    // Speed of light (m/sec)
    test_double(CppEphem::c(), DC * DAU / DAYSEC, __func__, __LINE__);

    // Speed of light (AU/day)
    test_double(CppEphem::c_au_per_day(), DC, __func__, __LINE__);

    // Meters per AU
    test_double(CppEphem::m_per_au(), DAU, __func__, __LINE__);

    // Seconds per day
    test_double(CppEphem::sec_per_day(), DAYSEC, __func__, __LINE__);

    // Test temperature conversions
    double temp_K(0.0);
    double temp_C(-273.15);
    double temp_F(-459.67);
    // Reduce accuracy for the tests
    double old_tol = DblTol();
    SetDblTol(1.0e-11);
    test_double(CppEphem::Temp_C2F(temp_C), temp_F, __func__, __LINE__);
    test_double(CppEphem::Temp_C2K(temp_C), temp_K, __func__, __LINE__);
    test_double(CppEphem::Temp_F2C(temp_F), temp_C, __func__, __LINE__);
    test_double(CppEphem::Temp_F2K(temp_F), temp_K, __func__, __LINE__);
    test_double(CppEphem::Temp_K2C(temp_K), temp_C, __func__, __LINE__);
    test_double(CppEphem::Temp_K2F(temp_K), temp_F, __func__, __LINE__);
    SetDblTol(old_tol);

    return pass();
}


/**********************************************************************//**
 * Tests basic coordinate conversion correction values
 *  @return Status of tests
 *************************************************************************/
bool test_CENamespace::test_Corrections()
{
    double mjd(51544.5);

    // Get dut1 for J2000 (and test interpolation)
    CppEphem::CorrectionsInterp(false);
    test_double(CppEphem::dut1(mjd), 0.355499, __func__, __LINE__);
    CppEphem::CorrectionsInterp(true);
    test_double(CppEphem::dut1(mjd), 0.355066, __func__, __LINE__);

    // Turn off interpolation for future tests
    CppEphem::CorrectionsInterp(false);

    test_double(CppEphem::dut1Error(mjd), 0.0, __func__, __LINE__);
    test_double(CppEphem::dut1Calc(mjd), 0.0, __func__, __LINE__);
    
    // Test x,y polar motion
    test_double(CppEphem::xp(mjd), 0.043190 * DAS2R, __func__, __LINE__);
    test_double(CppEphem::yp(mjd), 0.377700 * DAS2R, __func__, __LINE__);

    // Test dpsi,deps corrections
    test_double(CppEphem::dpsi(mjd), -0.252*DMAS2R, __func__, __LINE__);
    test_double(CppEphem::deps(mjd), -0.119*DMAS2R, __func__, __LINE__);

    // Estimate altitude/pressure based on pressure
    test_double(CppEphem::EstimateAltitude_m(1013.25), 0.0, __func__, __LINE__);
    test_double(CppEphem::EstimatePressure_hPa(0.0), 1013.25, __func__, __LINE__);

    // Test the name of the corrections files
    std::string file_path(CECORRFILEPATH);
    std::string nut_file = file_path + "/nutation.txt";
    std::string ttut1_hist = file_path + "/ttut1_historic.txt";
    std::string ttut1_pred = file_path + "/ttut1_predicted.txt";
    
    test_string(CppEphem::NutationFile(), nut_file, __func__, __LINE__);
    test_string(CppEphem::TtUt1HistFile(), ttut1_hist, __func__, __LINE__);
    test_string(CppEphem::TtUt1PredFile(), ttut1_pred, __func__, __LINE__);

    // Test setting the corrections filename
    std::string newfilename("testfilename.txt");
    CppEphem::SetNutationFile(newfilename);
    CppEphem::SetTtUt1HistFile(newfilename);
    CppEphem::SetTtUt1PredFile(newfilename);
    test_string(CppEphem::NutationFile(), newfilename, __func__, __LINE__);
    test_string(CppEphem::TtUt1HistFile(), newfilename, __func__, __LINE__);
    test_string(CppEphem::TtUt1PredFile(), newfilename, __func__, __LINE__);

    // Reset the filenames
    CppEphem::SetNutationFile(nut_file);
    CppEphem::SetTtUt1HistFile(ttut1_hist);
    CppEphem::SetTtUt1PredFile(ttut1_pred);

    return pass();
}

/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) 
{    
    test_CENamespace tester;
    return (!tester.runtests());
}

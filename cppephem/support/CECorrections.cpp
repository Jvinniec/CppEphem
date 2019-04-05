/***************************************************************************
 *  CECorrections.h: CppEphem                                              *
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

/** \class CECorrections
 CECorrections class handles downloading and retrieving corrections from the 
 IERS bulletins. Current corrections obtainable:
    - DUT1
    - X/Y polar motions
 
 The way this is done is by downloading the corrections data on the fly from:
    http://maia.usno.navy.mil/ser7/finals2000A.all
 The downloaded file will be stored locally to prevent the need to re-download 
 it every time it is needed in the future.

 IMPORTANT NOTE: These correction values should only be accessed through the 
 CppEphem namespace, and not by directly querying this class. This prevents
 creating multiple CECorrections objects that could be resource intensive.
 */

#include "CECorrections.h"
#include "CEException.h"
#include <sofam.h>
#include <curl/curl.h>
#include <exception>
#include <iostream>
#include <cmath>
#include <fstream>

#ifndef CESUPPORTDIR
#define CESUPPORTDIR std::string("")
#endif


/**********************************************************************//**
 * Constructor for coordinate corrections object
 *************************************************************************/
CECorrections::CECorrections() :
    outfilename_(CESUPPORTDIR + "corrections.txt"),
    corrections_(std::map<int, std::vector<double>>()),
    min_mjd_(1000000),
    max_mjd_(-1)
{
}


/**********************************************************************//**
 * Copy constructor
 *************************************************************************/
CECorrections::CECorrections(const CECorrections& other) :
    outfilename_(other.outfilename_),
    corrections_(other.corrections_),
    min_mjd_(other.min_mjd_),
    max_mjd_(other.max_mjd_)
{
}


/**********************************************************************//**
 * Return the DUT1 correction parameter (represents UT1 - UTC)
 * 
 * @param[in] mjd       Modified Julian date
 * @return DUT1 correction parameter (represents UT1 - UTC)
 *************************************************************************/
double CECorrections::dut1(const double& mjd) const
{
    return GetTableValue(mjd, 0);
}


/**********************************************************************//**
 * Return the x-polar motion correction parameter
 * 
 * @param[in] mjd       Modified Julian date
 * @return x-polar motion correction parameter
 *************************************************************************/
double CECorrections::xpolar(const double& mjd) const
{
    return GetTableValue(mjd, 1);
}


/**********************************************************************//**
 * Return the y-polar motion correction parameter
 * 
 * @param[in] mjd       Modified Julian date
 * @return y-polar motion correction parameter
 *************************************************************************/
double CECorrections::ypolar(const double& mjd) const
{
    return GetTableValue(mjd, 2);
}


/**********************************************************************//**
 * Downloads the IERS earth orientation correction parameters
 * 
 * @return Whether or not the download was successful
 *************************************************************************/
bool CECorrections::DownloadTables(void) const
{
    bool success = true;
    try {
        CURL *curl;
        FILE *fp;
        CURLcode res;
        std::string url("http://maia.usno.navy.mil/ser7/finals2000A.all");
        curl = curl_easy_init();                                                                                                                                                                                                                                                           
        if (curl)
        {   
            fp = fopen(outfilename_.c_str(), "wb");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);
            // TODO: handle the result
        }   
    } catch (std::exception& e) {
        std::cerr << "ERROR trying to download corrections files: " 
                  << e.what() << std::endl;
        success = false;
    }
    return success;
}


/**********************************************************************//**
 * Loads the IERS earth orientation correction parameters
 * @return Whether or not the load was successful
 * 
 * This method also identifies the minimum and maximum supported MJD loaded
 * from the corrections file.
 *************************************************************************/
bool CECorrections::LoadTables(void) const
{
    bool loaded = true;
    if (corrections_.size() == 0) {
        // Check if the file has been stored
        std::ifstream corrections_file(outfilename_, std::ios::in);

        // Try downloading the file
        if (!corrections_file.is_open()) {
            // Close the file so there's no funny business
            corrections_file.close();

            // Try downloading the correction files
            if (!DownloadTables())
                throw CEException::exception();

            // Try to open the file again
            corrections_file.open(outfilename_, std::ios::in);
            // Make sure the file is open
            if (!corrections_file.is_open()) {
                std::string msg = "Unable to load corrections file: " + outfilename_;
                throw CEException::corr_file_load_error(__func__, msg);
            }
        }
        
        // Try to load values from the file
        try {
            // Loop through each line of the file
            std::string line;
            std::vector<double> correct(3, 0.0);
            while(std::getline(corrections_file, line)) {
                int mjd(std::stoi(line.substr(7,8)));

                // Try to load dut1, xypolar from bulletin B positions
                try {
                    correct[0] = std::stod(line.substr(154,11));
                    correct[1] = std::stod(line.substr(134,10));
                    correct[2] = std::stod(line.substr(144,10));
                } 
                // Otherwise load from bulletin A positions
                catch (std::exception& e) {
                    try {
                        correct[0] = std::stod(line.substr(58,10));
                        correct[1] = std::stod(line.substr(18, 9));
                        correct[2] = std::stod(line.substr(37, 9));
                    }
                    catch (std::exception& e) {
                        // Reached end of useable fields in the file
                        break;
                    }
                }

                if (mjd < min_mjd_) min_mjd_ = mjd;
                if (mjd > max_mjd_) max_mjd_ = mjd;

                // The Standards of Fundamental Astronomy expects x,y polar
                // motion in radians, but the bulletins store the numeber in
                // arcseconds, so we need to do a conversion
                correct[1] *= DAS2R;
                correct[2] *= DAS2R;

                // Store the values
                corrections_[mjd] = correct;
            }

            // Close the corrections file
            corrections_file.close();

        } catch (std::exception& e) {
            std::cerr << "ERROR Unable to load corrections from file" << std::endl;
            std::cerr << e.what() << std::endl;
            loaded = false;
        }
    }
    return loaded;
}


/**********************************************************************//**
 * Loads a specific correction value from the internal tables
 * 
 * @param[in] mjd       Modified Julian date for lookup
 * @param[in] tbl_indx  Index of desired parameter
 * @return Whether or not the download was successful
 * 
 * The @p tbl_indx parameter references which parameter to return:
 *    - 0: dut1 (UT1 - UTC)
 *    - 1: x-polar motion
 *    - 2: y-polar motion
 *************************************************************************/
double CECorrections::GetTableValue(const double& mjd, 
                                    const int& tbl_indx) const
{
    LoadTables();
    int i_mjd(mjd);
    if (corrections_.count(i_mjd) == 0) {
        std::string msg = "Invalid mjd: " + std::to_string(mjd) + ". Accepted " +
                          "range is " + std::to_string(min_mjd_) + "-" + std::to_string(max_mjd_);
        throw CEException::invalid_value(__func__, msg);
    }
    return corrections_[i_mjd][tbl_indx];
}

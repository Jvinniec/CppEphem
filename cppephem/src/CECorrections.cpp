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
#include "CEDate.h"
#include "CEException.h"
#include <sofam.h>
#include <exception>
#include <iostream>
#include <cmath>
#include <fstream>

#ifndef NOCURL
#include <curl/curl.h>
#endif

// #ifndef CECORRFILEPATH
// #define CECORRFILEPATH std::string("")
// #endif


/**********************************************************************//**
 * Constructor for coordinate corrections object
 *************************************************************************/
CECorrections::CECorrections()
{
    init_members();
}


/**********************************************************************//**
 * Copy constructor
 *************************************************************************/
CECorrections::CECorrections(const CECorrections& other)
{
    copy_members(other);
}


/**********************************************************************//**
 * Return the DUT1 correction parameter (represents UT1 - UTC in seconds)
 * 
 * @param[in] mjd       Modified Julian date
 * @return DUT1 correction parameter (represents UT1 - UTC in seconds)
 *************************************************************************/
double CECorrections::dut1(const double& mjd) const
{
    // Update the cache
    UpdateNutationCache(mjd);
    
    // return cached value
    return cache_nut_dut1_;
}


/**********************************************************************//**
 * Return the x-polar motion correction parameter (radians)
 * 
 * @param[in] mjd       Modified Julian date
 * @return x-polar motion correction parameter (radians)
 *************************************************************************/
double CECorrections::xpolar(const double& mjd) const
{
    // Update the cache
    UpdateNutationCache(mjd);

    // return cached value
    return cache_nut_xp_;
}


/**********************************************************************//**
 * Return the y-polar motion correction parameter (radians)
 * 
 * @param[in] mjd       Modified Julian date
 * @return y-polar motion correction parameter (radians)
 *************************************************************************/
double CECorrections::ypolar(const double& mjd) const
{
    // Update the cache
    UpdateNutationCache(mjd);

    // return cached value
    return cache_nut_yp_;
}


/**********************************************************************//**
 * Return the offset in obliquity correction parameter (radians)
 * 
 * @param[in] mjd       Modified Julian date
 * @return Obliquity correction parameter (radians)
 *************************************************************************/
double CECorrections::deps(const double& mjd) const
{
    // Update the cache
    UpdateTtUt1Cache(mjd);

    // return cached value
    return cache_nut_deps_;
}


/**********************************************************************//**
 * Return the offset in longitude correction parameter (radians)
 * 
 * @param[in] mjd       Modified Julian date
 * @return longitude offset correction parameter (radians)
 *************************************************************************/
double CECorrections::dpsi(const double& mjd) const
{
    // Update the cache
    UpdateTtUt1Cache(mjd);

    // return cached value
    return cache_nut_dpsi_;
}


/**********************************************************************//**
 * Return the TT-UT1 correction at a given date (in seconds)
 * 
 * @param[in] mjd       Modified Julian date
 * @return TT-UT1 offset correction parameter at a given MJD (seconds)
 *************************************************************************/
double CECorrections::ttut1(const double& mjd) const
{
    // Update the cache
    UpdateTtUt1Cache(mjd);

    // return cached value
    return cache_ttut1_delt_;
}


/**********************************************************************//**
 * Overloaded assignment operator
 * 
 * @param[in] other     CECorrections object to be copied
 * @return Reference to this object post-assignment
 *************************************************************************/
CECorrections& CECorrections::operator=(const CECorrections& other)
{
    if (this != &other) {
        free_members();
        init_members();
        copy_members(other);
    }
    return *this;
}


/**********************************************************************//**
 * Sets the name of the nutation corrections file
 * 
 * @param[in] filename      Filename to read/write corrections from/to
 *************************************************************************/
void CECorrections::SetNutationFile(const std::string& filename)
{
    nutation_file_ = filename;
}


/**********************************************************************//**
 * Sets the name of the historic values TT-UT1 corrections file
 * 
 * @param[in] filename      Filename to read/write corrections from/to
 *************************************************************************/
void CECorrections::SetTtUt1HistFile(const std::string& filename)
{
    ttut1_file_hist_ = filename;
}


/**********************************************************************//**
 * Sets the name of the predicted values TT-UT1 corrections file
 * 
 * @param[in] filename      Filename to read/write corrections from/to
 *************************************************************************/
void CECorrections::SetTtUt1PredFile(const std::string& filename)
{
    ttut1_file_pred_ = filename;
}


/**********************************************************************//**
 * Defines that the correction values should be interpolated
 * 
 * @param[in] set_interp        New interpolation boolean
 *************************************************************************/
void CECorrections::SetInterp(bool set_interp)
{
    // Only update if the value is different
    if (set_interp != interp_) {
        interp_ = set_interp;
        // Update cached dates to force recompute
        cache_nut_mjd_   = -1.0e30;
        cache_ttut1_mjd_ = -1.0e30;
    }
}


/**********************************************************************//**
 * Returns a message of the 
 * 
 * @param[in] set_interp        New interpolation boolean
 *************************************************************************/
const std::string CECorrections::describe(void) const
{
    std::string msg = CEBase::describe() + "\n";
    msg += "   Nutation File    : " + this->NutationFile() + "\n";
    msg += "   Historical TT-UT1: " + this->TtUt1HistFile() + "\n";
    msg += "   Predicted TT-UT1 : " + this->TtUt1PredFile();
    return msg;
}


/**********************************************************************//**
 * Free data member objects
 *************************************************************************/
void CECorrections::free_members(void)
{
    // Clear the nutation vectors
    nutation_mjd_.clear();
    nutation_dut1_.clear();
    nutation_xp_.clear();
    nutation_yp_.clear();
    nutation_deps_.clear();
    nutation_dpsi_.clear();

    // Clear the TT-UT1 vectors
    ttut1_mjd_.clear();
    ttut1_delt_.clear();
}


/**********************************************************************//**
 * Copy data members from another object
 * 
 * @param[in] other     CECorrections object to be copied
 *************************************************************************/
void CECorrections::copy_members(const CECorrections& other)
{
    nutation_file_ = other.nutation_file_;
    nutation_mjd_  = other.nutation_mjd_;
    nutation_dut1_ = other.nutation_dut1_;
    nutation_xp_   = other.nutation_xp_;
    nutation_yp_   = other.nutation_yp_;
    nutation_deps_ = other.nutation_deps_;
    nutation_dpsi_ = other.nutation_dpsi_;

    ttut1_file_hist_ = other.ttut1_file_hist_;
    ttut1_file_pred_ = other.ttut1_file_pred_;
    ttut1_delt_      = other.ttut1_delt_;

    // Caching values
    cache_nut_mjd_    = other.cache_nut_mjd_;
    cache_nut_dut1_   = other.cache_nut_dut1_;
    cache_nut_xp_     = other.cache_nut_xp_;
    cache_nut_yp_     = other.cache_nut_yp_;
    cache_nut_deps_   = other.cache_nut_deps_;
    cache_nut_dpsi_   = other.cache_nut_dpsi_;
    cache_ttut1_mjd_  = other.cache_ttut1_mjd_;
    cache_ttut1_delt_ = other.cache_ttut1_delt_;
}


/**********************************************************************//**
 * Initialize data members
 *************************************************************************/
void CECorrections::init_members(void)
{
    // Note that CECORRFILEPATH is defined at compile time
    interp_ = false;

    nutation_file_ = std::string(CECORRFILEPATH) + "/nutation.txt";
    nutation_mjd_  = std::vector<int>(0);
    nutation_dut1_ = std::vector<double>(0);
    nutation_xp_   = std::vector<double>(0);
    nutation_yp_   = std::vector<double>(0);
    nutation_deps_ = std::vector<double>(0);
    nutation_dpsi_ = std::vector<double>(0);

    ttut1_file_hist_ = std::string(CECORRFILEPATH) + "/ttut1_historic.txt";
    ttut1_file_pred_ = std::string(CECORRFILEPATH) + "/ttut1_predicted.txt";
    ttut1_mjd_       = std::vector<double>(0);
    ttut1_delt_      = std::vector<double>(0); 

    // Nutation cached values
    cache_nut_mjd_  = -1e30;
    cache_nut_dut1_ = 0.0;
    cache_nut_xp_   = 0.0;
    cache_nut_yp_   = 0.0;
    cache_nut_deps_ = 0.0;
    cache_nut_dpsi_ = 0.0;

    // TT-UT1 cached values
    cache_ttut1_mjd_  = -1e30;
    cache_ttut1_delt_ = 63.8285; // Default to J2000 reference date value
}


/**********************************************************************//**
 * Initialize data members
 *************************************************************************/
std::ifstream CECorrections::LoadFile(const std::string& filename,
                                      const std::string& url) const
{
    // Check if the file has been stored
    std::ifstream corrections_file(filename, std::ios::in);

    // Try downloading the file
    if (!corrections_file.is_open()) {
        // Close the file so there's no funny business while attempting
        // to write to it
        corrections_file.close();

        // Try downloading the correction files
        if (!DownloadTable(filename, url)) {
            throw CEException::exception();
        }

        // Try to open the file again
        corrections_file.open(filename, std::ios::in);

        // Make sure the file is open
        if (!corrections_file.is_open()) {
            std::string msg = "Unable to load corrections file: " + filename;
            throw CEException::corr_file_load_error(__func__, msg);
        }
    }

    return corrections_file;
}


/**********************************************************************//**
 * Downloads the IERS earth orientation correction parameters
 * 
 * @return Whether or not the download was successful
 *************************************************************************/
bool CECorrections::DownloadTable(const std::string& filename,
                                  const std::string& url) const
{
    bool success = true;
    #ifndef NOCURL
    try {
        CURL *curl;
        FILE *fp;
        CURLcode res;
        curl = curl_easy_init();                                                                                                                                                                                                                                                           
        if (curl)
        {   
            fp = fopen(filename.c_str(), "wb");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);
            // TODO: handle the result
        }   
    } 
    // Catch exceptions that may happen
    catch (std::exception& e) {
        std::cerr << "ERROR trying to download corrections files: " 
                  << e.what() << std::endl;
        success = false;
    }
    #else
    std::cout << "CppEphem was not compiled with curl support. To download "
              << "the corrections file automatically, recompile with -DNOCURL=0."
              << std::endl;
    success = false;
    #endif

    return success;
}


/**********************************************************************//**
 * Loads the IERS earth orientation correction parameters
 * 
 * @return Whether or not the load was successful
 *************************************************************************/
bool CECorrections::LoadNutation(void) const
{
    bool loaded = true;
    if (nutation_mjd_.size() == 0) {
        // Check if the file has been stored
        std::string   url = "http://maia.usno.navy.mil/ser7/finals2000A.all";
        std::ifstream corrections_file = LoadFile(nutation_file_, url);

        // Try to load values from the file
        try {
            // Allocate an approximate amount of memory for the values
            nutation_mjd_.reserve(20000);
            nutation_dut1_.reserve(20000);
            nutation_xp_.reserve(20000);
            nutation_yp_.reserve(20000);
            nutation_deps_.reserve(20000);
            nutation_dpsi_.reserve(20000);

            // Preliminary storage values
            int    mjd;
            double dut1;
            double xp;
            double yp;
            double deps;
            double dpsi;

            // Loop through each line of the file
            std::string line;
            while(std::getline(corrections_file, line)) {
                mjd = std::stoi(line.substr(7,8));

                // Try to load dut1, xypolar from bulletin B positions
                try {
                    dut1 = std::stod(line.substr(154,11));
                    xp   = std::stod(line.substr(134,10));
                    yp   = std::stod(line.substr(144,10));
                    deps = std::stod(line.substr(175,10));
                    dpsi = std::stod(line.substr(165,10));
                } 
                // Otherwise load from bulletin A positions
                catch (std::exception& e) {
                    try {
                        dut1 = std::stod(line.substr(58,10));
                        xp   = std::stod(line.substr(18, 9));
                        yp   = std::stod(line.substr(37, 9));
                        deps = std::stod(line.substr(116,9));
                        dpsi = std::stod(line.substr(97,9));
                    }
                    catch (std::exception& e) {
                        // Reached end of useable fields in the file
                        break;
                    }
                }

                // The Standards of Fundamental Astronomy expects angles in 
                // units of radians, so xp, yp, deps, and dpsi need to be converted
                nutation_mjd_.push_back( mjd );
                nutation_dut1_.push_back( dut1 );
                nutation_xp_.push_back( xp * DAS2R );      // arcsec -> radians
                nutation_yp_.push_back( yp * DAS2R );      // arcsec -> radians
                nutation_deps_.push_back( deps * DMAS2R ); // marcsec -> radians
                nutation_dpsi_.push_back( dpsi * DMAS2R ); // marcsec -> radians
            }

            // Close the corrections file
            corrections_file.close();

            // Shrink up the vectors so we dont take more memory than necessary
            nutation_mjd_.shrink_to_fit();
            nutation_dut1_.shrink_to_fit();
            nutation_xp_.shrink_to_fit();
            nutation_yp_.shrink_to_fit();
            nutation_deps_.shrink_to_fit();
            nutation_dpsi_.shrink_to_fit();

        } catch (std::exception& e) {
            std::cerr << "ERROR Unable to load corrections from file" << std::endl;
            std::cerr << e.what() << std::endl;
            loaded = false;
        }
    }
    return loaded;
}


/**********************************************************************//**
 * Loads the TT-UT1 correction values
 * 
 * @return Whether or not the load was successful
 *************************************************************************/
bool CECorrections::LoadTtUt1(void) const
{
    bool loaded = true;
    if (ttut1_mjd_.size() == 0) {

        try{
            // Check if the file has been stored
            std::string   url = "http://maia.usno.navy.mil/ser7/deltat.data";
            std::ifstream corrections_file = LoadFile(ttut1_file_hist_, url);    
        
            // Allocate an approximate amount of memory for the values
            ttut1_mjd_.reserve(5000);

            // Preliminary storage values
            int    mjd;
            double delt;

            // Loop through each line of the file
            std::string line;
            while(std::getline(corrections_file, line)) {

                // Extract the Gregorian vector date
                std::vector<double> greg_vect(4, 0.0);
                greg_vect[0] = std::stod(line.substr(1,4));
                greg_vect[1] = std::stod(line.substr(6,2));
                greg_vect[2] = std::stod(line.substr(9,2));
                mjd = CEDate::GregorianVect2MJD(greg_vect);

                // Try to load delta T from the file
                try {
                    delt = std::stod(line.substr(13,7));
                } 
                // Otherwise load from bulletin A positions
                catch (std::exception& e) {
                    // Reached end of useable fields in the file
                    break;
                }

                // Store the values into a vector
                ttut1_mjd_.push_back( mjd );
                ttut1_delt_.push_back( delt );
            }

            // Close the corrections file
            corrections_file.close();

            // Load the predicted corrections for the future
            url = "http://maia.usno.navy.mil/ser7/deltat.preds";
            corrections_file = LoadFile(ttut1_file_pred_, url);

            // Loop through each line of the file
            std::getline(corrections_file, line);   // First line is a header
            while(std::getline(corrections_file, line)) {

                // Extract the MJD
                mjd = std::stoi(line.substr(3,9));

                // Only add this value if it is actually from a later date
                // than the last entry in the vector
                if (mjd > ttut1_mjd_.back()) {

                    // Try to load delta T from the file
                    try {
                        delt = std::stod(line.substr(24,5));
                    } 
                    // Otherwise load from bulletin A positions
                    catch (std::exception& e) {
                        // Reached end of useable fields in the file
                        break;
                    }

                    // Store the values into a vector
                    ttut1_mjd_.push_back( mjd );
                    ttut1_delt_.push_back( delt );
                }
            }

            // Shrink up the vectors so we dont take more memory than necessary
            ttut1_mjd_.shrink_to_fit();
            ttut1_delt_.shrink_to_fit();

        } catch (std::exception& e) {
            std::cerr << "ERROR Unable to load corrections from file" << std::endl;
            std::cerr << e.what() << std::endl;
            loaded = false;
        }
    }
    return loaded;
}


/**********************************************************************//**
 * Recompute cached values of nutation valeus if necessary
 * 
 * @param[in] mjd       Modified Julian date for lookup
 *************************************************************************/
void CECorrections::UpdateNutationCache(const double& mjd) const
{
    // Check if the nutation actually needs to be updated
    if (mjd != cache_nut_mjd_) {
        // Make sure nutation corrections are loaded
        LoadNutation();

        // Compute the closest index associated with the MJD
        int indx = (std::lower_bound(nutation_mjd_.begin(),nutation_mjd_.end(), mjd) 
                    - nutation_mjd_.begin() ) - 1;

        // Make sure the MJD date is covered by stored correction values
        if ((indx < 0) || (indx >= nutation_mjd_.size()-1)) {
            // TODO: Make this a warning message
            std::string min_mjd(std::to_string(nutation_mjd_.front()));
            std::string max_mjd(std::to_string(nutation_mjd_.back()));
            std::string msg = "Invalid mjd: " + std::to_string(mjd) + ". Accepted " +
                            "range is " + min_mjd + " - " + max_mjd;
            throw CEException::invalid_value(__func__, msg);
        } 

        // Get the uninterpolated value
        else if (!interp_) {
            cache_nut_dut1_ = nutation_dut1_[indx];
            cache_nut_xp_   = nutation_xp_[indx];
            cache_nut_yp_   = nutation_yp_[indx];
            cache_nut_deps_ = nutation_deps_[indx];
            cache_nut_dpsi_ = nutation_dpsi_[indx];
        } 
        
        // Otherwise interpolate between closest two values (a bit slower)
        else {
            double mjd_lower( nutation_mjd_[indx] );
            double mjd_upper( nutation_mjd_[indx+1] );
            
            cache_nut_dut1_ = InterpValue(mjd, mjd_lower, mjd_upper,
                                          nutation_dut1_[indx], 
                                          nutation_dut1_[indx+1]);
            cache_nut_xp_   = InterpValue(mjd, mjd_lower, mjd_upper,
                                          nutation_xp_[indx], 
                                          nutation_xp_[indx+1]);
            cache_nut_yp_   = InterpValue(mjd, mjd_lower, mjd_upper,
                                          nutation_yp_[indx], 
                                          nutation_yp_[indx+1]);
            cache_nut_deps_ = InterpValue(mjd, mjd_lower, mjd_upper,
                                          nutation_deps_[indx], 
                                          nutation_deps_[indx+1]);
            cache_nut_dpsi_ = InterpValue(mjd, mjd_lower, mjd_upper,
                                          nutation_dpsi_[indx], 
                                          nutation_dpsi_[indx+1]);
        }

        cache_nut_mjd_  = mjd;
    }

    return;
}


/**********************************************************************//**
 * Recompute cached values of nutation valeus if necessary
 * 
 * @param[in] mjd       Modified Julian date for lookup
 *************************************************************************/
void CECorrections::UpdateTtUt1Cache(const double& mjd) const
{
    // Check if the nutation actually needs to be updated
    if (mjd != cache_ttut1_mjd_) {
        // Make sure nutation corrections are loaded
        LoadTtUt1();

        // Compute the closest index associated with the MJD
        int indx = (std::lower_bound(ttut1_mjd_.begin(), ttut1_mjd_.end(), mjd)
                    - ttut1_mjd_.begin()) - 1;

        // Make sure the MJD date is covered by stored correction values
        if ((indx < 0) || (indx >= ttut1_mjd_.size()-1)) {
            // TODO: Make this a warning message
            std::string min_mjd(std::to_string(ttut1_mjd_.front()));
            std::string max_mjd(std::to_string(ttut1_mjd_.back()));
            std::string msg = "Invalid mjd: " + std::to_string(mjd) + ". Accepted " +
                            "range is " + min_mjd + " - " + max_mjd;
            throw CEException::invalid_value(__func__, msg);
        } 

        // Get the uninterpolated value
        else if (!interp_) {
            cache_ttut1_delt_ = ttut1_delt_[indx];
        }
        
        // Otherwise interpolate between closest two values (a bit slower)
        else {
            double mjd_lower( ttut1_mjd_[indx] );
            double mjd_upper( ttut1_mjd_[indx+1] );
            
            cache_ttut1_delt_ = InterpValue(mjd, mjd_lower, mjd_upper,
                                            ttut1_delt_[indx], 
                                            ttut1_delt_[indx+1]);
        }

        cache_ttut1_mjd_  = mjd;
    }

    return;
}


/**********************************************************************//**
 * Return the interpolated value at a given x value between two known values
 * 
 * @param[in] x         Desired position for interpolation
 * @param[in] x0        Lower position where value is known
 * @param[in] x1        Upper position where value is known
 * @param[in] y0        Known value at position @p x0
 * @param[in] y1        Known value at position @p x1
 * @return Obliquity correction parameter
 *************************************************************************/
double CECorrections::InterpValue(const double& x,
                                  const double& x0, const double& x1,
                                  const double& y0, const double& y1) const
{
    return (y0*(x1 - x) + y1*(x - x0)) / (x1-x0);
}
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

#ifndef CECorrections_h
#define CECorrections_h

#include <map>
#include <string>
#include <vector>

class CECorrections {
public:
    CECorrections();
    CECorrections(const CECorrections& other);
    virtual ~CECorrections() {}

    CECorrections& operator=(const CECorrections& other);

    double      dut1(const double& mjd) const;
    double      xpolar(const double& mjd) const;
    double      ypolar(const double& mjd) const;
    double      deps(const double& mjd) const;
    double      dpsi(const double& mjd) const;
    double      ttut1(const double& mjd) const;
    std::string NutationFile(void) const;
    std::string TtUt1HistFile(void) const;
    std::string TtUt1PredFile(void) const;
    void        SetNutationFile(const std::string& filename);
    void        SetTtUt1HistFile(const std::string& filename);
    void        SetTtUt1PredFile(const std::string& filename);
    void        SetInterp(bool set_interp);

private:

    void   copy_members(const CECorrections& other);
    void   free_members(void);
    void   init_members(void);
    std::ifstream LoadFile(const std::string& filename,
                          const std::string& url) const;
    bool   DownloadTable(const std::string& filename,
                         const std::string& url) const;
    bool   LoadNutation(void) const;
    bool   LoadTtUt1(void) const;
    double InterpValue(const double& x,
                       const double& x0, const double& x1,
                       const double& y0, const double& y1) const;
    void   UpdateNutationCache(const double& mjd) const;
    void   UpdateTtUt1Cache(const double& mjd) const;

    // Filenames for storing/loading correction values
    mutable std::string nutation_file_;   ///< File for nutation corrections
    mutable std::string ttut1_file_hist_; ///< File for historic TT-UT1 corrections
    mutable std::string ttut1_file_pred_; ///< File for predicted TT-UT1 corrections

    // Table to hold the corrections for a given MJD
    mutable std::vector<int>    nutation_mjd_;
    mutable std::vector<double> nutation_dut1_;
    mutable std::vector<double> nutation_xp_;
    mutable std::vector<double> nutation_yp_;
    mutable std::vector<double> nutation_deps_;
    mutable std::vector<double> nutation_dpsi_;

    mutable std::vector<double> ttut1_mjd_;
    mutable std::vector<double> ttut1_delt_;

    // Specifies whether to interpolate values between dates or not
    // Interpolating will give slightly more accurate results at the expense
    // of increasing computation time.
    bool interp_;

    // Caching variables so that we dont need to find new values if we've
    // already looked up the appropriate index
    mutable double cache_nut_mjd_;
    mutable double cache_nut_dut1_;
    mutable double cache_nut_xp_;
    mutable double cache_nut_yp_;
    mutable double cache_nut_deps_;
    mutable double cache_nut_dpsi_;
    mutable double cache_ttut1_mjd_;
    mutable double cache_ttut1_delt_;
};


/**********************************************************************//**
 * Returns the name of the nutation corrections file
 * 
 * @return Nutation corrections filename
 *************************************************************************/
inline
std::string CECorrections::NutationFile(void) const
{
    return nutation_file_;
}


/**********************************************************************//**
 * Returns the name of the historic TT-UT1 corrections file
 * 
 * @return Historic TT-UT1 corrections filename
 *************************************************************************/
inline
std::string CECorrections::TtUt1HistFile(void) const
{
    return ttut1_file_hist_;
}


/**********************************************************************//**
 * Returns the name of the precited TT-UT1 corrections file
 * 
 * @return Predicted TT-UT1 corrections filename
 *************************************************************************/
inline
std::string CECorrections::TtUt1PredFile(void) const
{
    return ttut1_file_pred_;
}

#endif /* CECorrections_h */

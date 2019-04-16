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
    std::string Filename(void) const;
    void        SetFilename(const std::string& filename);
    void        SetInterp(bool set_interp);

private:

    void   copy_members(const CECorrections& other);
    void   free_members(void);
    void   init_members(void);

    bool   DownloadTables(void) const;
    bool   LoadTables(void) const;
    double GetTableValue(const double& mjd, const int& tbl_indx) const;

    mutable std::string filename_;   ///< File where corrections are stored

    // Table to hold the corrections for a given MJD
    mutable std::map<int, std::vector<double>> corrections_;

    // Specifies whether to interpolate values between dates or not
    // Interpolating will give slightly more accurate results at the expense
    // of increasing computation time.
    bool         interp_;
    mutable int  min_mjd_;
    mutable int  max_mjd_;

};


/**********************************************************************//**
 * Returns the name of the corrections file
 * 
 * @return Corrections filename
 *************************************************************************/
inline
std::string CECorrections::Filename(void) const
{
    return filename_;
}


/**********************************************************************//**
 * Defines that the correction values should be interpolated
 * 
 * @param[in] set_interp        New interpolation boolean
 *************************************************************************/
inline
void CECorrections::SetInterp(bool set_interp)
{
    interp_ = set_interp;
}

#endif /* CECorrections_h */

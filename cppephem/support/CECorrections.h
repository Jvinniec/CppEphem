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

    double dut1(const double& mjd) const;
    double xpolar(const double& mjd) const;
    double ypolar(const double& mjd) const;

private:

    bool   DownloadTables(void) const;
    bool   LoadTables(void) const;
    double GetTableValue(const double& mjd, const int& tbl_indx) const;

    std::string outfilename_;   ///< File where corrections are stored

    // Table to hold the corrections for a given MJD
    mutable std::map<int, std::vector<double>> corrections_;
    //std::map<int, std::vector<double>> errors_;

    // Specifies whether to interpolate values between dates or not
    // Interpolating will give slightly more accurate results at the expense
    // of increasing computation time.
    static  bool interp_;
    mutable int  min_mjd_;
    mutable int  max_mjd_;

};

#endif /* CECorrections_h */

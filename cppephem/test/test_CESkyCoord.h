/***************************************************************************
 *  test_CESkyCoord.h: CppEphem                                            *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2018 JCardenzana                                           *
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

#ifndef test_CESkyCoord_h
#define test_CESkyCoord_h

#include "CESkyCoord.h"
#include "CEDate.h"
#include "CEObserver.h"
#include "CETestSuite.h"

class test_CESkyCoord : public CETestSuite {
public:
    test_CESkyCoord();
    virtual ~test_CESkyCoord();

    virtual bool runtests();

    /****** METHODS ******/
    virtual bool test_construct(void);
    virtual bool test_copy(void);

    virtual bool test_Convert2Icrs(void);
    virtual bool test_Convert2Cirs(void);
    virtual bool test_Convert2Galactic(void);
    virtual bool test_Convert2Observed(void);

    virtual bool test_AngularSeparation(void);
    virtual bool test_ConvertTo(void);
private:

    virtual bool test_coords(const CESkyCoord&  test,
                             const CESkyCoord&  expected,
                             const std::string& func,
                             const int&         line);

    // Private coordin
    CESkyCoord base_cirs_;
    CESkyCoord base_icrs_;
    CESkyCoord base_gal_;
    CESkyCoord base_obs_;
    CEDate     base_date_;
    CEObserver base_observer_;
};

#endif /* test_CESkyCoord_h */

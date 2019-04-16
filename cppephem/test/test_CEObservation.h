/***************************************************************************
 *  test_CEObservation.h: CppEphem                                         *
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

#ifndef test_CEObservation_h
#define test_CEObservation_h

#include "CEObservation.h"
#include "CETestSuite.h"

class test_CEObservation : public CETestSuite {
public:
    test_CEObservation();
    virtual ~test_CEObservation();

    virtual bool runtests(void);

    /****** METHODS ******/

    bool test_constructor(void);
    bool test_obj_return(void);
    bool test_cache(void);

private:

    /***** VARIABLES *****/

    CEBody        base_body_;
    CEDate        base_date_;
    CEObserver    base_observer_;
    CEObservation base_obs_;
};

#endif /* test_CEObservation_h */

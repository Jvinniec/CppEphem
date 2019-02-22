/***************************************************************************
 *  test_CEObserver.h: CppEphem                                            *
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

#ifndef test_CEObserver_h
#define test_CEObserver_h

#include "CEObserver.h"
#include "CETestSuite.h"

class test_CEObserver : public CETestSuite {
public:
    test_CEObserver();
    virtual ~test_CEObserver();

    virtual bool runtests(void);

    /****** METHODS ******/
    bool test_copy();
    bool test_set_geoCoords(void);
    bool test_set_atmoPars(void);

private:

    /***** VARIABLES *****/

    CEObserver base_obs_;
};

#endif /* test_CEObserver_h */
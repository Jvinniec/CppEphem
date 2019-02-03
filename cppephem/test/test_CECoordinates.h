/***************************************************************************
 *  test_CECoordinates.h: CppEphem                                         *
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

#ifndef test_CECoordinates_h
#define test_CECoordinates_h

#include "CECoordinates.h"
#include "CETestSuite.h"

class test_CECoordinates : public CETestSuite {
public:
    test_CECoordinates();
    virtual ~test_CECoordinates();

    virtual bool runtests();

    /****** METHODS ******/

    virtual bool test_SetCoord_Icrs(void);
    virtual bool test_SetCoord_Cirs(void);
    virtual bool test_SetCoord_Galactic(void);

private:

    // this is teh 
    CECoordinates base_;

};

#endif /* test_CECoordinates_h */
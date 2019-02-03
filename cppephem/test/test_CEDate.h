/***************************************************************************
 *  test_CEDate.h: CppEphem                                                *
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

#ifndef test_CEDate_h
#define test_CEDate_h

#include "CEDate.h"
#include "CETestSuite.h"

class test_CEDate : public CETestSuite {
public:
    test_CEDate();
    virtual ~test_CEDate();

    virtual bool runtests();

    /****** METHODS ******/
    
    virtual bool test_set_JD();

protected:

    CEDate base_date;
    CEDate test_date;

};

#endif /* test_CEDate_h */
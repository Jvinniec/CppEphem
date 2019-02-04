/***************************************************************************
 *  test_CENamespace.h: CppEphem                                           *
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

#ifndef test_CENamespace_h
#define test_CENamespace_h

#include "CENamespace.h"
#include "CETestSuite.h"

class test_CENamespace : public CETestSuite {
public:
    test_CENamespace();
    virtual ~test_CENamespace();

    virtual bool runtests();

    /****** METHODS ******/

    virtual bool test_SeaLevelVals(void);
    virtual bool test_Conversions(void);
    virtual bool test_Corrections(void);

};

#endif /* test_CENamespace_h */
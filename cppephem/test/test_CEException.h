/***************************************************************************
 *  test_CEException.h: CppEphem                                           *
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

#ifndef test_CEException_h
#define test_CEException_h

#include "CEException.h"
#include "CETestSuite.h"

class test_CEException : public CETestSuite {
public:
    test_CEException();
    virtual ~test_CEException();

    virtual bool runtests();

    /****** METHODS ******/

    virtual bool test_invalid_value(void);
    virtual bool test_sofa_error(void);
    virtual bool test_sofa_exception(void);

};

#endif /* test_CEException_h */
/***************************************************************************
 *  test_CEPlanet.h: CppEphem                                              *
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

#ifndef test_CEPlanet_h
#define test_CEPlanet_h

#include "CEPlanet.h"
#include "CETestSuite.h"

class test_CEPlanet : public CETestSuite {
public:
    test_CEPlanet();
    virtual ~test_CEPlanet();

    virtual bool runtests();

    /****** METHODS ******/

    virtual bool test_construct(void);

private:

    bool test_Planet(const CEPlanet&    planet1, 
                     const CEPlanet&    planet2,
                     const std::string& func,
                     const int&         line);

    CEPlanet base_;

};

#endif /* test_CEPlanet_h */
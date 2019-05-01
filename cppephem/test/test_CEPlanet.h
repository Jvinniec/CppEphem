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

#include "CEDate.h"
#include "CEObserver.h"
#include "CEPlanet.h"
#include "CETestSuite.h"

class test_CEPlanet : public CETestSuite {
public:
    test_CEPlanet();
    virtual ~test_CEPlanet();

    virtual bool runtests();

    /****** METHODS ******/

    virtual bool test_construct(void);

    // Test individual planets
    virtual bool test_mercury(void);
    virtual bool test_venus(void);
    virtual bool test_earth(void);
    virtual bool test_mars(void);
    virtual bool test_jupiter(void);
    virtual bool test_saturn(void);
    virtual bool test_uranus(void);
    virtual bool test_neptune(void);

    // Run the actual tests
    virtual bool test_planet(const CEPlanet&            test_planet,
                             const CECoordinates&       true_icrs,
                             const std::vector<double>& true_pos,
                             const std::vector<double>& true_vel);

private:

    bool test_Planets(void);

    CEObserver base_observer_;
    CEDate     base_date_;

};

#endif /* test_CEPlanet_h */

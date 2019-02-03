/***************************************************************************
 *  CETestSuite.h: CppEphem                                                     *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2017 JCardenzana                                           *
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

#ifndef CETestSuite_h
#define CETestSuite_h

#include <fstream>
#include <string>
#include <vector>

class CETestSuite {
public:
    
    // Constructor destructor
    CETestSuite();
    //CETestSuite(const std::string& log_filename);
    virtual ~CETestSuite();
    
    /******  METHODS  ******/
    
    virtual bool runtests(void) = 0;
    virtual bool pass(void);

protected:
    
    /******  METHODS  ******/
    
    virtual void cleanup();
    virtual bool test_double(const double& value, 
                             const double& expected,
                             const double& tol=1.0e-7);
    virtual bool test_int(const int& value, 
                          const int& expected,
                          const int& tol=0);
    virtual bool test_bool(bool value, 
                           bool expected);
    virtual bool test_string(const std::string& value, 
                             const std::string& expected);
    template<class T>
    bool test_vect_(const std::vector<T>& value,
                    const std::vector<T>& expected);
    bool test_vect(const std::vector<double>& value,
                   const std::vector<double>& expected);

    virtual void update_pass(const bool& test_passed);
    
private:

    /****** VARIABLES ******/
    std::ifstream log_;
    bool pass_ = true;
};


#endif /* CETestSuite_h */

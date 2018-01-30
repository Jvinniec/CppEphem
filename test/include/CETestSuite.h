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

class CETestSuite {
public:
    
    // Constructor destructor
    CETestSuite();
    //CETestSuite(const std::string& log_filename);
    virtual ~CETestSuite();
    
    /******  METHODS  ******/
    
    virtual bool runtests() = 0;

    /****** VARIABLES ******/
    
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
    virtual void update_pass(const bool& test_passed);

    /****** VARIABLES ******/
    std::ifstream log;
    bool pass = true;
    
private:
    
};


/**********************************************************************//**
 * Returns whether the analysis has succeeded or not
 *************************************************************************/
inline
void CETestSuite::cleanup(void)
{
    return;
}


/**********************************************************************//**
 * Returns whether the analysis has succeeded or not
 *************************************************************************/
inline
void CETestSuite::update_pass(const bool& test_passed)
{
    pass = pass && test_passed;
}


#endif /* CETestSuite_h */

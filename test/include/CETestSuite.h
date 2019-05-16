/***************************************************************************
 *  CETestSuite.h: CppEphem                                                *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2017-2019 JCardenzana                                      *
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

#define FUNC_DEFAULT "UNDEF"
#define LINE_DEFAULT -1

class CECoordinates;

class CETestSuite {
public:
    
    // Constructor destructor
    CETestSuite();
    //CETestSuite(const std::string& log_filename);
    virtual ~CETestSuite();
    
    /******  METHODS  ******/
    
    virtual bool runtests(void) = 0;
    virtual bool pass(void);

    virtual double DblTol(void) const;
    virtual int    IntTol(void) const;

    virtual void SetDblTol(const double& tol);
    virtual void SetIntTol(const int& tol);

protected:
    
    /******  METHODS  ******/
    
    virtual void cleanup();

    // Logging methods
    void log_success(const std::string& message,
                     const std::string& function,
                     const int&         line);
    void log_failure(const std::string& message,
                     const std::string& function,
                     const int&         line);

    // Testing methods for basic types
    virtual bool test(bool  test_success, 
                      const std::string& function = FUNC_DEFAULT,
                      const int&         line     = LINE_DEFAULT);
    virtual bool test_double(const double&      value, 
                             const double&      expected,
                             const std::string& function = FUNC_DEFAULT,
                             const int&         line     = LINE_DEFAULT);
    virtual bool test_int(const int&         value, 
                          const int&         expected,
                          const std::string& function = FUNC_DEFAULT,
                          const int&         line     = LINE_DEFAULT);
    virtual bool test_bool(bool  value, 
                           bool  expected,
                           const std::string& function = FUNC_DEFAULT,
                           const int&         line     = LINE_DEFAULT);

    virtual bool test_string(const std::string& value, 
                             const std::string& expected,
                             const std::string& function = FUNC_DEFAULT,
                             const int&         line     = LINE_DEFAULT);
    template<class T>
    bool test_vect_(const std::vector<T>& value,
                    const std::vector<T>& expected,
                    const T&           tol,
                    const std::string& function = FUNC_DEFAULT,
                    const int&         line     = LINE_DEFAULT);
    bool test_vect(const std::vector<double>& value,
                   const std::vector<double>& expected,
                   const std::string& function = FUNC_DEFAULT,
                   const int&         line     = LINE_DEFAULT);

    template<class T>
    bool test_lessthan_(const T& value,
                        const T& maxval,
                        const std::string& function = FUNC_DEFAULT,
                        const int&         line     = LINE_DEFAULT);
    bool test_lessthan(const double& value,
                       const double& maxval,
                       const std::string& function,
                       const int&         line);

    template<class T>
    bool test_greaterthan_(const T& value,
                           const T& minval,
                           const std::string& function = FUNC_DEFAULT,
                           const int&         line     = LINE_DEFAULT);
    bool test_greaterthan(const double& value,
                          const double& minval,
                          const std::string& function,
                          const int&         line);

    virtual void update_pass(const bool& test_passed);
    
private:

    /****** VARIABLES ******/

    std::ifstream log_;
    bool   pass_    = true;
    int    tol_int_ = 0;            //<! Expected integer precision
    double tol_dbl_ = 1.0e-15;      //<! Expected double precision
};


inline
double CETestSuite::DblTol(void) const
{
    return tol_dbl_;
}


inline
int CETestSuite::IntTol(void) const
{
    return tol_int_;
}


/**********************************************************************//**
 * Set the tolerance for comparing doubles
 * @param[in] tol               New tolerance for double tests
 *************************************************************************/
inline
void CETestSuite::SetDblTol(const double& tol)
{
    tol_dbl_ = tol;
}


/**********************************************************************//**
 * Set the tolerance for comparing integers
 * @param[in] tol               New tolerance for integer tests
 *************************************************************************/
inline
void CETestSuite::SetIntTol(const int& tol)
{
    this->tol_int_ = tol;
}

#endif /* CETestSuite_h */

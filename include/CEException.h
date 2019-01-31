/***************************************************************************
 *  CEException.cpp: CppEphem                                                   *
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

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <execinfo.h>

class CEExceptionHandler : public std::exception {
public:
    CEExceptionHandler() {
        // get void*'s for all entries on the stack
        void *array[100];
        int size = backtrace(array, 100);

        // print out all the frames to stderr
        char** bcktrace = backtrace_symbols(array, size);

        std::cerr << "Exception encountered. Printing backtrace..." << std::endl;
        for (size_t i = 0; i < size; i++) {
            std::cerr << std::string(bcktrace[i]) << std::endl;
        }
    }
    CEExceptionHandler(const std::string& origin,
                       const std::string& message,
                       const std::string& type = "<no type>");
    virtual ~CEExceptionHandler() throw() {}
    virtual const char* what() const noexcept;

protected:
    std::string origin_;
    std::string type_;
    std::string message_;
};


/***********************************************************************//**
 * @class CEException
 *
 * Interface for exceptions in CppEphem
 ***************************************************************************/
class CEException : public CEExceptionHandler {
public:

    // When a value is not valid
    class invalid_value : public CEExceptionHandler {
        public:
            invalid_value(const std::string& origin,
                          const std::string& message);
    };

    /* ----------------------------------------------------------- *
     *             EXCEPTIONS RELATED TO SOFA ERRORS
     * ----------------------------------------------------------- */

    // When an exception happens inside a sofa method
    class sofa_exception : public CEExceptionHandler {
        public:
            sofa_exception(const std::string& origin,
                           const std::string& sofa_method,
                           const std::string& message);
    };

    // When a particular error code is produced by a sofa method
    class sofa_error : public CEExceptionHandler {
        public: 
            sofa_error(const std::string& origin,
                       const std::string& sofa_method,
                       const int&         errcode,
                       const std::string& message);
    };
};

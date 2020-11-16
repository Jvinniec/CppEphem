/***************************************************************************
 *  CEException.i: CppEphem                                                *
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

/**
 * @file CEException.i
 * @brief SWIG file for CEException class
 * @author JCardenzana
 */
%{
/* Put headers and other declarations here that are needed for compilation */
// CppEphem HEADERS
#include "CEException.h"
%}
%include exception.i

%exception {
    try {
        $action
    }
    catch (const CEException::invalid_value& e) {
        SWIG_exception(SWIG_ValueError, e.what());
    }
    catch (const CEException::invalid_delimiter& e) {
        SWIG_exception(SWIG_ValueError, e.what());
    }
    catch (const CEException::corr_file_load_error& e) {
        SWIG_exception(SWIG_IOError, e.what());
    }
    catch (const CEException::sofa_error& e) {
        SWIG_exception(SWIG_ValueError, e.what());
    }
    catch (const CEException::sofa_exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
    catch (const CEException& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
    catch (const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
    catch (...) {
        SWIG_exception(SWIG_RuntimeError, "unknown exception");
    }
};


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

/** \class CEException
 CEException class handles Generic exceptions that may be thrown in the CppEphem
 CppEphem code.
 */

#include "CEException.h"


/**********************************************************************//**
 * Generates and returns the error message
 *  @return Formatted error message
 *************************************************************************/
CEExceptionHandler::CEExceptionHandler(const std::string& origin,
                                       const std::string& message,
                                       const std::string& type) :
    CEExceptionHandler()
{
    origin_  = origin;
    message_ += message;
    type_    = type;
}


/**********************************************************************//**
 * Generates and returns the error message
 *  @return Formatted error message
 *************************************************************************/
const char* CEExceptionHandler::what() const noexcept
{
    // Define the header of the error message
    std::string msg = "[ERROR] " + type_ + ": " + origin_ + "\n" + message_ + "\n";

    return msg.c_str();
}


/**********************************************************************//**
 * Append a string to the end of the message
 * @param[in] msg       Error message to be appended
 *************************************************************************/
void CEExceptionHandler::message_append(const std::string& msg)
{
    message_ += msg;
}


/**********************************************************************//**
 * Overwrite the error message
 * @param[in] msg       Error message
 *************************************************************************/
void CEExceptionHandler::message(const std::string& msg)
{
    message_ = msg;
}


/**********************************************************************//**
 * Generate an exception of type "invalid_value"
 *  @param[in] origin       Method that threw the error
 *  @param[in] message      Diagnostic message
 *************************************************************************/
CEException::invalid_value::invalid_value(const std::string& origin,
                                          const std::string& message) :
    CEExceptionHandler(origin, message, "Invalid Value")
{}


/**********************************************************************//**
 * Generate an exception of type "corr_file_load_error"
 *  @param[in] origin       Method that threw the error
 *  @param[in] message      Diagnostic message
 *************************************************************************/
CEException::corr_file_load_error::corr_file_load_error(const std::string& origin,
                                                        const std::string& message) :
    CEExceptionHandler(origin, message, "Corrections File Load Error")
{}


/**********************************************************************//**
 * Generate an exception of type "sofa_exception"
 *  @param[in] origin       Method that threw the error
 *  @param[in] message      Diagnostic message
 *************************************************************************/
CEException::sofa_exception::sofa_exception(const std::string& origin,
                                            const std::string& sofa_method,
                                            const std::string& message) :
    CEExceptionHandler(origin, message, "SOFA exception")
{
    // Overwrite the message
    this->message("SOFA method: " + sofa_method + "; " + message);
}


/**********************************************************************//**
 * Generate an exception of type "sofa_error"
 *  @param[in] origin       Method that threw the error
 *  @param[in] message      Diagnostic message
 * 
 * This error should be used when a known error code is produced by a SOFA
 * method.
 *************************************************************************/
CEException::sofa_error::sofa_error(const std::string& origin,
                                    const std::string& sofa_method,
                                    const int&         errcode,
                                    const std::string& message) :
    CEExceptionHandler(origin, message, "SOFA error")
{
    // Overwrite the message
    this->message("SOFA method: " + sofa_method + ", ErrCode: "+ std::to_string(errcode) +
                  "\n" + message);
}

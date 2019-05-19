/***************************************************************************
 *  CEAngle.cpp: CppEphem                                                  *
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

/** \class CEAngle
 The CEAngle class defines an angle. It can be constructed either from:
  * double: representing degrees or radians
  * string: representing HH:MM:SS or DD:MM:SS
  
 Methods are also provided to allow the user to retrieve the angle in whatever
 format they prefer
 */


#include "CEAngle.h"
#include "CEException.h"

/**********************************************************************//**
 * Default constructor
 *************************************************************************/
CEAngle::CEAngle()
{
    init_members();
}


/**********************************************************************//**
 * Construct from another CEAngle object
 * 
 * @param[in] other         CEAngle to copy data members from
 * 
 * By default this method assumes the passed angle is provided in RADIANS
 *************************************************************************/
CEAngle::CEAngle(const CEAngle& other)
{
    init_members();
    copy_members(other);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
CEAngle::~CEAngle(void)
{
    free_members();
}


/**********************************************************************//**
 * Copy assignment operator
 * 
 * @param[in] other         CEAngle object to copy objects from
 * @return CEAngle object constructed from @p other
 *************************************************************************/
CEAngle& CEAngle::operator=(const CEAngle& other)
{
    if (this != &other) {
        free_members();
        init_members();
        copy_members(other);
    }
    return *this;
}


/**********************************************************************//**
 * Return a double object representing the angle in radians
 *************************************************************************/
CEAngle::operator double()
{
    return angle_;
}


/**********************************************************************//**
 * Return a double object representing the angle in radians
 *************************************************************************/
CEAngle::operator double() const
{
    return angle_;
}


/**********************************************************************//**
 * Return CEAngle constructed from a string representing hours, minutes, seconds
 * 
 * @param[in] angle             Angle representing hours, minutes, seconds
 * @param[in] delim             Delimiter, by default all methods will be tested
 * 
 * The following values for @p delim are special cases:
 *      * 0 (int) = tests all following possible values for delimiter
 *      * ':' = 'HH:MM:SS'
 *      * ' ' = 'HH MM SS'
 *      * <any char> = The DD, MM, SS is separated by a user defined char
 *************************************************************************/
CEAngle CEAngle::Hms(const std::string& angle, const char& delim=0)
{
    CEAngle angle;
    angle.SetAngle(angle, CEAngleType::HMS, delim);
    return angle;
}


/**********************************************************************//**
 * Return CEAngle constructed from a string representing hours, minutes, seconds
 * 
 * @param[in] angle             Angle representing hours, minutes, seconds
 * @return CEAngle object
 *************************************************************************/
CEAngle CEAngle::Hms(const std::vector<double>& angle)
{
    CEAngle angle;
    angle.SetAngle(angle, CEAngleType::HMS);
    return angle;
}


std::string CEAngle::Hms(const char& delim)
{
    // Get the angle as a vector

}


/**********************************************************************//**
 * Return vector of doubles representing the {hours, min, sec, sec-fraction}
 * 
 * @return Vector of doubles representing {hours, min, sec, sec-fraction}
 * 
 * The returned vector has the following elements
 *      [0] = hours
 *      [1] = minutes
 *      [2] = seconds
 *      [3] = seconds fraction
 *************************************************************************/
std::vector<double> CEAngle::HmsVect(void) const
{
    // Stores the resulting values
    std::vector<int> HMS_i(4);

    // Stores the sign
    char pm;
    iauA2tf(9, angle_, &pm, &HMS_i[0]);
    
    // Convert the values into doubles
    std::vector<double> HMS_d(HMS_i.size());
    // Scale the fraction to be a true fraction
    HMS_d[0] = double(HMS_i[0]);
    HMS_d[1] = double(HMS_i[1]);
    HMS_d[2] = double(HMS_i[2]);
    HMS_d[3] = double(HMS_i[3])/1.0e9;
    // Multiply the degree by a negative
    if (pm == '-') HMS_d[0] *= -1;

    return HMS_d;
}


/**********************************************************************//**
 * Return CEAngle constructed from a string representing degrees, minutes, seconds
 * 
 * @param[in] angle             Angle representing degrees, arcmins, arcsecs
 * @param[in] delim             Delimiter, by default all methods will be tested
 * 
 * The following values for @p delim are special cases:
 *      * 0 (int) = tests all following possible values for delimiter
 *      * ':' = 'DD:MM:SS'
 *      * ' ' = 'DD MM SS'
 *      * <any char> = The DD, MM, SS is separated by a user defined char
 *************************************************************************/
CEAngle CEAngle::Dms(const std::string& angle, const char& delim=0)
{
    CEAngle angle;
    angle.SetAngle(angle, CEAngleType::DMS, delim);
    return angle;
}


/**********************************************************************//**
 * Return CEAngle constructed from a string representing degrees, minutes, seconds
 * 
 * @param[in] angle             Angle representing degrees, arcmins, arcsecs
 * @return CEAngle object
 *************************************************************************/
CEAngle CEAngle::Dms(const std::vector<double>& angle)
{
    CEAngle angle;
    angle.SetAngle(angle, CEAngleType::DMS);
    return angle;
}


/**********************************************************************//**
 * Return vector of doubles representing the {degrees, arcmin, arcsec, arcsec-fraction}
 * 
 * @return Vector of doubles representing {degrees, arcmin, arcsec, arcsec-fraction}
 * 
 * The returned vector has the following elements
 *      [0] = degrees
 *      [1] = arcminutes
 *      [2] = arcseconds
 *      [3] = arcseconds fraction
 *************************************************************************/
std::vector<double> CEAngle::DmsVect(void) const
{
    // Variables for holding the results
    char sign;
    std::vector<int> DMS_i(4, 0.0);

    // Run the appropriate SOFA value
    iauA2af(9, angle_, &sign, &DMS_i[0]);

    // Fill the values into a double vector
    std::vector<double> DMS_d(4, 0.0);
    DMS_d[0] = DMS_i[0] * (sign == '-' ? -1.0 : 1.0);
    DMS_d[1] = DMS_i[1];
    DMS_d[2] = DMS_i[2];
    DMS_d[3] = DMS_i[3] * 1.0e-9;

    return DMS_d ;
}


/**********************************************************************//**
 * Return CEAngle constructed from a degree angle
 * 
 * @param[in] angle             Angle in degrees
 * @return CEAngle object
 *************************************************************************/
CEAngle CEAngle::Deg(const double& angle)
{
    return CEAngle(angle*DD2R);
}


/**********************************************************************//**
 * Return angle in degrees as a double
 * 
 * @return Angle in degrees as a double
 *************************************************************************/
double CEAngle::Deg(void)
{
    return angle_ * DR2D;
}


/**********************************************************************//**
 * Return CEAngle constructed from a radians angle
 * 
 * @param[in] angle             Angle in radians
 * @return CEAngle object
 *************************************************************************/
CEAngle CEAngle::Rad(const double& angle)
{
    return CEAngle(angle);
}


/**********************************************************************//**
 * Return angle in radians as a double
 * 
 * @return Angle in radians as a double
 *************************************************************************/
double CEAngle::Rad(void)
{
    return angle_;
}


/**********************************************************************//**
 * Set the angle from a double
 * 
 * @param[in] angle             Angle value
 * @param[in] angle_type        CEAngleType (only DEGREES or RADIANS are accepted)
 * 
 * An exception is thrown if @p angle_type is neither DEGREES or RADIANS
 *************************************************************************/
void CEAngle::SetAngle(const double&      angle,
                       const CEAngleType& angle_type)
{
    // Set from degree angle
    if (angle_type == CEAngleType::DEGREES) {
        angle_ = angle * DD2R;
    }
    // Set from radians angle
    else if (angle_type == CEAngleType::RADIANS) {
        angle_ = angle;
    }
    // Otherwise throw an exception
    else {
        throw CEException::invalid_value(
            "CEAngle::SetAngle(const double&, const CEAngleType&)",
            "[ERROR] Method cannot be called with the provided CEAngleType");
    }

    return;
}


/**********************************************************************//**
 * Set the angle from a string representing either HMS or DMS angles
 * 
 * @param[in] angle             Angle represented by a HMS or DMS string
 * @param[in] angle_type        CEAngleType (only HMS or DMS are accepted)
 * @param[in] delim             Delimiter used to separate different components in @p angle
 * 
 * An exception is thrown if @p angle_type is neither HMS or DMS
 *************************************************************************/
void CEAngle::SetAngle(const std::string& angle,
                       const CEAngleType& angle_type,
                       const char&        delim)
{
    // Angle is a degree or radian
    if ((angle_type == CEAngleType::DEGREES) || (angle_type == CEAngleType::RADIANS)) {
        SetAngle(std::stod(angle), angle_type);
    }

    // Otherwise the angle is a delimited string (i.e. something like HH:MM:SS)
    else {
        // Create a string vector to hold the parsed values
        std::vector<std::string> angle_str;
        angle_str.reserve(4);

        // Handle specific delimitors
        if (int(delim) != 0) {
            // Split the string
            CppEphem::StrOpt::split(angle, delim, angle_str);
            if (angle_str.size() < 3) {
                throw CEException::invalid_value(
                    "CEAngle::SetAngle(const string&, const CEAngleType&, const char&)",
                    "[ERROR] Could not find specified delimiter: " + std::string(delim));
            }
        } 
        
        // Handle delimiter agnostic format
        else if (int(delim) == 0) {
            // Create vector of delimiters to be tried
            std::vector<char> delim_list = {':', ' '};
            bool success = false;
            for (auto& d : delim_list) {
                try {
                    // Try to set the angle
                    SetAngle(angle, angle_type, d);
                    success = true;
                    break;
                } catch (CEException::invalid_value& e) {
                    // Failed
                }
            }

            // Make sure setting the angle was a success
            if (!success) {
                throw CEException::invalid_value(
                    "CEAngle::SetAngle(const string&, const CEAngleType&, const char&)",
                    "[ERROR] Unable to parse angle string");
            }
        }

        // Now format the string in the appropriate way
        std::vector<double> angle_dbl(angle_str.size());
    }

    return;
}


/**********************************************************************//**
 * Set the angle from a vector of doubles
 * 
 * @param[in] angle             Angle as a vector of doubles
 * @param[in] angle_type        CEAngleType (only HMS or DMS are accepted)
 * 
 * An exception is thrown if @p angle_type is neither HMS or DMS
 *************************************************************************/
void CEAngle::SetAngle(const std::vector<double>& angle,
                       const CEAngleType&         angle_type)
{
    // Throw an error if the provided angle type does not make sense
    if ((angle_type != CEAngleType::HMS) || (angle_type != CEAngleType::DMS)) {
        throw CEException::invalid_value(
            "CEAngle::SetAngle(const std::vector&, const CEAngleType&)",
            "[ERROR] Method cannot be called with the provided CEAngleType");
    }

    // Construct the angle as the sum of the different components
    double full_ang( std::fabs(angle[0]) );
    full_ang += std::fabs(angle[1]) / 60.0;
    full_ang += std::fabs(angle[2]) / 3600.0;

    // In case the angle has been 
    if (angle.size() == 4) {
        full_ang += std::fabs(angle[3]) / 3600.0;
    }

    // Scale the angle from hours to degrees
    if (angle_type == CEAngleType::HMS) {
        full_ang *= 15.0;
    }

    // Set the angle
    SetAngle(full_ang, CEAngleType::DEGREES);
    return;
}


/**********************************************************************//**
 * Free allocated data members
 *************************************************************************/
void CEAngle::free_members(void)
{
    // Nothing to do here
    return;
}


/**********************************************************************//**
 * Copy data members from another CEAngle object
 * 
 * @param[in] other         CEAngle object to copy from
 *************************************************************************/
void CEAngle::copy_members(const CEAngle& other)
{
    angle_ = other.angle_;
}


/**********************************************************************//**
 * Initialize data members
 *************************************************************************/
void CEAngle::init_members(void)
{
    angle_ = 0.0;
}

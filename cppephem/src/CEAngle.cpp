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

#include <algorithm>

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
 * Construct from an angle in radians
 * 
 * @param[in] angle         Angle in radians
 * 
 * The provided @p angle is assumed to be in units of radians
 *************************************************************************/
CEAngle::CEAngle(const double& angle)
{
    init_members();
    SetAngle(angle, CEAngleType::RADIANS);
}


/**********************************************************************//**
 * Construct from another CEAngle object
 * 
 * @param[in] other         CEAngle to copy data members from
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
 * Copy assignment operator from a double
 * 
 * @param[in] other         double angle (radians)
 * @return CEAngle object constructed from @p other
 *************************************************************************/
CEAngle& CEAngle::operator=(const double& other)
{
    free_members();
    init_members();
    SetAngle(other, CEAngleType::RADIANS);
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
 * Return angle constructed from a string representing hours, minutes, seconds
 * 
 * @param[in] angle_str         Angle representing hours, minutes, seconds
 * @param[in] delim             Delimiter, by default all methods will be tested
 * @return Angle in radians
 * 
 * The following values for @p delim are special cases:
 *      * 0 (int) = tests all following possible values for delimiter
 *      * ':' = 'HH:MM:SS'
 *      * ' ' = 'HH MM SS'
 *      * <any char> = The DD, MM, SS is separated by a user defined char
 *************************************************************************/
CEAngle CEAngle::Hms(const char* angle_str, 
                     const char& delim)
{
    CEAngle angle;
    angle.SetAngle(angle_str, CEAngleType::HMS, delim);
    return angle;
}


/**********************************************************************//**
 * Return angle constructed from a string representing hours, minutes, seconds
 * 
 * @param[in] angle_vec         Angle representing hours, minutes, seconds
 * @return Angle in radians
 * 
 * The input @p angle is represented in the following manner
 *      [0] = hours
 *      [1] = minutes
 *      [2] = seconds
 *      [3] = (optional) seconds fraction
 * The index [3] is optional if index [2] contains the whole seconds portion
 *************************************************************************/
CEAngle CEAngle::Hms(const std::vector<double>& angle_vec)
{
    CEAngle angle;
    angle.SetAngle(angle_vec, CEAngleType::HMS);
    return angle;
}


/**********************************************************************//**
 * Return string representing the angle in HH:MM:SS
 * 
 * @param[in] delim             Delimiter to use in output string
 * @return Angle formatted as a string HH:MM:SS
 *************************************************************************/
std::string CEAngle::HmsStr(const char& delim) const
{
    // Get the angle as a vector
    std::vector<double> hms_d = HmsVect();

    // Assemble the string using the specified delimiter
    return CppEphem::StrOpt::join_angle(hms_d, delim);
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
 * Return double constructed from a string representing degrees, minutes, seconds
 * 
 * @param[in] angle_str         Angle representing degrees, arcmins, arcsecs
 * @param[in] delim             Delimiter, by default all methods will be tested
 * 
 * The following values for @p delim are special cases:
 *      * 0 (int) = tests all following possible values for delimiter
 *      * ':' = 'DD:MM:SS'
 *      * ' ' = 'DD MM SS'
 *      * <any char> = The DD, MM, SS is separated by a user defined char
 *************************************************************************/
CEAngle CEAngle::Dms(const char* angle_str, 
                     const char& delim)
{
    CEAngle angle;
    angle.SetAngle(angle_str, CEAngleType::DMS, delim);
    return angle;
}


/**********************************************************************//**
 * Return angle constructed from a string representing degrees, minutes, seconds
 * 
 * @param[in] angle_vec         Angle representing degrees, minutes, seconds
 * @return Angle in radians
 * 
 * The input @p angle is represented in the following manner
 *      [0] = degrees
 *      [1] = minutes
 *      [2] = seconds
 *      [3] = (optional) seconds fraction
 * The index [3] is optional if index [2] contains the whole seconds portion
 *************************************************************************/
CEAngle CEAngle::Dms(const std::vector<double>& angle_vec)
{
    CEAngle angle;
    angle.SetAngle(angle_vec, CEAngleType::DMS);
    return angle;
}


/**********************************************************************//**
 * Return string representing the angle in DD:MM:SS
 * 
 * @param[in] delim             Delimiter to use in output string
 * @return Angle formatted as a string DD:MM:SS
 *************************************************************************/
std::string CEAngle::DmsStr(const char& delim) const
{
    // Get the angle as a vector
    std::vector<double> dms_d = DmsVect();

    // Assemble the string using the specified delimiter
    return CppEphem::StrOpt::join_angle(dms_d, delim);
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
 * Return angle (radians) constructed from a degree angle
 * 
 * @param[in] angle             Angle in degrees
 * @return CEAngle object
 *************************************************************************/
CEAngle CEAngle::Deg(const double& angle)
{
    return CEAngle(angle * DD2R);
}


/**********************************************************************//**
 * Return angle in degrees as a double
 * 
 * @return Angle in degrees as a double
 *************************************************************************/
double CEAngle::Deg(void) const
{
    return angle_ * DR2D;
}


/**********************************************************************//**
 * Return angle constructed from a radians angle
 * 
 * @param[in] angle             Angle in radians
 * @return Angle constructed from a 'radians' object
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
double CEAngle::Rad(void) const
{
    return angle_;
}


/**********************************************************************//**
 * Set the angle from a double
 * 
 * @param[in] angle             Angle value
 * @param[in] angle_type        CEAngleType (only DEGREES or RADIANS are accepted)
 * 
 * @exception CEException::invalid_value
 *            @p angle_type is neither DEGREES nor RADIANS
 * 
 * This method calls the SOFA function iauAnp to ensure the angle is in the
 * range [0, 2pi).
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
void CEAngle::SetAngle(const char*        angle_str,
                       const CEAngleType& angle_type,
                       const char&        delim)
{
    // Angle is a degree or radian
    if ((angle_type == CEAngleType::DEGREES) || (angle_type == CEAngleType::RADIANS)) {
        SetAngle(std::stod(angle_str), angle_type);
    }

    // Otherwise the angle is a delimited string (i.e. something like HH:MM:SS)
    else {

        // Create a string vector to hold the parsed values
        std::vector<std::string> angle_vec;
        angle_vec.reserve(4);

        // Handle specific delimitors
        if (delim != 0) {

            // Split the string
            CppEphem::StrOpt::split(angle_str, delim, &angle_vec);

            // Throw an error if the string was not properly split
            if (angle_vec.size() < 3) {
                throw CEException::invalid_delimiter(
                    "CEAngle::SetAngle(const string&, const CEAngleType&, const char&)",
                    "Could not find specified delimiter: " + std::string(1,delim));
            }

            // Now format the string in the appropriate way
            std::vector<double> angle_dbl;
            for (auto& ang : angle_vec) {
                angle_dbl.push_back(std::stod(ang));
            }
            SetAngle(angle_dbl, angle_type);
        } 

        // Handle delimiter agnostic format
        else if (delim == 0) {

            // Create vector of delimiters to be tried
            std::vector<char> delim_list = {':', ' '};

            // Loop through the list of delimiters
            for (char d : delim_list) {
                // Check if the delimiter is in the string
                if (std::string(angle_str).find(d) != std::string::npos) {
                    // Try to set the angle
                    SetAngle(angle_str, angle_type, d);
                }
            }
        }
    }

    return;
}


/**********************************************************************//**
 * Set the angle from a vector of doubles representing HMS or DMS format
 * 
 * @param[in] angle_vec         Angle as a vector of doubles
 * @param[in] angle_type        Angle type of @p angle_vec
 * 
 * An exception is thrown if @p angle_type is neither HMS or DMS
 *************************************************************************/
void CEAngle::SetAngle(const std::vector<double>& angle_vec,
                       const CEAngleType&         angle_type)
{
    // Hours minutes seconds
    if (angle_type == CEAngleType::HMS) {
        SetAngle_HmsVect(angle_vec);
    } 
    // Degrees arcmins arcseconds
    else if (angle_type == CEAngleType::DMS) {
        SetAngle_DmsVect(angle_vec);
    }
    // Otherwise, all done
    else {
        throw CEException::invalid_value(
            "CEAngle::SetAngle(const std::vector<double>&, const CEAngleType&)",
            "[ERROR] Unknown angle type");
    }
}


/**********************************************************************//**
 * Set the angle from a vector of doubles representing {hours, minutes, seconds}
 * 
 * @param[in] angle             Angle as a vector of doubles
 * 
 * An exception is thrown if @p angle_type is neither HMS or DMS
 *************************************************************************/
void CEAngle::SetAngle_HmsVect(const std::vector<double>& angle)
{
    // Get the angle sign
    char pm = (angle[0] >= 0.0) ? '+' : '-';

    // Do the conversion
    double ang(0.0);
    double sec = (angle.size() == 4) ? angle[2]+angle[3] : angle[2];
    int err = iauTf2a(pm, std::abs(angle[0]), int(angle[1]), sec, &ang);

    // Handle the error
    if (err) {
        std::string msg = "[ERROR] ";
        switch(err) {
            // Hours out of bounds
            case 1:
                msg += "Hour value \'" + std::to_string(angle[0]) + "\' not in range 0-23";
                throw CEException::invalid_value("CEAngle::SetAngle", msg);
            // Minutes out of bounds
            case 2:
                msg += "Minutes value \'" + std::to_string(angle[1]) + "\' not in range 0-59";
                throw CEException::invalid_value("CEAngle::SetAngle", msg);
            // Seconds out of bounds
            case 3:
                msg += "Seconds value \'" + std::to_string(sec) + "\' not in range 0-59.999...";
                throw CEException::invalid_value("CEAngle::SetAngle", msg);
        }
    }

    // Set the angle
    SetAngle(ang, CEAngleType::RADIANS);
    return;
}


/**********************************************************************//**
 * Set the angle from a vector of doubles
 * 
 * @param[in] angle             Angle as a vector of doubles
 * 
 * An exception is thrown if @p angle_type is neither HMS or DMS
 *************************************************************************/
void CEAngle::SetAngle_DmsVect(const std::vector<double>& angle)
{
    // Get the angle's sign
    char sign = (angle[0] < 0.0) ? '-' : '+';

    // Convert the values to an angle in degrees
    double ang(0.0);
    double sec = (angle.size() == 4) ? angle[2]+angle[3] : angle[2];

    // Pass the values to the appropriate SOFA algorithm
    int err = iauAf2a(sign, int(angle[0]), int(angle[1]), sec, &ang);

    // Handle the error
    if (err) {
        std::string msg = "[ERROR] ";
        switch(err) {
            // Hours out of bounds
            case 1:
                msg += "Degree value \'" + std::to_string(angle[0]) + "\' not in range 0-359";
                throw CEException::invalid_value("CECoordinates::DMSToAngle", msg);
            // Minutes out of bounds
            case 2:
                msg += "Arcmin value \'" + std::to_string(angle[1]) + "\' not in range 0-59";
                throw CEException::invalid_value("CECoordinates::DMSToAngle", msg);
            // Seconds out of bounds
            case 3:
                msg += "Arcsec value \'" + std::to_string(sec) + "\' not in range 0-59.999...";
                throw CEException::invalid_value("CECoordinates::DMSToAngle", msg);
        }
    }

    // Set the angle
    SetAngle(ang, CEAngleType::RADIANS);
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

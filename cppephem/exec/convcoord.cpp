/***************************************************************************
 *  getcirs.cpp: CppEphem                                                  *
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

#include <stdio.h>
#include <getopt.h>
#include <map>
#include <string>
#include <time.h>

// CppEphem HEADERS
#include "CppEphem.h"
#include "CEExecOptions.h"


/**********************************************************************//**
 * Define the command line options for this program
 *************************************************************************/
CEExecOptions DefineOpts()
{
    CEExecOptions opts("convcoord");

    // Add a program version and description
    opts.AddProgramDescription(std::string() +
        "Converts a user supplied set of coordinates to another coordinate system. " +
        "Additional parameters can be supplied to provide more accurate results.");

    // Set the options
    opts.AddCirsPar();
    opts.AddIcrsPar();
    opts.AddGalacticPar();
    opts.AddEclipticPar();
    opts.AddObservedPar();
    opts.AddObserverPars();

    opts.AddStringParam("to", "Coordinate system to convert into (\"CIRS\", \"ICRS\", \"Galactic\", \"Observed\", \"Ecliptic\")", "");

    return opts;
}

/**********************************************************************//**
 * Generate an observer object
 *************************************************************************/
inline
CEObserver GenObserver(CEExecOptions& opts)
{
    CEObserver obs(opts.AsDouble("longitude"),
                   opts.AsDouble("latitude"),
                   opts.AsDouble("elevation"),
                   CEAngleType::DEGREES);
    obs.SetRelativeHumidity(opts.AsDouble("humidity"));
    obs.SetPressure_hPa(opts.AsDouble("pressure"));
    obs.SetWavelength_um(opts.AsDouble("wavelength"));

    return obs;
}

/**********************************************************************//**
 * Get the user supplied coordinates
 *************************************************************************/
inline
CESkyCoord GetInCoord(CEExecOptions& opts)
{
    // Preliminary values
    std::string    parname;
    CESkyCoordType coord_type;
    CEAngleType    angle_type_x = CEAngleType::DEGREES;
    CEAngleType    angle_type_y = CEAngleType::DEGREES;

    // Check for delimiter
    char delim(0);
    if (opts.AsString("delim").size() == 1) {
        delim        = opts.AsString("delim")[0];
        angle_type_x = CEAngleType::DMS;        // True for most coord-types
        angle_type_y = CEAngleType::DMS;        // Always the case for y-coordinate
    }

    // Extract the coordinate type
    if (opts.AsString("cirs").size() > 0) {
        parname    = "cirs";
        coord_type = CESkyCoordType::CIRS;
        if (int(delim)) {
            angle_type_x = CEAngleType::HMS;
        }
    } else if (opts.AsString("icrs").size() > 0) {
        parname    = "icrs";
        coord_type = CESkyCoordType::ICRS;
        if (int(delim)) {
            angle_type_x = CEAngleType::HMS;
        }
    } else if (opts.AsString("galactic").size() > 0) {
        parname    = "galactic";
        coord_type = CESkyCoordType::GALACTIC;
    } else if (opts.AsString("observed").size() > 0) {
        parname    = "observed";
        coord_type = CESkyCoordType::OBSERVED;
    } else if (opts.AsString("ecliptic").size() > 0) {
        parname    = "ecliptic";
        coord_type = CESkyCoordType::ECLIPTIC;
    }

    // Load the angles
    std::vector<std::string> angles = CLOptionsHelper::split(opts.AsString(parname), ',');
    CEAngle xcoord;
    CEAngle ycoord;
    ycoord.SetAngle(angles[1].c_str(), angle_type_y, delim);
    xcoord.SetAngle(angles[0].c_str(), angle_type_x, delim);

    return CESkyCoord(xcoord, ycoord, coord_type);
}

/**********************************************************************//**
 *************************************************************************/
CESkyCoordType GetOutType(CEExecOptions& opts)
{
    // Get the coordinate system type (lower case)
    std::string user_type = opts.AsString("to");
    if (user_type.size() == 0) {
        throw std::invalid_argument("Must supply coordinate system to conver to (specify the \"to\" parameter)");
    }
    std::for_each(user_type.begin(), user_type.end(), [](char & c) {
		c = ::tolower(c);
	});

    // Return the system based on a user supplied value
    CESkyCoordType ret_type = CESkyCoordType::ICRS;
    if (user_type == "cirs") {
        ret_type = CESkyCoordType::CIRS;
    } else if (user_type == "icrs") {
        ret_type = CESkyCoordType::ICRS;
    } else if (user_type == "galactic") {
        ret_type = CESkyCoordType::GALACTIC;
    } else if (user_type == "observed") {
        ret_type = CESkyCoordType::OBSERVED;
    } else if (user_type == "ecliptic") {
        ret_type = CESkyCoordType::ECLIPTIC;
    }

    return ret_type;
}

/**********************************************************************//**
 *************************************************************************/
void PrintCoordConvResults(const CESkyCoord& outcoord,
                                          const CESkyCoord& incoord,
                                          const CEDate&     date,
                                          const CEObserver& obs)
{
    // Create a list of coordinate names
    std::map<CESkyCoordType, std::vector<std::string>> types;
    types[CESkyCoordType::CIRS]     = {"CIRS",     "Right Ascension", "Declination"};
    types[CESkyCoordType::ICRS]     = {"ICRS",     "Right Ascension", "Declination"};
    types[CESkyCoordType::GALACTIC] = {"Galactic", "Longitude",       "Latitude"};
    types[CESkyCoordType::OBSERVED] = {"Observed", "Azimuth",         "Zenith"};
    types[CESkyCoordType::ECLIPTIC] = {"Ecliptic", "Longitude",       "Latitude"};
                                                                                      
    std::vector<std::string> outnames = types[outcoord.GetCoordSystem()];
    std::vector<std::string> innames  = types[incoord.GetCoordSystem()];
    
    // Print output coordinates
    std::printf("\n") ;
    std::printf("******************************************\n");
    std::printf(" Results of %s -> %s conversion\n", 
                innames[0].c_str(), outnames[0].c_str());
    std::printf("******************************************\n");
    std::printf("%s Coordinates (output)\n", outnames[0].c_str());
    std::printf("    %-15s: %f degrees\n", outnames[1].c_str(), outcoord.XCoord().Deg());
    std::printf("    %-15s: %+f degrees\n", outnames[2].c_str(), outcoord.YCoord().Deg());
    std::printf("%s Coordinates (input)\n", innames[0].c_str());
    std::printf("    %-15s: %f degrees\n", innames[1].c_str(), incoord.XCoord().Deg());
    std::printf("    %-15s: %+f degrees\n", innames[2].c_str(), incoord.YCoord().Deg());
    std::printf("Julian Date        : %f\n", date.JD());
    if ((outcoord.GetCoordSystem() == CESkyCoordType::OBSERVED) || 
        (incoord.GetCoordSystem()  == CESkyCoordType::OBSERVED)) {
        std::printf("Observer Info\n");
        std::printf("    Longitude      : %f deg\n", obs.Longitude_Deg());
        std::printf("    Latitude       : %+f deg\n", obs.Latitude_Deg());
        std::printf("    Elevation      : %f meters\n", obs.Elevation_m());
        std::printf("    Pressure       : %f hPa\n", obs.Pressure_hPa());
        std::printf("    Temperature    : %f Celsius\n", obs.Temperature_C());
        std::printf("    Relative Humid.: %f\n", obs.RelativeHumidity());
    }
    std::printf("\n");
}

/**********************************************************************//**
 *************************************************************************/
int main(int argc, char** argv) {
    
    // Get the options from the command line
    CEExecOptions opts = DefineOpts();
    if (opts.ParseCommandLine(argc, argv)) return 0;
    
    // Create a date
    CEDate date(opts.AsDouble("juliandate"), CEDateType::JD);

    // Create the observer
    CEObserver obs = GenObserver(opts);
    
    // Get the input coordinates
    CESkyCoord incoord = GetInCoord(opts);

    // Convert the coordinates
    CESkyCoordType outtype    = GetOutType(opts);
    CESkyCoord     cirs_coord = incoord.ConvertTo(outtype, date, obs);
    
    // Print the results
    PrintCoordConvResults(cirs_coord, incoord, date, obs);
    
    return 0;
}

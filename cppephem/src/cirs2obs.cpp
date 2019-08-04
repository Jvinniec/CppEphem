/***************************************************************************
 *  cirs2obs.cpp: CppEphem                                                 *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016-2019 JCardenzana                                      *
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
    CEExecOptions opts("cirs2obs");

    // Add a program version and description
    opts.AddProgramDescription(std::string() +
        "Converts from CIRS coordinates to observed altitude and zenith " +
        "coordinates for a given Julian date. Additional observing conditions " +
        "an also be specified to allow more accurate coordinate values.");

    // Set the options
    opts.AddCirsPars();
    opts.AddObserverPars();
    opts.AddJDPar();

    return opts;
}


/**********************************************************************//**
 *************************************************************************/
void PrintResults(CEExecOptions&    inputs, 
                  const CESkyCoord& obs_coords,
                  const CESkyCoord& obs_cirs_coords,
                  const CEAngle&    hour_angle)
{
    std::printf("\n") ;
    std::printf("******************************************\n");
    std::printf("* Results of CIRS -> Observed conversion *\n");
    std::printf("******************************************\n");
    std::printf("Observed Coordinates (output)\n");
    std::printf("    Azimuth        : %f degrees\n", obs_coords.XCoord().Deg());
    std::printf("    Zenith         : %+f degrees\n", obs_coords.YCoord().Deg());
    std::printf("    Altitude       : %+f degrees\n", 90.0-obs_coords.YCoord().Deg());
    std::printf("CIRS Coordinates (input)\n");
    std::printf("    Right Ascension: %f degrees\n", inputs.AsDouble("ra"));
    std::printf("    Declination    : %+f degrees\n", inputs.AsDouble("dec"));
    std::printf("    Julian Date    : %f\n", inputs.AsDouble("juliandate"));
    std::printf("Apparent CIRS Coordinates\n");
    std::printf("    Right Ascension: %f\n", obs_cirs_coords.XCoord().Deg());
    std::printf("    Declination    : %+f\n", obs_cirs_coords.YCoord().Deg());
    std::printf("    Hour Angle     : %+f\n", hour_angle.Deg());
    std::printf("Observer Info\n");
    std::printf("    Longitude      : %f deg\n", inputs.AsDouble("longitude"));
    std::printf("    Latitude       : %+f deg\n", inputs.AsDouble("latitude"));
    std::printf("    Elevation      : %f meters\n", inputs.AsDouble("elevation"));
    std::printf("    Pressure       : %f hPa\n", inputs.AsDouble("pressure"));
    std::printf("    Temperature    : %f Celsius\n", inputs.AsDouble("temperature"));
    std::printf("    Relative Humid.: %f\n", inputs.AsDouble("humidity"));
    std::printf("\n");
}

/**********************************************************************//**
 *************************************************************************/
int main(int argc, char** argv) {
    
    // Get the options from the command line
    CEExecOptions opts = DefineOpts();
    if (opts.ParseCommandLine(argc, argv)) return 0;
    
    // Create a map to store the results
    std::map<std::string, double> results;
    results["azimuth"]      = 0.0;
    results["zenith"]       = 0.0;
    results["observed_ra"]  = 0.0;
    results["observed_dec"] = 0.0;
    results["hour_angle"]   = 0.0;
    
    // Create a date
    CEDate date(opts.AsDouble("juliandate"), CEDateType::JD);

    // Create the observer
    CEObserver obs(opts.AsDouble("longitude"),
                   opts.AsDouble("latitude"),
                   opts.AsDouble("elevation"),
                   CEAngleType::DEGREES);
    obs.SetRelativeHumidity(opts.AsDouble("humidity"));
    obs.SetPressure_hPa(opts.AsDouble("pressure"));
    obs.SetWavelength_um(opts.AsDouble("wavelength"));
    
    // Convert the coordinates
    CESkyCoord cirs_coord(CEAngle::Deg(opts.AsDouble("ra")),
                          CEAngle::Deg(opts.AsDouble("dec")),
                          CESkyCoordType::CIRS);
    CESkyCoord obs_coord;
    CESkyCoord obs_cirs_coord;
    CEAngle    hour_angle;
    CESkyCoord::CIRS2Observed(cirs_coord, &obs_coord, date, obs, 
                              &obs_cirs_coord, &hour_angle);

    // Print the results
    PrintResults(opts, obs_coord, obs_cirs_coord, hour_angle);
    
    return 0;
}

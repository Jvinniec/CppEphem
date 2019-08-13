/***************************************************************************
 *  gal2obs.cpp: CppEphem                                                  *
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
    CEExecOptions opts("gal2obs");

    // Add a program version and description
    opts.AddProgramDescription(std::string() +
        "Converts from Galactic coordinates to observed altitude and zenith " +
        "coordinates for a given Julian date. Additional observing conditions " +
        "an also be specified to allow more accurate coordinate values.");

    // Set the options
    opts.AddGalacticPars();
    opts.AddObserverPars();
    opts.AddJDPar();

    return opts;
}


/**********************************************************************//**
 *************************************************************************/
void PrintResults(CEExecOptions& inputs,
                  const CESkyCoord& obs_coords,
                  const CESkyCoord& obs_gal_coords)
{
    std::printf("\n");
    std::printf("**********************************************\n");
    std::printf("* Results of GALACTIC -> Observed conversion *\n");
    std::printf("**********************************************\n");
    std::printf("Observed Coordinates (output)\n");
    std::printf("    Azimuth        : %f degrees\n", obs_coords.XCoord().Deg());
    std::printf("    Zenith         : %+f degrees\n", obs_coords.YCoord().Deg());
    std::printf("    Altitude       : %+f degrees\n", 90.0-obs_coords.YCoord().Deg());
    std::printf("Galactic Coordinates (input)\n");
    std::printf("    Gal. Longitude : %f degrees\n", inputs.AsDouble("glon"));
    std::printf("    Gal. Latitude  : %+f degrees\n", inputs.AsDouble("glat"));
    std::printf("    Julian Date    : %f\n", inputs.AsDouble("juliandate"));
    std::printf("Apparent Galactic Coordinates\n");
    std::printf("    Gal. Longitude : %f\n", obs_gal_coords.XCoord().Deg());
    std::printf("    Gal. Latitude  : %+f\n", obs_gal_coords.YCoord().Deg());
    std::printf("Observer Info\n");
    std::printf("    Longitude      : %f deg\n", inputs.AsDouble("longitude"));
    std::printf("    Latitude       : %+f deg\n", inputs.AsDouble("latitude"));
    std::printf("    Elevation      : %f meters\n", inputs.AsDouble("elevation"));
    std::printf("    Pressure       : %f hPa\n", inputs.AsDouble("pressure"));
    std::printf("    Temperature    : %f Celsius\n", inputs.AsDouble("temperature"));
    std::printf("    Relative Humid.: %f\n", inputs.AsDouble("humidity"));
    std::printf("\n") ;
}

/**********************************************************************//**
 *************************************************************************/
int main(int argc, char** argv) 
{
    // Get the options from the command line
    CEExecOptions opts = DefineOpts() ;
    if (opts.ParseCommandLine(argc, argv)) return 0 ;
    
    // Create a date
    CEDate date(opts.AsDouble("juliandate"), CEDateType::JD);

    // Create the observer
    CEObserver obs(opts.AsDouble("longitude")*DD2R,
                   opts.AsDouble("latitude")*DD2R,
                   opts.AsDouble("elevation"),
                   CEAngleType::DEGREES);
    obs.SetRelativeHumidity(opts.AsDouble("humidity"));
    obs.SetPressure_hPa(opts.AsDouble("pressure"));
    obs.SetWavelength_um(opts.AsDouble("wavelength"));

    // Convert the coordinates
    CESkyCoord gal_coords(CEAngle::Deg(opts.AsDouble("glon")),
                          CEAngle::Deg(opts.AsDouble("glat")),
                          CESkyCoordType::GALACTIC);
    CESkyCoord obs_gal_coords;
    CESkyCoord obs_coords;
    CESkyCoord::Galactic2Observed(gal_coords, &obs_coords,
                                  date, obs, &obs_gal_coords);
    
    // Print the results
    PrintResults(opts, obs_coords, obs_gal_coords);
    
    return 0;
}

/***************************************************************************
 *  gal2obs.cpp: CppEphem                                                  *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2016 JCardenzana                                           *
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
#include "CLOptions.h"

/**********************************************************************//**
 * Define the command line options for this program
 *************************************************************************/
CLOptions DefineOpts()
{
    CLOptions opts;

    // Add a program version and description
    std::string vers_str = std::string("gal2obs v") + CPPEPHEM_VERSION;
    opts.AddVersionInfo(vers_str);
    opts.AddProgramDescription(std::string() +
                               "Converts from Galactic coordinates to observed " +
                               "altitude and zenith coordinates for a given " +
                               "Julian date. Additional observing conditions " +
                               "an also be specified to allow more accurate " +
                               "coordinate values.");

    // Set the options
    opts.AddDoubleParam("x,longitude", "Observer longitude (degrees, East-positive)",0.0) ;
    opts.AddDoubleParam("y,latitude", "observer latitude (degrees)", 0.0) ;
    opts.AddDoubleParam("l,glon", "Galactic longitude (degrees)", 0.0);
    opts.AddDoubleParam("b,glat", "Galactic latitude (degrees)", 0.0);
    opts.AddDoubleParam("j,juliandate", "Julian date for query (default is current JD)", CEDate::CurrentJD());
    
    // Add optional observer related parameters
    CEObserver obs;
    opts.AddDoubleParam("e,elevation", "Observer elevation above sea-level (m)", obs.Elevation_m());
    opts.AddDoubleParam("h,humidity", "Observer relative humidity (0-1)", obs.RelativeHumidity());
    opts.AddDoubleParam("p,pressure", "Observer atomospheric pressure (hPa)", obs.Pressure_hPa());
    opts.AddDoubleParam("t,temperature", "Observer temperature (Celsius)", obs.Temperature_C());
    opts.AddDoubleParam("w,wavelength", "Observing wavelength (micrometers)", obs.Wavelength_um());

    return opts;
}


/**********************************************************************//**
 *************************************************************************/
void PrintResults(CLOptions& inputs, std::map<std::string, double> results)
{
    std::printf("\n");
    std::printf("**********************************************\n");
    std::printf("* Results of GALACTIC -> Observed conversion *\n");
    std::printf("**********************************************\n");
    std::printf("Observed Coordinates (output)\n");
    std::printf("    Azimuth        : %f degrees\n", results["azimuth"]*DR2D);
    std::printf("    Zenith         : %+f degrees\n", results["zenith"]*DR2D);
    std::printf("    Altitude       : %+f degrees\n", 90.0-results["zenith"]*DR2D);
    std::printf("Galactic Coordinates (input)\n");
    std::printf("    Gal. Longitude : %f degrees\n", inputs.AsDouble("glon"));
    std::printf("    Gal. Latitude  : %+f degrees\n", inputs.AsDouble("glat"));
    std::printf("    Julian Date    : %f\n", inputs.AsDouble("juliandate"));
    std::printf("Apparent Galactic Coordinates\n");
    std::printf("    Gal. Longitude : %f\n", results["observed_glon"]*DR2D);
    std::printf("    Gal. Latitude  : %+f\n", results["observed_glat"]*DR2D);
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
    CLOptions opts = DefineOpts() ;
    if (opts.ParseCommandLine(argc, argv)) return 0 ;
    
    // Create a map to store the results
    std::map<std::string, double> results ;
    results["azimuth"]       = 0.0;
    results["zenith"]        = 0.0;
    results["observed_glon"] = 0.0;
    results["observed_glat"] = 0.0;
    
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
    int errcode = CECoordinates::Galactic2Observed(opts.AsDouble("glon")*DD2R, 
                                                   opts.AsDouble("glat")*DD2R,
                                                   &results["azimuth"], 
                                                   &results["zenith"],
                                                   date,
                                                   obs,
                                                   CEAngleType::RADIANS,
                                                   obs.Wavelength_um(),
                                                   &results["observed_glon"],
                                                   &results["observed_glat"]);
    
    // Print the results
    PrintResults(opts, results) ;
    
    return errcode ;
}

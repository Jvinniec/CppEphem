/***************************************************************************
 *  obs2cirs.cpp: CppEphem                                                 *
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

#include <stdio.h>
#include <getopt.h>
#include <map>
#include <string>
#include <time.h>
//#include <vector>

// CppEphem HEADERS
#include "CppEphem.h"
#include "CEExecOptions.h"

/**********************************************************************//**
 *************************************************************************/
CEExecOptions DefineOpts()
{
    CEExecOptions opts("obs2cirs");

    // Add a program version and description
    opts.AddProgramDescription(std::string() +
        "Takes observed positions (azimuth,zenith angle) and computes the " +
        "CIRS RA,Dec positions based on the observer location and " +
        "atmospheric properties. The only values necessary to get rough " +
        "coordinates are 'longitude', 'latitude', 'azimuth', 'zenith', " +
        "and 'juliandate'.");

    // Setup the defaults
    opts.AddObservedPars();
    opts.AddObserverPars();
    opts.AddJDPar();
    
    return opts ;
}

/**********************************************************************//**
 *************************************************************************/
void PrintResults(CEExecOptions& inputs, 
                  const std::map<std::string, double>& results)
{
    std::printf("\n") ;
    std::printf("******************************************\n") ;
    std::printf("* Results of Observed -> CIRS conversion *\n") ;
    std::printf("******************************************\n") ;
    std::printf("Observed Coordinates (input)\n") ;
    std::printf("    Azimuth        : %f degrees\n", inputs.AsDouble("azimuth")) ;
    std::printf("    Zenith         : %+f degrees\n", inputs.AsDouble("zenith")) ;
    std::printf("    Altitude       : %+f degrees\n", 90.0-inputs.AsDouble("zenith")) ;
    std::printf("CIRS Coordinates (output)\n") ;
    std::printf("    Right Ascension: %f degrees\n", results.at("ra")) ;
    std::printf("    Declination    : %+f degrees\n", results.at("dec")) ;
    std::printf("Observer Info\n") ;
    std::printf("    Julian Date    : %f\n", inputs.AsDouble("juliandate")) ;
    std::printf("    Longitude      : %f deg\n", inputs.AsDouble("longitude")) ;
    std::printf("    Latitude       : %+f deg\n", inputs.AsDouble("latitude")) ;
    std::printf("    Elevation      : %f meters\n", inputs.AsDouble("elevation")) ;
    std::printf("    Pressure       : %f hPa\n", inputs.AsDouble("pressure")) ;
    std::printf("    Temperature    : %f Celsius\n", inputs.AsDouble("temperature")) ;
    std::printf("    Relative Humid.: %f\n", inputs.AsDouble("humidity")) ;
    std::printf("\n") ;
}

/**********************************************************************//**
 *************************************************************************/
int main(int argc, char** argv) {
    
    // Parse the command line options
    CEExecOptions opts = DefineOpts() ;
    if (opts.ParseCommandLine(argc, argv)) return 0 ;
    
    // Create a map to store the results
    std::map<std::string, double> results ;
    results["ra"] = 0.0 ;
    results["dec"]  = 0.0 ;

    // Define the observer
    CEObserver observer(opts.AsDouble("longitude"),
                        opts.AsDouble("latitude"),
                        opts.AsDouble("elevation"),
                        CEAngleType::DEGREES);
    observer.SetPressure_hPa(opts.AsDouble("pressure"));
    observer.SetTemperature_C(opts.AsDouble("temperature"));
    observer.SetRelativeHumidity(opts.AsDouble("humidity"));
    observer.SetWavelength_um(opts.AsDouble("wavelength"));

    // Define the date
    CEDate date(opts.AsDouble("juliandate"), CEDateType::JD);

    // Convert the coordinates
    int errcode = CECoordinates::Observed2CIRS(opts.AsDouble("azimuth")*DD2R,
                                               opts.AsDouble("zenith")*DD2R,
                                               &results["ra"], &results["dec"],
                                               date, observer, CEAngleType::RADIANS);
    
    // Convert back to degrees
    results["ra"]  *= DR2D;
    results["dec"] *= DR2D;
    
    // Print the results
    PrintResults(opts, results) ;
    
    return errcode ;
}

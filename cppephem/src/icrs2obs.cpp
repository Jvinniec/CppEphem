/***************************************************************************
 *  icrs2obs.cpp: CppEphem                                                 *
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

// CppEphem HEADERS
#include "CppEphem.h"
#include "CEExecOptions.h"

/**********************************************************************//**
 *************************************************************************/
CEExecOptions DefineOpts()
{
    CEExecOptions opts("icrs2obs");
    
    // Add version and description
    opts.AddProgramDescription(std::string() +
        "Converts from ICRS (solar system barycentric) coordinates to " +
        "observed altitude, zenith coordinates for a given Julian date. " +
        "Additional observing parameters can also be supplied for more " +
        "accurate coordinate values");

    // Setup the options
    opts.AddIcrsPars();
    opts.AddObserverPars();
    opts.AddJDPar();
    
    return opts;
}


/**********************************************************************//**
 *************************************************************************/
void PrintResults(const CESkyCoord& input_icrs,
                  const CESkyCoord& observed_altaz,
                  const CESkyCoord& observed_icrs,
                  const CEObserver& observer,
                  const CEDate    & date)
{
    std::printf("\n") ;
    std::printf("******************************************\n") ;
    std::printf("* Results of ICRS -> Observed conversion *\n") ;
    std::printf("******************************************\n") ;
    std::printf("Observed Coordinates (output)\n") ;
    std::printf("    Azimuth        : %f deg\n", observed_altaz.XCoord().Deg()) ;
    std::printf("    Zenith         : %+f deg\n", observed_altaz.YCoord().Deg()) ;
    std::printf("    Altitude       : %+f deg\n", 90.0-observed_altaz.YCoord().Deg()) ;
    std::printf("ICRS Coordinates (input)\n") ;
    std::printf("    Right Ascension: %f deg\n", input_icrs.XCoord().Deg()) ;
    std::printf("    Declination    : %+f deg\n", input_icrs.YCoord().Deg()) ;
    std::printf("    Julian Date    : %f\n", date.JD()) ;
    std::printf("Apparent ICRS Coordinates\n");
    std::printf("    Right Ascension: %f deg\n", observed_icrs.XCoord().Deg());
    std::printf("    Declination    : %f deg\n", observed_icrs.YCoord().Deg());
    std::printf("Observer Info\n") ;
    std::printf("    Longitude      : %f deg\n", observer.Longitude_Deg()) ;
    std::printf("    Latitude       : %+f deg\n", observer.Latitude_Deg()) ;
    std::printf("    Elevation      : %f meters\n", observer.Elevation_m()) ;
    std::printf("    Pressure       : %f hPa\n", observer.Pressure_hPa()) ;
    std::printf("    Temperature    : %f Celsius\n", observer.Temperature_C()) ;
    std::printf("    Relative Humid.: %f\n", observer.RelativeHumidity()) ;
    std::printf("\n") ;
}

/**********************************************************************//**
 * Main
 *************************************************************************/
int main(int argc, char** argv)
{    
    CEExecOptions opts = DefineOpts();
    if (opts.ParseCommandLine(argc, argv)) return 0;
    
    // Create the input and output coordinates
    CESkyCoord icrs_coords(CEAngle::Deg(opts.AsDouble("ra")), 
                           CEAngle::Deg(opts.AsDouble("dec")),
                           CESkyCoordType::ICRS);
    
    // Define the date
    CEDate date(opts.AsDouble("juliandate"), CEDateType::JD);

    // Define the observer
    CEObserver observer(opts.AsDouble("longitude"),
                        opts.AsDouble("latitude"),
                        opts.AsDouble("elevation"),
                        CEAngleType::DEGREES);
    observer.SetPressure_hPa(opts.AsDouble("pressure"));
    observer.SetTemperature_C(opts.AsDouble("temperature"));
    observer.SetRelativeHumidity(opts.AsDouble("humidity"));
    observer.SetWavelength_um(opts.AsDouble("wavelength"));

    // Convert the coordinates
    CESkyCoord obs_coords;
    CESkyCoord obs_icrs_coords;
    CESkyCoord::ICRS2Observed(icrs_coords, &obs_coords, date, observer, &obs_icrs_coords);
    
    // Print the results
    PrintResults(icrs_coords, obs_coords, obs_icrs_coords, observer, date);
    
    return 0 ;
}

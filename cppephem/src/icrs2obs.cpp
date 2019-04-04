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
#include "CLOptions.h"

/**********************************************************************//**
 *************************************************************************/
CLOptions DefineOpts()
{
    // Create a default observer
    CEObserver obs;
    
    // Create the
    CLOptions opts;
    opts.AddDoubleParam("X,longitude", "Observer longitude (degrees)",0.0);
    opts.AddDoubleParam("Y,latitude", "Observer latitude (degrees)",0.0);
    opts.AddDoubleParam("R,ra", "Right Ascension (degrees)", 0.0);
    opts.AddDoubleParam("D,dec", "Declination (degrees)", 0.0);
    opts.AddDoubleParam("j,juliandate", "Julian Date for query", CEDate::CurrentJD());
    opts.AddDoubleParam("e,elevation", "Observer elevation (meters above sea-level)", obs.Elevation_m());
    opts.AddDoubleParam("h,humidity", "Observer's relative humidity (0-1)", obs.RelativeHumidity());
    opts.AddDoubleParam("p,pressure", "Observer's atmospheric pressure (hPa)", obs.Pressure_hPa());
    opts.AddDoubleParam("t,temperature", "Observer's temperature (degrees Celsius)", obs.Temperature_C());
    opts.AddDoubleParam("w,wavelength", "Wavelength of light being observed (micrometers)", 0.5);
    opts.AddDoubleParam("d,dut1", "UT1-UTC", 0.0);
    opts.AddDoubleParam("x,xpolar", "x-polar motion (best to leave alone)", 0.0);
    opts.AddDoubleParam("y,ypolar", "y-polar motion (best to leave alone)", 0.0);
    
    return opts;
}


/**********************************************************************//**
 *************************************************************************/
void PrintResults(CECoordinates& input_icrs,
                  CECoordinates& observed_altaz,
                  CECoordinates& observed_icrs,
                  CEObserver&    observer,
                  CEDate    &    date)
{
    std::printf("\n") ;
    std::printf("******************************************\n") ;
    std::printf("* Results of ICRS -> Observed conversion *\n") ;
    std::printf("******************************************\n") ;
    std::printf("Observed Coordinates (output)\n") ;
    std::printf("    Azimuth        : %f deg\n", observed_altaz.XCoordinate_Deg()) ;
    std::printf("    Zenith         : %+f deg\n", observed_altaz.YCoordinate_Deg()) ;
    std::printf("    Altitude       : %+f deg\n", 90.0-observed_altaz.YCoordinate_Deg()) ;
    std::printf("ICRS Coordinates (input)\n") ;
    std::printf("    Right Ascension: %f deg\n", input_icrs.XCoordinate_Deg()) ;
    std::printf("    Declination    : %+f deg\n", input_icrs.YCoordinate_Deg()) ;
    std::printf("    Julian Date    : %f\n", date.JD()) ;
    //std::printf("Apparent ICRS Coordinates\n");
    //std::printf("    Right Ascension: %f deg\n", observed_icrs.XCoordinate_Deg());
    //std::printf("    Declination    : %f deg\n", observed_icrs.XCoordinate_Deg());
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
    CLOptions opts = DefineOpts();
    if (opts.ParseCommandLine(argc, argv)) return 0;
    
    // Create the input and output coordinates
    CECoordinates input(opts.AsDouble("ra"), opts.AsDouble("dec"),
                        CECoordinateType::ICRS, CEAngleType::DEGREES);
    
    CEDate date(opts.AsDouble("juliandate"), CEDateType::JD);

    CEObserver observer(opts.AsDouble("longitude"),
                        opts.AsDouble("latitude"),
                        opts.AsDouble("elevation"),
                        CEAngleType::DEGREES);
    observer.SetPressure(opts.AsDouble("pressure"));
    observer.SetTemperature_C(opts.AsDouble("temperature"));
    observer.SetRelativeHumidity(opts.AsDouble("humidity"));

    // Convert the coordinates
    CECoordinates output;
    try {
        output = input.GetObservedCoords(date, observer,
                                         opts.AsDouble("dut1"),
                                         opts.AsDouble("xpolar"), 
                                         opts.AsDouble("ypolar"), 
                                         opts.AsDouble("wavelength"));
    } catch (CEException & e) {
        std::cerr << e.what() << std::endl;
    }
    
    // Print the results
    PrintResults(input, output, output, observer, date) ;
    
    return 0 ;
}

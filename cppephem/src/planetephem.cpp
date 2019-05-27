/***************************************************************************
 *  planetephem.cpp: CppEphem                                              *
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

#include <algorithm>
#include <stdio.h>

#include "CppEphem.h"
#include "CEExecOptions.h"


/**********************************************************************//**
 * Forward declarations
 *************************************************************************/
CEExecOptions DefineOpts() ;
CEPlanet GetPlanet(const int& p_id) ;
void PrintEphemeris(CEObservation& obs, 
                    const double&  duration, 
                    const double&  step_size) ;

/**********************************************************************//**
 * Main method
 *************************************************************************/
int main(int argc, char** argv)
{
    // Get the command line options
    CEExecOptions opts = DefineOpts() ;
    if (opts.ParseCommandLine(argc,argv)) return 0;
    
    // Turn on interpolation for correction terms
    CppEphem::CorrectionsInterp(true);

    // Create the observed object
    CEPlanet planet = GetPlanet(opts.AsInt("planet")) ;
    if (planet.Name().empty()) return 0 ;

    // Set the algorithm
    std::string method = opts.AsString("method");
    std::transform(method.begin(), method.end(), method.begin(), ::tolower);
    if (method == "sofa") {
        planet.SetAlgorithm(CEPlanetAlgo::SOFA);
    } else if (method == "jpl") {
        planet.SetAlgorithm(CEPlanetAlgo::JPL);
    } else {
        throw CEException::invalid_value("planetephem.cpp", 
                                "[ERROR] Invalid planet algorithm: " + method);
    }
    
    // Create the date object
    CEDate date(opts.AsDouble("startJD"), CEDateType::JD) ;
    
    // Create the observer
    CEObserver observer(opts.AsDouble("longitude"),
                        opts.AsDouble("latitude"),
                        opts.AsDouble("elevation"),
                        CEAngleType::DEGREES) ;
    observer.SetPressure_hPa(opts.AsDouble("pressure"));
    observer.SetTemperature_C(opts.AsDouble("temperature"));
    observer.SetRelativeHumidity(opts.AsDouble("humidity"));
    observer.SetWavelength_um(opts.AsDouble("wavelength"));
    observer.SetUTCOffset(CETime::SystemUTCOffset_hrs());
    
    CEObservation coords(&observer, &planet, &date) ;
    
    // Now list the information requested
    PrintEphemeris(coords, opts.AsDouble("duration"), opts.AsDouble("step")) ;
    
    return 0;
}

/**********************************************************************//**
 * Set the command line options
 *************************************************************************/
CEExecOptions DefineOpts()
{
    CEExecOptions opts("planetephem");

    // Add version and description
    opts.AddProgramDescription(std::string() +
        "Returns the apparent ICRS RA,Dec and observed Az,Alt coordinates for " +
        "a given planet for a given observer. It is important also to note " +
        "observed coordinates will be dubious very close to the horizon due " +
        "to the atmospheric correction algorithms employed.");

    // Define the parameters
    opts.AddIntParam("p,planet",
                     "Planet number (1=Mercury, 2=Venus, 4=Mars, 5=Jupiter, 6=Saturn, 7=Uranus, 8=Neptune)",
                     0);

    // Observer parameters
    opts.AddObserverPars();

    // Time information
    opts.AddDoubleParam("startJD",
                        "Starting time as a Julian date. Default will be the current Julian date",
                        CEDate::CurrentJD());
    opts.AddDoubleParam("duration",
                        "Duration for printing out results (in minutes). Results will be printed from 'startJD' to 'startJD + duration'. Default is for one 24 hour period",
                        24.0*60.0);
    opts.AddDoubleParam("step",
                        "Number of minutes between each line of the print out.",
                        30.0);
    opts.AddStringParam("method",
                        "Method for computing planet positions ('sofa' or 'jpl', default='sofa')",
                        "sofa");
    return opts ;
}

/**********************************************************************//**
 * Returns the planet requested by the user
 *************************************************************************/
CEPlanet GetPlanet(const int& p_id)
{
    switch (p_id)
    {
        case 1:
            return CEPlanet::Mercury();
        case 2:
            return CEPlanet::Venus();
        case 4:
            return CEPlanet::Mars();
        case 5:
            return CEPlanet::Jupiter();
        case 6:
            return CEPlanet::Saturn();
        case 7:
            return CEPlanet::Uranus();
        case 8:
            return CEPlanet::Neptune();
        default:
            std::string msg = std::string() + "[ERROR] Planet ID (" +
                              std::to_string(p_id) + ") is not valid!";
            throw CEException::invalid_value("planetephem, GetPlanet()", msg);
    }
}

/**********************************************************************//**
 * Prints the results of the planet position query
 *************************************************************************/
void PrintEphemeris(CEObservation& obs, 
                    const double&  duration, 
                    const double&  step_size)
{
    // Lets get the objects in obs
    CEDate* date = obs.Date();
    CEPlanet* planet = dynamic_cast<CEPlanet*>( obs.Body() );
    CEObserver* observer = obs.Observer();
    std::vector<double> localtime = CETime::TimeDbl2Vect(date->GetTime(observer->UTCOffset()));
    
    // Print some information about the observer
    std::vector<double> long_hms = CECoordinates::GetDMS( observer->Longitude_Deg(),
                                                          CEAngleType::DEGREES );
    std::vector<double> lat_hms  = CECoordinates::GetDMS( observer->Latitude_Deg(),
                                                          CEAngleType::DEGREES );
    std::printf("\n") ;
    std::printf("= OBSERVER ===================\n");
    std::printf("  Longitude: %+4dd %02dm %4.1fs\n", int(long_hms[0]), int(long_hms[1]), long_hms[2]+long_hms[3]);
    std::printf("  Latitude :  %+3dd %02dm %4.1fs\n", int(lat_hms[0]), int(lat_hms[1]), lat_hms[2]+lat_hms[3]);
    std::printf("  Elevation: %f m \n", observer->Elevation_m());
    std::printf("  Pressure : %f hPa\n", observer->Pressure_hPa());
    std::printf("  Temp     : %f Celsius\n", observer->Temperature_C());
    std::printf("  Humidity : %f %%\n", 100.0*observer->RelativeHumidity());
    std::printf("  Wavelen. : %f micrometers\n", observer->Wavelength_um());
    std::printf("  LocalTime: %02d:%02d:%04.1f\n", int(localtime[0]), int(localtime[1]), localtime[2]+localtime[3]);
    std::printf("  UTCOffset: %2d hrs\n\n", int(observer->UTCOffset()));

    // Print some basic information regarding the planet itself
    std::printf("= PLANET =====================\n");
    std::printf("  Name  : %s\n", planet->Name().c_str());
    std::printf("  Mass  : %e kg\n", planet->Mass_kg());
    std::printf("  Radius: %f km\n", planet->Radius_m()/1000.0);
    std::printf("  Albedo: %f\n\n", planet->Albedo());
    
    std::vector<double> ra;
    std::vector<double> dec;
    
    // Now do the stuff we actually want
    std::printf("= NOTES ======================\n");
    std::printf("  * RA,DEC represent apparent ICRS coordinates for the observer\n");
    std::printf("  * Coordinates should be considered dubious at small altitudes\n");
    std::printf("\n");
    std::printf("      JD        LOCAL     RA (appar.)    DEC (appar.)     Az       Alt  \n") ;
    std::printf(" =======================================================================\n") ;
    int max_steps = int(duration/step_size);
    CECoordinates obs_coords;
    CECoordinates appar_coords;
    for (int s=0; s<=max_steps; s++) {

        // Get the observed coordinates
        obs_coords.SetCoordinates(obs.GetAzimuth_Rad(), obs.GetZenith_Rad(),
                                  CECoordinateType::OBSERVED);
        // Convert to RA,DEC
        appar_coords = obs_coords.ConvertToICRS(date->JD(),
                                                observer->Longitude_Rad(),
                                                observer->Latitude_Rad(),
                                                observer->Elevation_m(),
                                                observer->Pressure_hPa(),
                                                observer->Temperature_C(),
                                                observer->RelativeHumidity(),
                                                date->dut1(),
                                                date->xpolar(),
                                                date->ypolar(),
                                                observer->Wavelength_um());
        // Update the coordiantes of the planet
        ra  = appar_coords.XCoord().HmsVect();
        dec = appar_coords.YCoord().DmsVect();
        
        std::printf(" %11.2f  %08.1f  %2.0fh %2.0fm %4.1fs  %+3.0fd %2.0fm %4.1fs  %8.3f  %+7.3f\n",
                    double(*date), date->GetTime(observer->UTCOffset()),
                    ra[0], ra[1], ra[2] + ra[3],
                    dec[0], dec[1], dec[2] + dec[3],
                    obs_coords.XCoordinate_Deg(),
                    90.0-obs_coords.YCoordinate_Deg());
        
        // Update the date
        date->SetDate(date->JD() + step_size/(60.0*24.0), CEDateType::JD);
    }
    
    std::printf(" -------------------------------------------------------------------\n");
}

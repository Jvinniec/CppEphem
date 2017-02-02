//
//  planetephem.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 2/1/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

#include <stdio.h>

#include "CppEphem.h"
#include "CLOptions.h"


/*********************************************
 * Forward declarations
 *********************************************/
CLOptions DefineOpts() ;
CEPlanet GetPlanet(int p_id) ;
void PrintEphemeris(CEObservation& obs, double duration, double step_size) ;

/*********************************************
 * Main method
 *********************************************/
int main(int argc, char** argv)
{
    // Get the command line options
    CLOptions opts = DefineOpts() ;
    if (opts.ParseCommandLine(argc,argv)) return 0;
    
    // Create the observed object
    CEPlanet planet = GetPlanet(opts.AsInt("planet")) ;
    if (planet.Name().empty()) return 0 ;
    
    // Create the date object
    CEDate date(opts.AsDouble("startJD")) ;
    
    // Create the observer,
    CEObserver observer(opts.AsDouble("longitude"),
                        opts.AsDouble("latitude"),
                        opts.AsDouble("elevation"),
                        CEAngleType::DEGREES,
                        &date) ;
    
    CEObservation coords(&observer, &planet) ;
    
    // Now list the information requested
    PrintEphemeris(coords, opts.AsDouble("duration"), opts.AsDouble("step")) ;
    
    return 0;
}

/*********************************************
 * Set the command line options
 *********************************************/
CLOptions DefineOpts()
{
    CLOptions opts ;
    opts.AddProgramDescription("Returns the RA,Dec and observed Az,Alt coordinates for a given planet for a given observer.") ;
    opts.AddIntParam("p,planet",
                     "Planet number (1=Mercury, 2=Venus, 4=Mars, 5=Jupiter, 6=Saturn, 7=Uranus, 8=Neptune)",
                     0) ;
    opts.AddDoubleParam("l,longitude",
                        "Observer geographic longitude (degrees, east positive)",
                        0.0) ;
    opts.AddDoubleParam("a,latitude",
                        "Observer geographic latitude (degrees)",
                        0.0) ;
    opts.AddDoubleParam("e,elevation",
                        "Observer elevation (meters above sea level)",
                        0.0) ;
    opts.AddDoubleParam("t,startJD",
                        "Starting time as a Julian date. Default will be the current Julian date",
                        CEDate::CurrentJD()) ;
    opts.AddDoubleParam("d,duration",
                        "Duration for printing out results (in minutes). Results will be printed from 'startJD' to 'startJD + duration'. Default is for one 24 hour period",
                        24.0*60.0);
    opts.AddDoubleParam("s,step",
                        "Number of minutes between each line of the print out.",
                        30.0) ;
    return opts ;
}

/*********************************************
 * Returns the planet requested by the user
 *********************************************/
CEPlanet GetPlanet(int p_id)
{
    switch (p_id)
    {
        case 1:
            return CEPlanet::Mercury() ;
        case 2:
            return CEPlanet::Venus() ;
        case 4:
            return CEPlanet::Mars() ;
        case 5:
            return CEPlanet::Jupiter() ;
        case 6:
            return CEPlanet::Saturn() ;
        case 7:
            return CEPlanet::Uranus() ;
        case 8:
            return CEPlanet::Neptune() ;
        default:
            std::cout << "[ERROR] Planet ID (" << p_id << ") is not valid!" << std::endl;
            CEPlanet noplanet ;
            return noplanet ;
    }
}

/*********************************************
 * Returns the planet requested by the user
 *********************************************/
void PrintEphemeris(CEObservation& obs, double duration, double step_size)
{
    // Lets get the objects in obs
    CEDate* date = obs.Date() ;
    CEPlanet* planet = dynamic_cast<CEPlanet*>( obs.Body() ) ;
    CEObserver* observer = obs.Observer() ;
    
    // Print some information about the observer
    std::vector<double> long_hms = CECoordinates::GetDMS( observer->Longitude_Deg() ) ;
    std::vector<double> lat_hms = CECoordinates::GetDMS( observer->Latitude_Deg() ) ;
    std::printf("\n") ;
    std::printf("= OBSERVER ===================\n");
    std::printf("  Longitude: %+4dd %02dm %4.1fs\n", int(long_hms[0]), int(long_hms[1]), long_hms[2]) ;
    std::printf("  Latitude :  %+2dd %02dm %4.1fs\n", int(lat_hms[0]), int(lat_hms[1]), lat_hms[2]) ;
    std::printf("  Elevation: %f m \n\n", observer->Elevation_m()) ;
    
    // Print some basic information regarding the planet itself
    std::printf("= PLANET =====================\n") ;
    std::printf("  Name  : %s\n", planet->Name().c_str()) ;
    std::printf("  Mass  : %e kg\n", planet->Mass_kg()) ;
    std::printf("  Radius: %f km\n", planet->Radius_m()/1000.0) ;
    std::printf("  Albedo: %f\n\n", planet->Albedo()) ;
    
    std::vector<double> ra, dec ;
    
    // Now do the stuff we actually want
    std::printf("    JD      UTC          RA              DEC          Az       Alt  \n") ;
    std::printf(" ===================================================================\n") ;
    int max_steps = int(duration/step_size) ;
    for (int s=0; s<max_steps; s++) {
        // Set the date
        date->SetDate(*date + step_size/(60.0*24.0)) ;
        
        ra = CECoordinates::GetHMS( obs.GetApparentXCoordinate_Deg() ) ;
        dec = CECoordinates::GetDMS( obs.GetApparentYCoordinate_Deg() ) ;
        
        std::printf(" %8d %08.1f  %2.0fh %2.0fm %4.1fs  %+3.0fd %2.0fm %4.1fs  %8.3f  %+7.3f\n",
                    int(*date), date->GetTime_UTC(),
                    ra[0], ra[1], ra[2],
                    dec[0], dec[1], dec[2],
                    obs.GetAzimuth_Deg(),
                    90.0-obs.GetZenith_Deg()) ;
        
    }
    
    std::printf(" -------------------------------------------------------------------\n") ;
}

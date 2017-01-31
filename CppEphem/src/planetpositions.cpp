//
//  planetpositions.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 1/30/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

/** \file planetpositions.cpp
 * This script allows viewing the current coordinates of a given
 * solar system planet for dates within 3000 B.C. to 3000 A.D.
 * NOTE: The coordinates should be accurate to within ~0.5 degrees for
 *       inner solar system planets, and ~1-2 degrees for outer planets.
 */

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <ncurses.h>
#include "CLOptions.h"

#include "CppEphem.h"

/**
 * Define the command line options for this program
 */
CLOptions DefineOptions()
{
    CLOptions options ;
    options.AddDoubleParam("L,longitude","Geographic observer longitude (degrees, east positive)",-93.62) ;
    options.AddDoubleParam("B,latitude", "Geographic observer latitude (degrees)", 42.0347) ;
    options.AddDoubleParam("e,elevation","Observer elevation above sea-level (meters)", 287.0) ;
    options.AddIntParam("u,UpdateFrequency", "Number of milliseconds between updates", 1000) ;
    options.AddDoubleParam("o,UTCOffset", "Observer offset from UTC time", -6.0) ;
    
    return options ;
}

int Description() ;
/**
 * Main
 */
int main(int argc, char** argv)
{
    // Parse the command line options
    CLOptions opt = DefineOptions() ;
    if (opt.ParseCommandLine(argc, argv)) return 0.0 ;
    
    // Create the object representing Mercury
    std::vector<CEPlanet*> planets(8) ;
    planets[0] = new CEPlanet(CEPlanet::Mercury()) ;
    planets[1] = new CEPlanet(CEPlanet::Venus()) ;
    planets[2] = new CEPlanet(CEPlanet::Mars()) ;
    planets[3] = new CEPlanet(CEPlanet::Jupiter()) ;
    planets[4] = new CEPlanet(CEPlanet::Saturn()) ;
    planets[5] = new CEPlanet(CEPlanet::Uranus()) ;
    planets[6] = new CEPlanet(CEPlanet::Neptune()) ;
    planets[7] = new CEPlanet(CEPlanet::Pluto()) ;

    // Create an object that always represents the current instantaneous time
    CEDate date ;
    
    // Create a default observer, longitude and latitude in degrees. I'm using Ames, IA, USA.
    // Note that longitude is "east-positive" from Greenwich.
    CEObserver observer(opt.AsDouble("longitude"), opt.AsDouble("latitude"),
                        opt.AsDouble("elevation"), CEAngleType::DEGREES, &date) ;
    observer.SetUTCOffset(opt.AsDouble("UTCOffset")) ;
    
    // Now get the coordinates of the object as a constantly updating 'CEObservation' object
    std::vector<CEObservation> coords(planets.size());
    coords[0] = CEObservation(&observer, planets[0]) ;
    coords[1] = CEObservation(&observer, planets[1]) ;
    coords[2] = CEObservation(&observer, planets[2]) ;
    coords[3] = CEObservation(&observer, planets[3]) ;
    coords[4] = CEObservation(&observer, planets[4]) ;
    coords[5] = CEObservation(&observer, planets[5]) ;
    coords[6] = CEObservation(&observer, planets[6]) ;
    coords[7] = CEObservation(&observer, planets[7]) ;

    int update_freq(opt.AsInt("UpdateFrequency")) ;
    
    // Now describe the results
    WINDOW * mainwin;
    
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    
    int line = Description() ;
    
    mvaddstr(line+6,1, " +-----------+---------------+---------------+--------+--------+---------+") ;
    mvaddstr(line+7,1, " |   Name    |      RA       |      DEC      |   Az   |   Alt  | Zenith  |") ;
    mvaddstr(line+8,1, " +-----------+---------------+---------------+--------+--------+---------+") ;
    mvaddstr(line+9+planets.size(),1, " +-----------+---------------+---------------+--------+--------+---------+") ;
    
    char str[200] ;
    std::vector<double> ra_vec, dec_vec ;
    double ra, dec ;
    while (true) {
        usleep(update_freq*1000);
        
        // Update the date to the current time
        date.SetDate() ;
        
        // Print date and time information
        std::string jd_str    = "JD   : " + std::to_string(int(date)) ;
        std::string date_str  = "Date : " + std::to_string(int(date.Gregorian())) ;
        std::string utc_str   = "UTC  : " + std::to_string(date.GetTime_UTC()) ;
        std::string local_str = "Local: " + std::to_string(date.GetTime(observer.UTCOffset())) ;
        mvaddstr(line+1,1, jd_str.c_str()) ;
        mvaddstr(line+2,1, utc_str.c_str()) ;
        mvaddstr(line+3,1, date_str.c_str()) ;
        mvaddstr(line+4,1, local_str.c_str()) ;
        
        // Loop through each planet and print it's respective information
        for (int p=0; p<planets.size(); p++) {
            // Get the RA,Dec
            ra = planets[p]->XCoordinate_Deg(date) ;
            dec = planets[p]->YCoordinate_Deg(date) ;
            // Format the RA,Dec
            ra_vec = CECoordinates::GetHMS(ra) ;
            dec_vec = CECoordinates::GetDMS(dec) ;
            // Generate the new line to be printed
            std::snprintf(str, 200, " |%10s | %2.0fh %2.0fm %4.1fs |%+3.0fd %2.0fm %4.1fs |%8.3f|%8.3f|%8.3f |",
                          planets[p]->Name().c_str(),
                          ra_vec[0], ra_vec[1], ra_vec[2],
                          dec_vec[0], dec_vec[1], dec_vec[2],
                          coords[p].GetAzimuth_Deg(),
                          90.0-coords[p].GetZenith_Deg(),
                          coords[p].GetZenith_Deg()) ;
            // Add the line to the output
            mvaddstr(line+9+p,1,str) ;
        }
        
        refresh();
    }
    
    delwin(mainwin);
    endwin();
    refresh();
}

int Description()
{
    int line(0) ;
    mvaddstr(line++, 1, "DESCRIPTION:") ;
    mvaddstr(line++, 3, "This program prints the current positions of the planets to the terminal.") ;
    mvaddstr(line++, 3, "NOTE: Az,Alt,Zenith values are relative to Earth-Moon barycenter.") ;
    return line ;
}
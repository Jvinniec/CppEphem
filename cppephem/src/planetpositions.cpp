/***************************************************************************
 *  planetpositions.cpp: CppEphem                                          *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2017 JCardenzana                                           *
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

/**********************************************************************//**
 * Define the command line options for this program
 *************************************************************************/
CLOptions DefineOptions()
{
    CLOptions options ;
    options.AddDoubleParam("L,longitude","Geographic observer longitude (degrees, east positive)",-93.62) ;
    options.AddDoubleParam("B,latitude", "Geographic observer latitude (degrees)", 42.0347) ;
    options.AddDoubleParam("e,elevation","Observer elevation above sea-level (meters)", 287.0) ;
    options.AddDoubleParam("o,UTCOffset", "Observer offset from UTC time (default is current system UTC offset)", CETime::SystemUTCOffset_hrs()) ;
    options.AddIntParam("u,UpdateFrequency", "Number of milliseconds between updates", 1000) ;
    options.AddIntParam("a,algorithm","Sets the algorithm used to compute planet positions (1=SOFA,2=JPL)",1) ;
    options.AddBoolParam("m,DrawMap", "Draws a crude map of the southern sky with visible planets", true) ;
    
    return options ;
}

/**********************************************************************//**
 * Forward Declarations
 *************************************************************************/

void DrawSkyMap(std::vector<CEObservation> &coords, int line) ;
void DrawGrid(int line) ;
int Description() ;


/**********************************************************************//**
 * Main
 *************************************************************************/
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
    
    for (int i=0; i<planets.size(); i++) {
        if (opt.AsInt("algorithm") == 1) {
            planets[i]->SetAlgorithm(CEPlanetAlgo::SOFA) ;
        } else if (opt.AsInt("algorithm") == 2) {
            planets[i]->SetAlgorithm(CEPlanetAlgo::JPL) ;
        }
    }
    
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
    
    mvaddstr(line+6,5, " +-----------+---------------+---------------+--------+--------+---------+") ;
    mvaddstr(line+7,5, " |   Name    |      RA       |      DEC      |   Az   |   Alt  | Zenith  |") ;
    mvaddstr(line+8,5, " +-----------+---------------+---------------+--------+--------+---------+") ;
    mvaddstr(line+9+planets.size(),5, " +-----------+---------------+---------------+--------+--------+---------+") ;
    
    char str[200] ;
    std::vector<double> ra_vec, dec_vec, az(planets.size()), zen(planets.size()) ;
    double ra, dec ;
    while (true) {
        usleep(update_freq*1000);
        
        // Update the date to the current time
        date.SetDate() ;
        // Compute tdb
        double jd_tdb = CEDate::UTC2TDB( date ) ;
        
        // Print date and time information
        std::string jd_str    = "JD   : " + std::to_string(date) +" ("+ std::to_string(jd_tdb) +")";
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
            ra = planets[p]->XCoordinate_Deg(jd_tdb) ;
            dec = planets[p]->YCoordinate_Deg(jd_tdb) ;
            // Format the RA,Dec
            ra_vec = CECoordinates::GetHMS(ra) ;
            dec_vec = CECoordinates::GetDMS(dec) ;
            // Get the alt,az

            
            // Generate the new line to be printed
            std::snprintf(str, 200, " |%10s | %2.0fh %2.0fm %4.1fs |%+3.0fd %2.0fm %4.1fs |%8.3f|%8.3f|%8.3f |",
                          planets[p]->Name().c_str(),
                          ra_vec[0], ra_vec[1], ra_vec[2],
                          dec_vec[0], dec_vec[1], dec_vec[2],
                          coords[p].GetAzimuth_Deg(),
                          90.0-coords[p].GetZenith_Deg(),
                          coords[p].GetZenith_Deg()) ;
            // Add the line to the output
            mvaddstr(line+9+p,5,str) ;
        }
        
        // Draw a little picture that represents the southern sky for the observer
        if (opt.AsBool("DrawMap")) DrawSkyMap(coords, line+9+planets.size()+2) ;
        
        refresh();
    }
    
    delwin(mainwin);
    endwin();
    refresh();
}

/**********************************************************************//**
 *************************************************************************/
void DrawSkyMap(std::vector<CEObservation> &coords, int line)
{
    DrawGrid(line) ;
    // Loop through the planet objects and plot their alt,az values
//    int x,y ;
    double az,zen ;
    for (int p=0; p<coords.size(); p++) {
        az = coords[p].GetAzimuth_Deg() ;
        if ((az < 90.0) || (az > 270.0)) continue ;
        zen = coords[p].GetZenith_Deg() ;
        if (zen > 90.0) continue ;
        int x = 6+(az-90.0)/2.0 ;
        int y = line+(zen/5.0)+1 ;
        std::string abbrv( coords[p].Body()->Name() ) ;
        abbrv[1] = '\0' ;
        mvaddstr(y, x, abbrv.c_str()) ;
    }
}

/**********************************************************************//**
 *************************************************************************/
void DrawGrid(int line)
{
    // Draw the coordinate grid first
    mvaddstr(line++, 1, "  90 +--------------+--------------+--------------+--------------+--------------+--------------+") ;
    mvaddstr(line++, 1, "A    |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "L 80 |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "T    |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "I 70 |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "T    |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "U 60 |............................................|............................................|") ;
    mvaddstr(line++, 1, "D    |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "E 50 |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "     |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "  40 |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "     |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "  30 |............................................|............................................|") ;
    mvaddstr(line++, 1, "     |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "  20 |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "     |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "  10 |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "     |              .              .              |              .              .              |") ;
    mvaddstr(line++, 1, "   0 |______________.______________.______________|______________.______________.______________|") ;
    mvaddstr(line++, 1, "     90            120            150            180            210            240            270") ;
    mvaddstr(line++, 1, "     E                  AZIMUTH                   S                   AZIMUTH                  W") ;
    
}

/**********************************************************************//**
 *************************************************************************/
int Description()
{
    int line(0) ;
    mvaddstr(line++, 1, "DESCRIPTION:") ;
    mvaddstr(line++, 3, "This program prints the current positions of the planets to the terminal.") ;
//    mvaddstr(line++, 3, "NOTE: Az,Alt,Zenith values are relative to Earth-Moon barycenter.") ;
    return line ;
}

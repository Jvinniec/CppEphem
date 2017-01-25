//
//  CEPlanet.h
//  CppEphem
//
//  Created by Josh Cardenzana on 1/13/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

#ifndef CEPlanet_h
#define CEPlanet_h

#include "CEBody.h"

class CEPlanet : public CEBody {
public:
    CEPlanet() ;
    CEPlanet(const std::string& name, double xcoord, double ycoord,
             CECoordinateType coord_type = CECoordinateType::CIRS,
             CEAngleType angle_type = CEAngleType::RADIANS) ;
    CEPlanet(const std::string& name,
             CECoordinates coordinates) ;
    virtual ~CEPlanet() ;
    
    /****************************
     * Atribute getters
     ****************************/
    /// @return Radius in meters.
    double Radius_m() {return radius_m_ ;}
    /// @return Mass in kilograms.
    double Mass_kg() {return mass_kg_ ;}
    
    /****************************
     * Atribute setters
     ****************************/
    /// @param[in] new_radius New radius (meters)
    void SetRadius_m(double new_radius) {radius_m_ = new_radius ;}
    /// @param[in] new_mass New mass (kilograms)
    void SetMass_kg(double new_mass) {mass_kg_ = new_mass ;}
    
    /****************************
     * Overloads to compute the planets position
     ****************************/
    
    
    /****************************
     * Methods for computing apparent "phase"
     ****************************/
    
    /****************************
     * Methods for computing apparent magnetude
     ****************************/
    
    /****************************
     * Some generic planets in our solar system
     ****************************/
    class Mercury ;
    class Venus ;
    class Earth ;
    class Mars ;
    class Jupiter ;
    class Saturn ;
    class Uranus ;
    class Neptune ;
    class Pluto ;
    
protected:
    
    // Basic properties
    double radius_m_ ;          ///< Radius (in meters)
    double mass_kg_ ;           ///< Mass (kilograms)
    
    // Orbital properties
    double semi_major_axis_au_ = 0.0 ;          ///< Semi major axis in Astronomical Units (AU)
    double eccentricity_ = 0.0 ;                ///< Eccentricity
    double inclination_rad_ = 0.0 ;             ///< inclination in radians
    double mean_longitude_rad_ = 0.0 ;          ///< Mean longitude (radians)
    double perihelion_lon_rad_ = 0.0 ;          ///< Longitude of perihelion (radians)
    double ascending_node_lon_rad_ = 0.0 ;      ///< Longitude of ascending node (radians)
    
    // Derivatives of orbital properties
    double semi_major_axis_au_per_cent_ = 0.0 ;
    double eccentricity_per_cent_ = 0.0 ;
    double inclination_rad_per_cent_ = 0.0 ;
    double mean_longitude_rad_per_cent_ = 0.0 ;
    double perihelion_long_rad_per_cent_ = 0.0 ;
    double ascending_node_lon_rad_per_cent_ = 0.0 ;
};

#endif /* CEPlanet_h */

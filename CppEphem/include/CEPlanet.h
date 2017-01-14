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
    CEPlanet(const std::string& name,
             CECoordinates coordinates) ;
    virtual ~CEPlanet() ;
    
    /// Return the name of the object
    std::string GetName() {return name_ ;}
    
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
    /// @param new_radius New radius (meters)
    void SetRadius_m(double new_radius) {radius_m_ = new_radius ;}
    /// @param new_mass New mass (kilograms)
    void SetMass_kg(double new_mass) {mass_kg_ = new_mass ;}
    
protected:
    
    // Basic properties
    std::string name_ ;         ///< Some name for this planet object
    double radius_m_ ;          ///< Radius (in meters)
    double mass_kg_ ;           ///< Mass (kilograms)
    
    // Orbital characteristics
    // TODO: add orbital characteristics
};

#endif /* CEPlanet_h */

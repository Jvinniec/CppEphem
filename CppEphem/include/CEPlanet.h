//
//  CEPlanet.h
//  CppEphem
//
//  Created by Josh Cardenzana on 1/13/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

#ifndef CEPlanet_h
#define CEPlanet_h

#include <cmath>
#include "CEBody.h"

class CEPlanet : public CEBody {
public:
    CEPlanet() ;
    CEPlanet(const std::string& name, double xcoord, double ycoord,
             CECoordinateType coord_type = CECoordinateType::ICRS,
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
    virtual double XCoordinate_Rad(double new_date=-1.0e30)
        {
            UpdateCoordinates(new_date) ;
            return CECoordinates::XCoordinate_Rad() ;
        }
    virtual double XCoordinate_Deg(double new_date=-1.0e30)
        {return XCoordinate_Rad(new_date)*DR2D ;}
    virtual double YCoordinate_Rad(double new_date=-1.0e30)
        {
            UpdateCoordinates(new_date) ;
            return CECoordinates::YCoordinate_Rad() ;
        }
    virtual double YCoordinate_Deg(double new_date=-1.0e30)
        {return YCoordinate_Rad(new_date)*DR2D ;}
    virtual void UpdateCoordinates(double new_date=-1.0e30) ;
    
    /****************************
     * Methods for computing apparent "phase"
     ****************************/
    
    
    /****************************
     * Methods for computing apparent magnitude (i.e. a measure of brightness)
     ****************************/
    
    
    /****************************
     * Methods to set orbital properties
     ****************************/
    virtual void SetSemiMajorAxis_AU(double semi_major_axis_au,
                                double semi_major_axis_au_per_cent=0.0) ;
    virtual void SetEccentricity(double eccentricity,
                                double eccentricity_per_cent=0.0) ;
    virtual void SetInclination(double inclination,
                                double inclination_per_cent=0.0,
                                CEAngleType angle_type=CEAngleType::DEGREES) ;
    virtual void SetMeanLongitude(double mean_longitude,
                                double mean_longitude_per_cent=0.0,
                                CEAngleType angle_type=CEAngleType::DEGREES) ;
    virtual void SetPerihelionLongitude(double perihelion_lon,
                                double perihelion_lon_per_cent=0.0,
                                CEAngleType angle_type=CEAngleType::DEGREES) ;
    virtual void SetAscendingNodeLongitude(double ascending_node_lon,
                                double ascending_node_lon_per_cent=0.0,
                                CEAngleType angle_type=CEAngleType::DEGREES) ;

    /// Set the tolerance for the eccentric anomoly recursive formula
    ///     @param[in] tol          // Tolerance (degrees)
    virtual void SetTolerance(double tol = 1.0e-6) {E_tol = tol ;}
    
    /****************************
     * Some generic planets in our solar system
     ****************************/
    static CEPlanet Mercury() ;
    static CEPlanet* Venus() {return nullptr ;}
    static CEPlanet* Earth() {return nullptr ;}
    static CEPlanet Mars() ;
    static CEPlanet* Jupiter() {return nullptr ;}
    static CEPlanet* Saturn() {return nullptr ;}
    static CEPlanet* Uranus() {return nullptr ;}
    static CEPlanet* Neptune() {return nullptr ;}
    static CEPlanet* Pluto() {return nullptr ;}
    
protected:
    
    // Basic properties
    double radius_m_ ;          ///< Radius (in meters)
    double mass_kg_ ;           ///< Mass (kilograms)
    
    // The coordinates representing the current position will need to be
    // relative to some date, since planets move. This is the cached date
    double cached_jd_ = 0.0 ;                   ///< Julian date of the current coordinates
    double x_icrs_ = 0.0 ;                      ///< X-Coordinate relative to solar system barycenter
    double y_icrs_ = 0.0 ;                      ///< Y-Coordinate relative to solar system barycenter
    double z_icrs_ = 0.0 ;                      ///< Z-Coordinate relative to solar system barycenter
    
    // Orbital properties
    double semi_major_axis_au_ = 0.0 ;          ///< a - Semi major axis in Astronomical Units (AU)
    double eccentricity_ = 0.0 ;                ///< e - Eccentricity
    double inclination_deg_ = 0.0 ;             ///< I - inclination in radians
    double mean_longitude_deg_ = 0.0 ;          ///< L - Mean longitude (radians)
    double perihelion_lon_deg_ = 0.0 ;          ///< w - Longitude of perihelion (radians)
    double ascending_node_lon_deg_ = 0.0 ;      ///< Omega - Longitude of ascending node (radians)
    
    // Derivatives of orbital properties
    double semi_major_axis_au_per_cent_ = 0.0 ;
    double eccentricity_per_cent_ = 0.0 ;
    double inclination_deg_per_cent_ = 0.0 ;
    double mean_longitude_deg_per_cent_ = 0.0 ;
    double perihelion_long_deg_per_cent_ = 0.0 ;
    double ascending_node_lon_deg_per_cent_ = 0.0 ;
    
    // The following is the tolerance for the computation of eccentric anomoly
    double E_tol = 1.0e-6 ;
    
    // Extra terms for outer planets (Jupiter, Saturn, Uranus, Neptune, and Pluto)
    double b_ = 0.0 ;
    double c_ = 0.0 ;
    double s_ = 0.0 ;
    double f_ = 0.0 ;   // TODO: Check whether f_ needs to be converted to radians
    
    // Methods for value computation

    /// Compute current value of a given element
    ///     @param[in] value                    Once of the orbital property values
    ///     @param[in] value_derivative_        Time derivative of 'value'
    ///     @param[in] time                     Days since J2000 epoch
    inline double ComputeElement(double value, double value_derivative_, double time)
        {return value + value_derivative_*time;}
    double MeanAnomaly(double mean_longitude_deg,
                       double perihelion_long_deg,
                       double T=0.0)
        {
            double M = mean_longitude_deg - perihelion_long_deg + (b_*T*T)
                        + c_*std::cos(f_*T*DD2R) + s_*std::sin(f_*T*DD2R) ;
            while (M>180.0) M-=360.0 ;
            while (M<-180.0) M+=360.0 ;
            
            return M ;
        }
    
    // Recursive formula necessary for the computation of eccentric anomoly
    double EccentricAnomoly(double& M, double& e, double &En, double& del_E) ;
    
};


#endif /* CEPlanet_h */

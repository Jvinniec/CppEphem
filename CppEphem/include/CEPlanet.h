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

/////////////////////////////////////////////
/// Date enum
enum CEPlanetAlgo {SOFA,              ///< Use methods included in sofa software
                   JPL                ///< Use Keplerian algorithm outlined by JPL
                   } ;


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
    /// @return Albedo.
    double Albedo() {return albedo_ ;}
    
    /****************************
     * Atribute setters
     ****************************/
    /// @param[in] new_radius New radius (meters)
    void SetMeanRadius_m(double new_radius) {radius_m_ = new_radius ;}
    /// @param[in] new_mass New mass (kilograms)
    void SetMass_kg(double new_mass) {mass_kg_ = new_mass ;}
    /// @param[in] new_albedo New Albedo
    void SetAlbedo(double new_albedo) {albedo_ = new_albedo ;}
    
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
    
    // Methods for updating the coordinates
    virtual void UpdateCoordinates(double new_date=-1.0e30) ;
    virtual void Update_JPL(double new_date=-1.0e30) ;
    virtual void Update_SOFA(double new_date=-1.0e30) ;
    
    /****************************
     * Methods for getting the current x,y,z coordinates and velocities relative to the ICRS point
     ****************************/
    double GetXICRS() {return x_icrs_ ;}    ///< X distance from solar system barycenter (AU)
    double GetYICRS() {return y_icrs_ ;}    ///< Y distance from solar system barycenter (AU)
    double GetZICRS() {return z_icrs_ ;}    ///< Z distance from solar system barycenter (AU)
    double GetVxICRS() {return vx_icrs_ ;}  ///< X velocity relative to solar system center (AU/day)
    double GetVyICRS() {return vy_icrs_ ;}  ///< Y velocity relative to solar system center (AU/day)
    double GetVzICRS() {return vz_icrs_ ;}  ///< Z velocity relative to solar system center (AU/day)
    
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
    // Set the extra terms for the outer planets
    virtual void SetExtraTerms(double b, double c, double s, double f)
        {
            b_ = b ;
            c_ = c ;
            s_ = s ;
            f_ = f ;
        }
    
    /// Set the tolerance for the eccentric anomoly recursive formula
    ///     @param[in] tol          Tolerance (degrees)
    virtual void SetTolerance(double tol = 1.0e-6) {E_tol = tol ;}
    /// Set the reference object for computing more accurate RA,Dec values
    ///     @param[in] reference    Reference object (i.e. planet where the observer is located
    virtual void SetReference(CEPlanet* reference)
        {
            if (reference_ != nullptr) {
                // redirect and delete
                CEPlanet* tmp = reference_ ;
                reference_ = nullptr ;
                delete tmp ;
            }
            
            reference_ = reference ;
        }
    
    /****************************
     * Some generic planets in our solar system
     ****************************/
    static CEPlanet Mercury() ;
    static CEPlanet Venus() ;
    static CEPlanet Earth() ;
    static CEPlanet EMBarycenter() ;
    static CEPlanet Mars() ;
    static CEPlanet Jupiter() ;
    static CEPlanet Saturn() ;
    static CEPlanet Uranus() ;
    static CEPlanet Neptune() ;
    static CEPlanet Pluto() ;
    
    // Set the algorithm
    void SetAlgorithm(CEPlanetAlgo new_algo)
        {
            algorithm_type_ = new_algo;
            if (reference_ != nullptr) reference_->SetAlgorithm(new_algo) ;
        }
    // Set the sofa planet id (note, only values from 1-8 are acceptable)
    void SetSofaID(double new_id)
        {
            // If the id is outside the acceptable range, then set the algorithm to jpl
            if ((new_id < 1.0)||(new_id > 8.0)) {
                SetAlgorithm(CEPlanetAlgo::JPL) ;
            }
            sofa_planet_id_ = new_id;
        }
    
protected:
    
    // Basic properties
    double radius_m_ ;          ///< Radius (in meters)
    double mass_kg_ ;           ///< Mass (kilograms)
    double albedo_ ;            ///< Albedo (0 -> 1)
    
    // The following is a reference point for the observer
    // This will almost always be the Earth-Moon barycenter
    CEPlanet* reference_ = nullptr ;
    
    // Define the algorithm used to compute the planets position
    CEPlanetAlgo algorithm_type_ = CEPlanetAlgo::SOFA ;
    /// Sofa planet id (note: 3.5 implies the earth-center which uses a different method
    /// than the other planets)
    double sofa_planet_id_ = 0 ;
    
    // The coordinates representing the current position will need to be
    // relative to some date, since planets move. This is the cached date
    double cached_jd_ = 0.0 ;                   ///< Julian date of the current coordinates
    double x_icrs_ = 0.0 ;                      ///< X-Coordinate relative to solar system barycenter
    double y_icrs_ = 0.0 ;                      ///< Y-Coordinate relative to solar system barycenter
    double z_icrs_ = 0.0 ;                      ///< Z-Coordinate relative to solar system barycenter
    // Note, these velocities are only computed for "algorithm_type_=SOFA" at the moment
    double vx_icrs_ = 0.0 ;                     ///< X-velocity relative to solar system center
    double vy_icrs_ = 0.0 ;                     ///< Y-velocity relative to solar system center
    double vz_icrs_ = 0.0 ;                     ///< Z-velocity relative to solar system center
    
    /******************************************
     * Properties for the JPL algorithm
     ******************************************/
    // Orbital properties (element 2 is the derivative)
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
    double f_ = 0.0 ;
    
    /******************************************
     * Methods for the JPL algorithm
     ******************************************/

    /// Compute current value of a given element
    ///     @param[in] value                    Once of the orbital property values
    ///     @param[in] value_derivative_        Time derivative of 'value'
    ///     @param[in] time                     Days since J2000 epoch
    inline double ComputeElement(double value, double value_derivative_, double time)
        {return value + value_derivative_*time;}
    double MeanAnomaly(double mean_longitude_deg,
                       double perihelion_long_deg,
                       double T=0.0) ;
    
    // Recursive formula necessary for the computation of eccentric anomoly
    double EccentricAnomoly(double& M, double& e, double &En, double& del_E) ;
    
};


#endif /* CEPlanet_h */

/***************************************************************************
 *  CEPlanet.h: CppEphem                                                   *
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
    CEPlanet(const std::string&      name, 
             const double&           xcoord, 
             const double&           ycoord,
             const CECoordinateType& coord_type=CECoordinateType::CIRS,
             const CEAngleType&      angle_type=CEAngleType::RADIANS) ;
    CEPlanet(const CEPlanet& other);
    virtual ~CEPlanet() ;
    
    CEPlanet& operator=(const CEPlanet& other);

    /******  Methods  ******/
    double         Radius_m();
    double         Mass_kg();
    double         Albedo();
    void           SetMeanRadius_m(double new_radius);
    void           SetMass_kg(double new_mass);
    void           SetAlbedo(double new_albedo);
    virtual double XCoordinate_Rad(double new_date=-1.0e30) const;
    virtual double XCoordinate_Deg(double new_date=-1.0e30) const;
    virtual double YCoordinate_Rad(double new_date=-1.0e30) const;
    virtual double YCoordinate_Deg(double new_date=-1.0e30) const;
    virtual void   UpdateCoordinates(double new_date=-1.0e30) const;
    virtual void   Update_JPL(double new_date=-1.0e30) const;
    virtual void   Update_SOFA(double new_date=-1.0e30) const;
    
    /****************************
     * Methods for getting the current x,y,z coordinates and velocities relative to the ICRS point
     ****************************/
    double GetXICRS();
    double GetYICRS();
    double GetZICRS();
    std::vector<double> PositionICRS(void) const;
    double GetVxICRS();
    double GetVyICRS();
    double GetVzICRS();
    std::vector<double> VelocityICRS(void) const;
    
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
                                double semi_major_axis_au_per_cent=0.0);
    virtual void SetEccentricity(double eccentricity,
                                double eccentricity_per_cent=0.0);
    virtual void SetInclination(double inclination,
                                double inclination_per_cent=0.0,
                                CEAngleType angle_type=CEAngleType::DEGREES);
    virtual void SetMeanLongitude(double mean_longitude,
                                double mean_longitude_per_cent=0.0,
                                CEAngleType angle_type=CEAngleType::DEGREES);
    virtual void SetPerihelionLongitude(double perihelion_lon,
                                double perihelion_lon_per_cent=0.0,
                                CEAngleType angle_type=CEAngleType::DEGREES);
    virtual void SetAscendingNodeLongitude(double ascending_node_lon,
                                double ascending_node_lon_per_cent=0.0,
                                CEAngleType angle_type=CEAngleType::DEGREES);
    virtual void SetExtraTerms(double b, double c, double s, double f);
    virtual void SetTolerance(double tol = 1.0e-6);
    virtual void SetReference(CEPlanet* reference);
    
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
    CEPlanetAlgo Algorithm(void) const;
    void         SetAlgorithm(const CEPlanetAlgo& new_algo);
    void         SetSofaID(const double& new_id);
    
private:
    
    void copy_members(const CEPlanet& other);
    void init_members(void);
    void free_members(void);

    void UpdatePosition(const double& jd) const;

    /******************************************
     * Methods for the JPL algorithm
     ******************************************/

    inline double ComputeElement(double value, 
                                 double value_derivative_, 
                                 double time) const;
    double        MeanAnomaly(double mean_longitude_deg,
                              double perihelion_long_deg,
                              double T=0.0) const;
    
    // Recursive formula necessary for the computation of eccentric anomoly
    double EccentricAnomoly(double& M, double& e, double &En, double& del_E) const;

    // Basic properties
    double radius_m_ ;          ///< Radius (in meters)
    double mass_kg_ ;           ///< Mass (kilograms)
    double albedo_ ;            ///< Albedo (0 -> 1)
    
    // The following is a reference point for the observer
    // This will almost always be the Earth-Moon barycenter
    CEPlanet* reference_;
    
    // Define the algorithm used to compute the planets position
    CEPlanetAlgo algorithm_type_;
    /// Sofa planet id (note: 3.5 implies the earth-center which uses a different method
    /// than the other planets)
    double sofa_planet_id_;
    
    // The coordinates representing the current position will need to be
    // relative to some date, since planets move. This is the cached date
    mutable double cached_jd_;             ///< Julian date of the current coordinates
    mutable std::vector<double> pos_icrs_; ///< XYZ position (AU) relative to solar system barycenter
    // Note, these velocities are only computed for "algorithm_type_=SOFA" at the moment
    mutable std::vector<double> vel_icrs_; ///< XYZ velocity (AU/day) relative to solar system barycenter
    
    /******************************************
     * Properties for the JPL algorithm
     ******************************************/
    // Orbital properties (element 2 is the derivative)
    double semi_major_axis_au_;          ///< a - Semi major axis in Astronomical Units (AU)
    double eccentricity_;                ///< e - Eccentricity
    double inclination_deg_;             ///< I - inclination in radians
    double mean_longitude_deg_;          ///< L - Mean longitude (radians)
    double perihelion_lon_deg_;          ///< w - Longitude of perihelion (radians)
    double ascending_node_lon_deg_;      ///< Omega - Longitude of ascending node (radians)
    
    // Derivatives of orbital properties
    double semi_major_axis_au_per_cent_;
    double eccentricity_per_cent_;
    double inclination_deg_per_cent_;
    double mean_longitude_deg_per_cent_;
    double perihelion_long_deg_per_cent_;
    double ascending_node_lon_deg_per_cent_;
    
    // The following is the tolerance for the computation of eccentric anomoly
    double E_tol = 1.0e-6 ;
    
    // Extra terms for outer planets (Jupiter, Saturn, Uranus, Neptune, and Pluto)
    double b_;
    double c_;
    double s_;
    double f_;    
};


/**********************************************************************//**
 * @return Algorithm used for computing the planet position
 *************************************************************************/
inline
CEPlanetAlgo CEPlanet::Algorithm(void) const
{
    return algorithm_type_;
}


/**********************************************************************//**
 * @return Radius in meters.
 *************************************************************************/
inline
double CEPlanet::Radius_m() 
{
    return radius_m_;
}


/**********************************************************************//**
 * @return Mass in kilograms.
 *************************************************************************/
inline
double CEPlanet::Mass_kg() 
{
    return mass_kg_;
}


/**********************************************************************//**
 * @return Albedo.
 *************************************************************************/
inline
double CEPlanet::Albedo() 
{
    return albedo_;
}


/**********************************************************************//**
 * @param[in] new_radius New radius (meters)
 *************************************************************************/
inline
void CEPlanet::SetMeanRadius_m(double new_radius)
{
    radius_m_ = new_radius;
}


/**********************************************************************//**
 * @param[in] new_mass New mass (kilograms)
 *************************************************************************/
inline
void CEPlanet::SetMass_kg(double new_mass)
{
    mass_kg_ = new_mass ;
}


/**********************************************************************//**
 * @param[in] new_albedo New Albedo
 *************************************************************************/
inline
void CEPlanet::SetAlbedo(double new_albedo)
{
    albedo_ = new_albedo;
}


/**********************************************************************//**
 *************************************************************************/
inline
double CEPlanet::XCoordinate_Rad(double new_date) const
{
    UpdateCoordinates(new_date);
    return CECoordinates::XCoordinate_Rad();
}


/**********************************************************************//**
 *************************************************************************/
inline
double CEPlanet::XCoordinate_Deg(double new_date) const
{
    return XCoordinate_Rad(new_date)*DR2D;
}


/**********************************************************************//**
 *************************************************************************/
inline
double CEPlanet::YCoordinate_Rad(double new_date) const
{
    UpdateCoordinates(new_date) ;
    return CECoordinates::YCoordinate_Rad();
}


/**********************************************************************//**
 *************************************************************************/
inline
double CEPlanet::YCoordinate_Deg(double new_date) const
{
    return YCoordinate_Rad(new_date)*DR2D;
}


/**********************************************************************//**
 * Return X distance from solar system barycenter (AU)
 *************************************************************************/
inline
double CEPlanet::GetXICRS()
{
    return pos_icrs_[0] ;
}


/**********************************************************************//**
 * Y distance from solar system barycenter (AU)
 *************************************************************************/
inline
double CEPlanet::GetYICRS() 
{
    return pos_icrs_[1] ;
}


/**********************************************************************//**
 * Z distance from solar system barycenter (AU)
 *************************************************************************/
inline
double CEPlanet::GetZICRS() 
{
    return pos_icrs_[2] ;
}


/**********************************************************************//**
 * Z distance from solar system barycenter (AU)
 *************************************************************************/
inline
std::vector<double> CEPlanet::PositionICRS(void) const
{
    return pos_icrs_;
}


/**********************************************************************//**
 * X velocity relative to solar system barycenter (AU/day)
 *************************************************************************/
inline
double CEPlanet::GetVxICRS() 
{
    return vel_icrs_[0];
}


/**********************************************************************//**
 * Y velocity relative to solar system barycenter (AU/day)
 *************************************************************************/
inline
double CEPlanet::GetVyICRS() 
{
    return vel_icrs_[1];
} 


/**********************************************************************//**
 * Z velocity relative to solar system barycenter (AU/day)
 *************************************************************************/
inline
double CEPlanet::GetVzICRS() 
{
    return vel_icrs_[2];
}


/**********************************************************************//**
 * Vector of velocities relative to solar system barycenter (AU/day)
 *************************************************************************/
inline
std::vector<double> CEPlanet::VelocityICRS(void) const
{
    return vel_icrs_;
}

/**********************************************************************//**
 *************************************************************************/
inline
void CEPlanet::SetExtraTerms(double b, double c, double s, double f)
{
    b_ = b ;
    c_ = c ;
    s_ = s ;
    f_ = f ;
}


/**********************************************************************//**
 * Set the tolerance for the eccentric anomoly recursive formula
 * 
 * @param[in] tol          Tolerance (degrees)
 *************************************************************************/
inline
void CEPlanet::SetTolerance(double tol) 
{
    E_tol = tol ;
}


/**********************************************************************//**
 * Set the desired planet computation algorithm
 * 
 * @param[in] new_algo      Algorithm for computing planet spatial parameters
 *************************************************************************/
inline
void CEPlanet::SetAlgorithm(const CEPlanetAlgo& new_algo)
{
    algorithm_type_ = new_algo;
    if (reference_ != nullptr) reference_->SetAlgorithm(new_algo) ;
}


/**********************************************************************//**
 * Compute current value of a given element
 * 
 * @param[in] value                    Once of the orbital property values
 * @param[in] value_derivative_        Time derivative of 'value'
 * @param[in] time                     Days since J2000 epoch
 *************************************************************************/
inline 
double CEPlanet::ComputeElement(double value, double value_derivative_, double time) const
{
    return value + value_derivative_*time;
}

#endif /* CEPlanet_h */

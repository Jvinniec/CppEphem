//
//  CEPlanet.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 1/13/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

/** \class CEPlanet
 The CEPlanet class describes a planetary object.
 */

#include <stdio.h>

#include "CEPlanet.h"

/////////////////////////////////////////////////////
/// Default constructor
CEPlanet::CEPlanet() :
    CEBody()
{}

/////////////////////////////////////////////////////
/// Primary constructor
///     @param[in] name             Some identifying name for this object
///     @param[in] xcoord           X-coordinate
///     @param[in] ycoord           Y-coordinate
///     @param[in] coord_type       Coordinate system for passed coordinates
///     @param[in] angle_type       Angle type for the coordinates passed
CEPlanet::CEPlanet(const std::string& name, double xcoord, double ycoord,
                   CECoordinateType coord_type,
                   CEAngleType angle_type) :
    CEBody(name, xcoord, ycoord, coord_type, angle_type)
{}

/////////////////////////////////////////////////////
/// Primary constructor
///     @param[in] name             Some identifying name for this object
///     @param[in] coordinates      Coordinates of this object
CEPlanet::CEPlanet(const std::string& name,
                   CECoordinates coordinates) :
    CEBody(coordinates, name)
{}

/////////////////////////////////////////////////////
/// Destructor
CEPlanet::~CEPlanet()
{}

# pragma mark - Specific Planetary objects

/////////////////////////////////////////////////////
/// Returns an object representing Mercury.
///     @return CEPlanet object representing Mercury
CEPlanet CEPlanet::Mercury()
{
    CEPlanet mercury("Mercury", 0.0, 0.0) ;
    mercury.SetSemiMajorAxis_AU(0.38709843, 0.0) ;
    mercury.SetEccentricity(0.20563661, 0.00002123) ;
    mercury.SetInclination(7.00559432, -0.00590158, CEAngleType::DEGREES) ;
    mercury.SetMeanLongitude(252.25166724, 149472.67486623, CEAngleType::DEGREES) ;
    mercury.SetPerihelionLongitude(77.45771895, 0.15940013, CEAngleType::DEGREES) ;
    mercury.SetAscendingNodeLongitude(48.33961819, -0.12214182, CEAngleType::DEGREES) ;
    
    return mercury ;
}

/////////////////////////////////////////////////////
/// Returns an object representing Mars.
///     @return CEPlanet object representing Mars
CEPlanet CEPlanet::Mars()
{
    CEPlanet mars("Mars", 0.0, 0.0) ;
    mars.SetSemiMajorAxis_AU(1.52371243, 0.00000097) ;
    mars.SetEccentricity(0.09336511, 0.00009149) ;
    mars.SetInclination(1.85181869, -0.00724757, CEAngleType::DEGREES) ;
    mars.SetMeanLongitude(-4.56813164, 19140.29934243, CEAngleType::DEGREES) ;
    mars.SetPerihelionLongitude(-23.91744784, 0.45223625, CEAngleType::DEGREES) ;
    mars.SetAscendingNodeLongitude(49.71320984, -0.26852431, CEAngleType::DEGREES) ;
    
    return mars ;
}

# pragma mark - Public Methods

/////////////////////////////////////////////////////
/// Set the semi-major axis (in AU) and it's derivative
///     @param[in] semi_major_axis_au           Semi-major axis (units of AU)
///     @param[in] semi_major_axis_au_per_cent  Time derivative of semi-major axis (AU/century)
void CEPlanet::SetSemiMajorAxis_AU(double semi_major_axis_au,
                                   double semi_major_axis_au_per_cent)
{
    // Make sure the semi_major_axis is positive
    if (semi_major_axis_au <= 0.0) {
        std::cerr << "[ERROR] CEPlanet::SetSemiMajorAxis_AU() : Semi major axis must be positive!\n";
    } else {
        semi_major_axis_au_ = semi_major_axis_au ;
        semi_major_axis_au_per_cent_ = semi_major_axis_au_per_cent ;
    }
}

/////////////////////////////////////////////////////
/// Set the eccentricity
///     @param[in] eccentricity                 Eccentricity (0 -> 1)
///     @param[in] eccentricity_per_cent        Time derivative of eccentricity (/century)
void CEPlanet::SetEccentricity(double eccentricity,
                               double eccentricity_per_cent)
{
    eccentricity_ = eccentricity ;
    eccentricity_per_cent_ = eccentricity_per_cent ;
}

/////////////////////////////////////////////////////
/// Set the inclination angle of the planets orbit
///     @param[in] inclination              Inclination angle (units of 'angle_type')
///     @param[in] inclination_per_cent     Time derivative of the inclination (angle/century)
///     @param[in] angle_type               Angle type for above values (DEGREES or RADIANS)
void CEPlanet::SetInclination(double inclination,
                              double inclination_per_cent,
                              CEAngleType angle_type)
{
    if (angle_type == CEAngleType::RADIANS) {
        inclination *= DR2D ;
        inclination_per_cent *= DR2D ;
    }
    
    inclination_deg_ = inclination ;
    inclination_deg_per_cent_ = inclination_per_cent * DR2D ;
}

/////////////////////////////////////////////////////
/// Set the mean longitude
///     @param[in] mean_longitude           Mean longitude (units of 'angle_type')
///     @param[in] mean_longitude_per_cent  Time Derivative of mean longitude (/century)
///     @param[in] angle_type               Angle type for provided values (DEGREES or RADIANS)
void CEPlanet::SetMeanLongitude(double mean_longitude,
                                double mean_longitude_per_cent,
                                CEAngleType angle_type)
{
    if (angle_type == CEAngleType::RADIANS) {
        mean_longitude *= DR2D ;
        mean_longitude_per_cent *= DR2D ;
    }
    
    mean_longitude_deg_ = mean_longitude ;
    mean_longitude_deg_per_cent_ = mean_longitude_per_cent ;
}

/////////////////////////////////////////////////////
/// Set the longitude at perihelion
///     @param[in] perihelion_lon           Longitude at perihelion
///     @param[in] perihelion_lon_per_cent  Time derivative of longitude at perihelion (/century)
///     @param[in] angle_type               Angle type for provided values (DEGREES or RADIANS)
void CEPlanet::SetPerihelionLongitude(double perihelion_lon,
                                      double perihelion_lon_per_cent,
                                      CEAngleType angle_type)
{
    if (angle_type == CEAngleType::RADIANS) {
        perihelion_lon *= DR2D ;
        perihelion_lon_per_cent *= DR2D ;
    }
    
    perihelion_lon_deg_ = perihelion_lon ;
    perihelion_long_deg_per_cent_ = perihelion_lon_per_cent ;
}

/////////////////////////////////////////////////////
/// Set the longitude of the ascending node
///     @param[in] ascending_node_lon           Longitude of the ascending node
///     @param[in] ascending_node_lon_per_cent  Time derivative of longitude of the ascending node (/century)
///     @param[in] angle_type                   Angle type for provided values (DEGREES or RADIANS)
void CEPlanet::SetAscendingNodeLongitude(double ascending_node_lon,
                                         double ascending_node_lon_per_cent,
                                         CEAngleType angle_type)
{
    if (angle_type == CEAngleType::RADIANS) {
        ascending_node_lon *= DR2D ;
        ascending_node_lon_per_cent *= DR2D ;
    }
    
    ascending_node_lon_deg_ = ascending_node_lon ;
    ascending_node_lon_deg_per_cent_ = ascending_node_lon_per_cent ;
}

/////////////////////////////////////////////////////
/// Recomputes the coordinates of the planet if the date
/// has changed from the last time the coordinates were
/// computed.
///     @param[in] new_jd       Julian date
void CEPlanet::UpdateCoordinates(double new_jd)
{
    // If no date was supplied, or if the date hasnt changed, do nothing
    if ((new_jd < -1.0e29) || (new_jd == cached_jd_)) return ;
    
    /* Date has changed, so we need to recompute the coordinates of this object */
    
    // Compute the number of centuries since J2000 epoch
    double T = (new_jd-CppEphem::julian_date_J2000()) / 36525.0 ;
    
    // Compute the values
    double a = ComputeElement(semi_major_axis_au_, semi_major_axis_au_per_cent_, T) ;
    double e = ComputeElement(eccentricity_, eccentricity_per_cent_, T) ;
    double I = ComputeElement(inclination_deg_, inclination_deg_per_cent_, T) ;
    double L = ComputeElement(mean_longitude_deg_, mean_longitude_deg_per_cent_, T) ;
    double w = ComputeElement(perihelion_lon_deg_, perihelion_long_deg_per_cent_, T) ;
    double O = ComputeElement(ascending_node_lon_deg_, ascending_node_lon_deg_per_cent_, T) ;
    
    // Compute the argument of perihelion (W) and mean anomoly (M)
    double W = w - O ;
    double M = MeanAnomaly(L, w, T) ;
    
    // Iterate on the eccentric anomoly
    double E = M + e*DR2D * std::sin(M*DD2R) ;
    double del_E(0.0) ;
    E = EccentricAnomoly(M, e, E, del_E) ;
    
    // Compute the planets heliocentric coordinates in it's orbital plane
    double x_prime = a * (std::cos(E*DD2R) - e) ;
    double y_prime = a * std::sqrt(1.0 - (e*e)) * std::sin(E*DD2R) ;
//    double z_prime = 0.0 ;  // by definition
    
    // Transform the coordinates into the coordinates of the ecliptic
    double x_ecl = x_prime * (std::cos(W*DD2R)*std::cos(O*DD2R) - std::sin(W*DD2R)*std::sin(O*DD2R)*std::cos(I*DD2R)) +
                   y_prime * (-std::sin(W*DD2R)*std::cos(O*DD2R) - std::cos(W*DD2R)*std::sin(O*DD2R)*std::cos(I*DD2R)) ;
    double y_ecl = x_prime * (std::cos(W*DD2R)*std::sin(O*DD2R) + std::sin(W*DD2R)*std::cos(O*DD2R)*std::cos(I*DD2R)) +
                   y_prime * (-std::sin(W*DD2R)*std::sin(O*DD2R) + std::cos(W*DD2R)*std::cos(O*DD2R)*std::cos(I*DD2R)) ;
    double z_ecl = x_prime * (std::sin(W*DD2R)*std::sin(I*DD2R)) + y_prime * (std::cos(W*DD2R)*std::sin(I*DD2R)) ;
    
    // Convert from the ecliptic coordinates to ICRS
    double obl = DD2R * 23.43928 ;
    x_icrs_ = x_ecl ;
    y_icrs_ = y_ecl * std::cos(obl) - z_ecl * std::sin(obl) ;
    z_icrs_ = y_ecl * std::sin(obl) + z_ecl * std::cos(obl) ;
    
    // Now compute the actual coordiantes in ICRS
    xcoord_ = std::atan2(y_icrs_, x_icrs_) ;
    ycoord_ = M_PI_2 - std::acos(z_icrs_ / std::sqrt(x_icrs_*x_icrs_ + y_icrs_*y_icrs_ + z_icrs_*z_icrs_)) ;
    
    // Make sure the x-coordinate is in the appropriate range
    while (xcoord_ > M_PI*2.0) xcoord_ -= M_PI*2.0 ;
    while (xcoord_ < 0.0) xcoord_ += M_PI*2.0 ;
    
    // Now that the coordinates are updated, reset the time
    cached_jd_ = new_jd ;
}

# pragma mark - Protected Methods

/////////////////////////////////////////////////////
/// Recursive method for computing the eccentric anomoly
///     @param[in] M            Mean anomoly
///     @param[in] e            Computed eccentricity for given date
///     @param[in] En           N-th computed eccentric anomoly
///     @param[in] del_E        Just used to prevent the need to allocate memory each loop
double CEPlanet::EccentricAnomoly(double& M,
                                  double& e,
                                  double& En,
                                  double& del_E)
{
    // Note that because this method is called recursively, no
    // variables should be created inside of this method
    
    // compute del_E
    del_E = (M - (En - e*DR2D*std::sin(En*DD2R))) / (1.0-e*std::cos(En*DD2R)) ;

    // If the change is less than the tolerance, we're all good!
    if ( std::fabs(del_E) < E_tol) {
        return En ;
    }
    // If not, then do another iteration
    else {
        En += del_E ;
        return EccentricAnomoly(M, e, En, del_E) ;
    }
}

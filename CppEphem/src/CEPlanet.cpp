//
//  CEPlanet.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 1/13/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

/** \class CEPlanet
 The CEPlanet class describes a planetary object. Static methods exist
 for obtaining specific planetary descriptions of the major solar system
 bodies: Mercury, Venus, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto.
 Accuracy in the positions of these objects should be assumed as:

 - Inner planets (Mercury - Mars): < 0.5 degrees
 - Outer planets (Jupiter - Pluto): < 1-2 degrees
 
 Note that these numbers have not been verified at the moment.
 Planet positions are computed using the JPL Keplerian formulas and
 their best fit orbital parameters.
 
 The positions are computed first for the planet in order to obtain its
 ICRS reference frame x,y,z coordinates. Then, the ICRS x,y,z coordinates
 of the Earth-Moon barycenter are computed. The planets x,y,z coordinates
 are then projected relative to the E-M barycenter coordinates to obtain
 the apparent RA,Dec coordinates from the Earth.
 
 Future improvements will include correcting the coordinates for a given
 observer's location on the Earth relative to the E-M barycenter.
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
{
    // Delete the reference object if we no longer need it
    if (reference_ != nullptr) delete reference_ ;
}

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
    
    // Set the reference object as the Earth-Moon barycenter
    mercury.SetReference(new CEPlanet(CEPlanet::EMBarycenter())) ;
    
    // Set planet characteristics
    mercury.SetMeanRadius_m(2440000.0) ;
    mercury.SetAlbedo(0.106) ;
    mercury.SetMass_kg(3.302E23) ;
    
    return mercury ;
}

/////////////////////////////////////////////////////
/// Returns an object representing Venus.
///     @return CEPlanet object representing Venus
CEPlanet CEPlanet::Venus()
{
    CEPlanet venus("Venus", 0.0, 0.0) ;
    venus.SetSemiMajorAxis_AU(0.72332102, -0.00000026) ;
    venus.SetEccentricity(0.00676399, -0.00005107) ;
    venus.SetInclination(3.39777545, 0.00043494, CEAngleType::DEGREES) ;
    venus.SetMeanLongitude(181.97970850, 58517.81560260, CEAngleType::DEGREES) ;
    venus.SetPerihelionLongitude(131.76755713, 0.05679648, CEAngleType::DEGREES) ;
    venus.SetAscendingNodeLongitude(76.67261496, -0.27274174, CEAngleType::DEGREES) ;
    
    // Set the reference object as the Earth-Moon barycenter
    venus.SetReference(new CEPlanet(CEPlanet::EMBarycenter())) ;
    
    venus.SetMeanRadius_m(6051800.0) ;
    venus.SetAlbedo(0.65) ;
    venus.SetMass_kg(48.685E23) ;
    
    return venus ;
}

/////////////////////////////////////////////////////
/// Returns an object representing the Earth-Moon barycenter.
///     @return CEPlanet object representing the Earth-Moon barycenter
CEPlanet CEPlanet::EMBarycenter()
{
    CEPlanet em_barycenter("EMBaryCenter", 0.0, 0.0) ;
    em_barycenter.SetSemiMajorAxis_AU(1.00000018, -0.00000003) ;
    em_barycenter.SetEccentricity(0.01673163, -0.00003661) ;
    em_barycenter.SetInclination(-0.00054346, -0.01337178, CEAngleType::DEGREES) ;
    em_barycenter.SetMeanLongitude(100.46691572, 35999.37306329, CEAngleType::DEGREES) ;
    em_barycenter.SetPerihelionLongitude(102.93005885, 0.31795260, CEAngleType::DEGREES) ;
    em_barycenter.SetAscendingNodeLongitude(-5.11260389, -0.24123856, CEAngleType::DEGREES) ;

    // Note that we dont need to set the reference object for this one
    
    return em_barycenter ;
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
    
    // Set the reference object as the Earth-Moon barycenter
    mars.SetReference(new CEPlanet(CEPlanet::EMBarycenter())) ;
    
    mars.SetMeanRadius_m(3389900.0) ;
    mars.SetAlbedo(0.150) ;
    mars.SetMass_kg(6.4185E23) ;
    
    return mars ;
}

/////////////////////////////////////////////////////
/// Returns an object representing Jupiter.
///     @return CEPlanet object representing Jupiter
CEPlanet CEPlanet::Jupiter()
{
    CEPlanet jupiter("Jupiter", 0.0, 0.0) ;
    jupiter.SetSemiMajorAxis_AU(5.20248019, -0.00002864) ;
    jupiter.SetEccentricity(0.04853590, -0.00005107) ;
    jupiter.SetInclination(1.29861416, -0.00322699, CEAngleType::DEGREES) ;
    jupiter.SetMeanLongitude(34.33479152, 3034.90371757, CEAngleType::DEGREES) ;
    jupiter.SetPerihelionLongitude(14.27495244, 0.18199196, CEAngleType::DEGREES) ;
    jupiter.SetAscendingNodeLongitude(100.29282654, 0.13024619, CEAngleType::DEGREES) ;
    
    jupiter.SetExtraTerms(-0.00012452, 0.06064060, -0.35635438, 38.35125000) ;
    
    // Set the reference object as the Earth-Moon barycenter
    jupiter.SetReference(new CEPlanet(CEPlanet::EMBarycenter())) ;
    
    jupiter.SetMeanRadius_m(69911000.0) ;
    jupiter.SetAlbedo(0.52) ;
    jupiter.SetMass_kg(1.89813E27) ;
    
    return jupiter ;
}

/////////////////////////////////////////////////////
/// Returns an object representing Saturn.
///     @return CEPlanet object representing Saturn
CEPlanet CEPlanet::Saturn()
{
    CEPlanet saturn("Saturn", 0.0, 0.0) ;
    saturn.SetSemiMajorAxis_AU(9.54149883, -0.00003065) ;
    saturn.SetEccentricity(0.05550825, -0.00032044) ;
    saturn.SetInclination(2.49424102, 0.00451969, CEAngleType::DEGREES) ;
    saturn.SetMeanLongitude(50.07571329, 1222.11494724, CEAngleType::DEGREES) ;
    saturn.SetPerihelionLongitude(92.86136063, 0.54179478, CEAngleType::DEGREES) ;
    saturn.SetAscendingNodeLongitude(113.63998702, -0.25015002, CEAngleType::DEGREES) ;
    
    saturn.SetExtraTerms(0.00025899, -0.13434469, 0.87320147, 38.35125000) ;
    
    // Set the reference object as the Earth-Moon barycenter
    saturn.SetReference(new CEPlanet(CEPlanet::EMBarycenter())) ;
    
    saturn.SetMeanRadius_m(58232000.0) ;
    saturn.SetAlbedo(0.47) ;
    saturn.SetMass_kg(5.68319E26) ;
    
    return saturn ;
}

/////////////////////////////////////////////////////
/// Returns an object representing Uranus.
///     @return CEPlanet object representing Uranus
CEPlanet CEPlanet::Uranus()
{
    CEPlanet uranus("Uranus", 0.0, 0.0) ;
    uranus.SetSemiMajorAxis_AU(19.18797948, -0.00020455) ;
    uranus.SetEccentricity(0.04685740, -0.00001550) ;
    uranus.SetInclination(0.77298127, -0.00180155, CEAngleType::DEGREES) ;
    uranus.SetMeanLongitude(314.20276625, 428.49512595, CEAngleType::DEGREES) ;
    uranus.SetPerihelionLongitude(172.43404441, 0.09266985, CEAngleType::DEGREES) ;
    uranus.SetAscendingNodeLongitude(73.96250215, 0.05739699, CEAngleType::DEGREES) ;
    
    uranus.SetExtraTerms(0.00058331, -0.97731848, 0.17689245, 7.67025000) ;
    
    // Set the reference object as the Earth-Moon barycenter
    uranus.SetReference(new CEPlanet(CEPlanet::EMBarycenter())) ;
    
    uranus.SetMeanRadius_m(25362000.0) ;
    uranus.SetAlbedo(0.51) ;
    uranus.SetMass_kg(8.68103E25) ;
    
    return uranus ;
}

/////////////////////////////////////////////////////
/// Returns an object representing Neptune.
///     @return CEPlanet object representing Neptune
CEPlanet CEPlanet::Neptune()
{
    CEPlanet neptune("Neptune", 0.0, 0.0) ;
    neptune.SetSemiMajorAxis_AU(30.06952752, 0.00006447) ;
    neptune.SetEccentricity(0.00895439, 0.00000818) ;
    neptune.SetInclination(1.77005520, 0.00022400, CEAngleType::DEGREES) ;
    neptune.SetMeanLongitude(304.22289287, 218.46515314, CEAngleType::DEGREES) ;
    neptune.SetPerihelionLongitude(46.68158724, 0.01009938, CEAngleType::DEGREES) ;
    neptune.SetAscendingNodeLongitude(131.78635853, -0.00606302, CEAngleType::DEGREES) ;
    
    neptune.SetExtraTerms(-0.00041348, 0.68346318, -0.10162547, 7.67025000) ;
    
    // Set the reference object as the Earth-Moon barycenter
    neptune.SetReference(new CEPlanet(CEPlanet::EMBarycenter())) ;
    
    neptune.SetMeanRadius_m(24624000.0) ;
    neptune.SetAlbedo(0.41) ;
    neptune.SetMass_kg(1.0241E26) ;
    
    return neptune ;
}

/////////////////////////////////////////////////////
/// Returns an object representing Pluto.
///     @return CEPlanet object representing Pluto
CEPlanet CEPlanet::Pluto()
{
    CEPlanet pluto("Pluto", 0.0, 0.0) ;
    pluto.SetSemiMajorAxis_AU(39.48686035, 0.00449751) ;
    pluto.SetEccentricity(0.24885238, 0.00006016) ;
    pluto.SetInclination(17.14104260, 0.00000501, CEAngleType::DEGREES) ;
    pluto.SetMeanLongitude(238.96535011, 145.18042903, CEAngleType::DEGREES) ;
    pluto.SetPerihelionLongitude(224.09702598, -0.00968827, CEAngleType::DEGREES) ;
    pluto.SetAscendingNodeLongitude(110.30167986, -0.00809981, CEAngleType::DEGREES) ;
    
    pluto.SetExtraTerms(-0.01262724, 0.0, 0.0, 0.0) ;
    
    // Set the reference object as the Earth-Moon barycenter
    pluto.SetReference(new CEPlanet(CEPlanet::EMBarycenter())) ;
    
    pluto.SetMeanRadius_m(1195000.0) ;
    pluto.SetAlbedo(0.3) ;
    pluto.SetMass_kg(1.307E22) ;
    
    return pluto ;
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
    double T = (new_jd-CppEphem::julian_date_J2000()) / 36525.6898326 ;
    
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
    
    double x_eq(x_icrs_), y_eq(y_icrs_), z_eq(z_icrs_) ;
    // If there is a reference object, then compute more accurate RA,DEC
    if (reference_ != nullptr) {
        reference_->UpdateCoordinates(new_jd) ;
        x_eq = x_icrs_ - reference_->GetXICRS() ;
        y_eq = y_icrs_ - reference_->GetYICRS() ;
        z_eq = z_icrs_ - reference_->GetZICRS() ;
    }
    
    // Now compute the actual coordiantes in ICRS
    xcoord_ = std::atan2(y_eq, x_eq) ;
    ycoord_ = M_PI_2 - std::acos(z_eq / std::sqrt(x_eq*x_eq + y_eq*y_eq + z_eq*z_eq)) ;
    
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

/***************************************************************************
 *  CEPlanet.cpp: CppEphem                                                 *
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

/** \class CEPlanet
 The CEPlanet class describes a planetary object. Static methods exist
 for obtaining specific planetary descriptions of the major solar system
 bodies: Mercury, Venus, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto.
 
 Two algorithms exist for computing the apparent RA,Dec and observed 
 positions of these bodies: SOFA and JPL
 
 <h2>SOFA (default for Mercury - Neptune)</h2>
 Accuracy in the positions derived from the SOFA algorithms should be 
 assumed as:
 
 - Inner planets (Mercury - Mars): < few arcsecs
 - Outer planets (Jupiter - Neptune): < few arcmins
 
 Because of the obviously better accuracy in these parameters as compared
 to the JPL algorithm, the SOFA methods are the default. However, it should
 be noted that there is no algorithm for Pluto in the SOFA method, so for
 Pluto the JPL algorithm should is used instead.
 
 <h2>JPL (default for pluto)</h2>
 Accuracy in the positions of these objects should be assumed as:

 - Inner planets (Mercury - Mars): < 0.5 degrees
 - Outer planets (Jupiter - Pluto): < 1-2 degrees
 
 Note that these numbers have not been verified at the moment.
 Planet positions are computed using the JPL Keplerian formulas and
 their best fit orbital parameters. These are self-implemented algorithms
 so any inaccuracy is on the fault of the programmer (JCardenzana) not JPL.
 
 Additionally, Pluto only has coordinate computation algorithms for
 implemented for the JPL algorithm, so this algorithm will always be used
 for computing the positions of Pluto.
 
 <h2>Default Planet Objects</h2>
 The eight planets and Pluto have predefined static methods. To create one of
 these predefined planet objects, it is advised to use these methods:
 
 \code{.cpp}
    // Define one of the planets:
    CEPlanet mercury = CEPlanet::Mercury();
    CEPlanet venus   = CEPlanet::Venus();
    CEPlanet mars    = CEPlanet::Mars();
    CEPlanet jupiter = CEPlanet::Jupiter();
    CEPlanet saturn  = CEPlanet::Saturn();
    CEPlanet uranus  = CEPlanet::Uranus();
    CEPlanet neptune = CEPlanet::Neptune();
    CEPlanet pluto   = CEPlanet::Pluto();
 \endcode
 
 <h2>Converting To Observed Coordinates</h2>
 In order to get the observed coordinates of a planet, it is a simple matter
 of combining three objects: Observer, date, and planet. This can be done in
 the following way:
 
 \code{.cpp}
 
    // == CREATE THE INPUT OBJECTS == //

    // Create a date object for the current time
    CEDate date;
 
    // Create a default observer, longitude and latitude in degrees. 
    // I'm using Ames, IA, USA.
    CEObserver observer(-93.62, 42.0347, 287.0, CEAngleType::DEGREES) ;
 
    // Create a planet object representing Mercury
    CEPlanet mercury = CEPlanet::Mercury() ;
 
    // Now assemble the observer and the 
    // planet into an observation object
    CEObservation observed_coords(&observer, &mercury, &date) ;
 
    // == ACCESS THE OBSERVED PLANET POSITION == //
    
    // Now we can get the current positions of mercury
    double azimuth(0.0), zenith(0.0);
    observed_coords.GetAzimuthZenith_Deg(&azimuth, &zenith) ;
 
    // At this point 'azimuth' and 'zenith' hold the azimuth
    // and zenith angle (in degrees) of mercury for 'observer'
    // at the date/time represented by 'date'.
 
 \endcode

 To get an updated position of Mercury at some other date, we simply update
 the value of 'date' and call again 'CEObservation::GetAzimuthZenith_Deg()'
 
 \code{.cpp}
    
    // Change the date to June 5, 2018 at 1pm UTC
    date.SetDate( 20180605 + (13.0/24.0), CEDateType::GREGORIAN );
 
    // Get the updated positions
    observed_coords.GetAzimuthZenith_Deg(&azimuth, &zenith) ;
 
 \endcode
 
 The following chain of computations occurs behind the scenes (i.e. you dont
 have to know how it's done in order to get the coordinates out).
 
 The positions are computed first for the planet in order to obtain its
 heliocentric (ICRS in JPL algorithm) reference frame x,y,z coordinates. 
 Then, the x,y,z coordinates of the Earth (Earth-Moon barycenter in JPL)
 are computed. The planets x,y,z coordinates are then projected relative to 
 the Earth centric (E-M barycenter in JPL algorithm) coordinates to obtain 
 the apparent RA,Dec coordinates from the Earth.
 
 Future improvements will include correcting the coordinates for a given
 observer's location on the Earth relative to the E-M barycenter.
 */

#include <stdio.h>

#include "CEPlanet.h"

/**********************************************************************//**
 * Default constructor
 *************************************************************************/
CEPlanet::CEPlanet() :
    CEBody()
{
    init_members();
}

/**********************************************************************//**
 * Primary constructor
 * 
 * @param[in] name             Some identifying name for this object
 * @param[in] xcoord           X-coordinate
 * @param[in] ycoord           Y-coordinate
 * @param[in] coord_type       Coordinate system for passed coordinates
 * @param[in] angle_type       Angle type for the coordinates passed
 *************************************************************************/
CEPlanet::CEPlanet(const std::string&      name, 
                   const double&           xcoord, 
                   const double&           ycoord,
                   const CECoordinateType& coord_type,
                   const CEAngleType&      angle_type) :
    CEBody(name, xcoord, ycoord, coord_type, angle_type)
{
    init_members();
}


/**********************************************************************//**
 * Copy constructor
 * 
 * @param[in] other            CEPlanet object to copy from
 *************************************************************************/
CEPlanet::CEPlanet(const CEPlanet& other) :
    CEBody(other)
{
    init_members();
    copy_members(other);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
CEPlanet::~CEPlanet()
{
    free_members();
}


/**********************************************************************//**
 * Copy assignment operator
 * 
 * @param[in] other             CEPlanet object to be copied
 * @return Reference to this object post-copy
 *************************************************************************/
CEPlanet& CEPlanet::operator=(const CEPlanet& other)
{
    if (this != &other) {
        // Copy parent class members
        this->CEBody::operator=(other);

        free_members();
        init_members();
        copy_members(other);
    }
    return *this;
}


/**********************************************************************//**
 * Returns an object representing Mercury.
 *     @return CEPlanet object representing Mercury
 *************************************************************************/
CEPlanet CEPlanet::Mercury()
{
    CEPlanet mercury("Mercury", 0.0, 0.0) ;
    mercury.SetSemiMajorAxis_AU(0.38709843, 0.0) ;
    mercury.SetEccentricity(0.20563661, 0.00002123) ;
    mercury.SetInclination(7.00559432, -0.00590158, CEAngleType::DEGREES) ;
    mercury.SetMeanLongitude(252.25166724, 149472.67486623, CEAngleType::DEGREES) ;
    mercury.SetPerihelionLongitude(77.45771895, 0.15940013, CEAngleType::DEGREES) ;
    mercury.SetAscendingNodeLongitude(48.33961819, -0.12214182, CEAngleType::DEGREES) ;
    
    // Set planet characteristics
    mercury.SetMeanRadius_m(2440000.0) ;
    mercury.SetAlbedo(0.106) ;
    mercury.SetMass_kg(3.302E23) ;
    
    // Set the sofa planet ID
    mercury.SetSofaID(1) ;
    
    return mercury ;
}

/**********************************************************************//**
 * Returns an object representing Venus.
 *     @return CEPlanet object representing Venus
 *************************************************************************/
CEPlanet CEPlanet::Venus()
{
    CEPlanet venus("Venus", 0.0, 0.0) ;
    venus.SetSemiMajorAxis_AU(0.72332102, -0.00000026) ;
    venus.SetEccentricity(0.00676399, -0.00005107) ;
    venus.SetInclination(3.39777545, 0.00043494, CEAngleType::DEGREES) ;
    venus.SetMeanLongitude(181.97970850, 58517.81560260, CEAngleType::DEGREES) ;
    venus.SetPerihelionLongitude(131.76755713, 0.05679648, CEAngleType::DEGREES) ;
    venus.SetAscendingNodeLongitude(76.67261496, -0.27274174, CEAngleType::DEGREES) ;
    
    // Set the planetary properties
    venus.SetMeanRadius_m(6051800.0) ;
    venus.SetAlbedo(0.65) ;
    venus.SetMass_kg(48.685E23) ;
    
    // Set the sofa planet ID
    venus.SetSofaID(2) ;
    
    return venus ;
}

/**********************************************************************//**
 * Returns an object representing Earth.
 * @return CEPlanet object representing Earth
 *************************************************************************/
CEPlanet CEPlanet::Earth()
{
    CEPlanet earth("Earth", 0.0, 0.0) ;
    earth.SetSemiMajorAxis_AU(1.00000018, -0.00000003) ;
    earth.SetEccentricity(0.01673163, -0.00003661) ;
    earth.SetInclination(-0.00054346, -0.01337178, CEAngleType::DEGREES) ;
    earth.SetMeanLongitude(100.46691572, 35999.37306329, CEAngleType::DEGREES) ;
    earth.SetPerihelionLongitude(102.93005885, 0.31795260, CEAngleType::DEGREES) ;
    earth.SetAscendingNodeLongitude(-5.11260389, -0.24123856, CEAngleType::DEGREES) ;
    
    // Set planetary properties
    earth.SetMeanRadius_m(0.0) ;
    earth.SetAlbedo(0.0) ;
    earth.SetMass_kg(0.0) ;
    
    // Set the sofa planet ID
    earth.SetSofaID(3.5) ;
    
    return earth ;
}

/**********************************************************************//**
 * Returns an object representing the Earth-Moon barycenter.
 * 
 * @return CEPlanet object representing the Earth-Moon barycenter
 *************************************************************************/
CEPlanet CEPlanet::EMBarycenter()
{
    CEPlanet em_barycenter("EMBaryCenter", 0.0, 0.0) ;
    em_barycenter.SetSemiMajorAxis_AU(1.00000018, -0.00000003) ;
    em_barycenter.SetEccentricity(0.01673163, -0.00003661) ;
    em_barycenter.SetInclination(-0.00054346, -0.01337178, CEAngleType::DEGREES) ;
    em_barycenter.SetMeanLongitude(100.46691572, 35999.37306329, CEAngleType::DEGREES) ;
    em_barycenter.SetPerihelionLongitude(102.93005885, 0.31795260, CEAngleType::DEGREES) ;
    em_barycenter.SetAscendingNodeLongitude(-5.11260389, -0.24123856, CEAngleType::DEGREES) ;

    // Set the sofa planet ID
    em_barycenter.SetSofaID(3) ;
    
    return em_barycenter ;
}

/**********************************************************************//**
 * Returns an object representing Mars.
 * 
 * @return CEPlanet object representing Mars
 *************************************************************************/
CEPlanet CEPlanet::Mars()
{
    CEPlanet mars("Mars", 0.0, 0.0) ;
    mars.SetSemiMajorAxis_AU(1.52371243, 0.00000097) ;
    mars.SetEccentricity(0.09336511, 0.00009149) ;
    mars.SetInclination(1.85181869, -0.00724757, CEAngleType::DEGREES) ;
    mars.SetMeanLongitude(-4.56813164, 19140.29934243, CEAngleType::DEGREES) ;
    mars.SetPerihelionLongitude(-23.91744784, 0.45223625, CEAngleType::DEGREES) ;
    mars.SetAscendingNodeLongitude(49.71320984, -0.26852431, CEAngleType::DEGREES) ;
    
    // Set planetary properties
    mars.SetMeanRadius_m(3389900.0) ;
    mars.SetAlbedo(0.150) ;
    mars.SetMass_kg(6.4185E23) ;
    
    // Set the sofa planet ID
    mars.SetSofaID(4) ;
    
    
    return mars ;
}

/**********************************************************************//**
 * Returns an object representing Jupiter.
 * 
 * @return CEPlanet object representing Jupiter
 *************************************************************************/
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
    
    // Set planetary properties
    jupiter.SetMeanRadius_m(69911000.0) ;
    jupiter.SetAlbedo(0.52) ;
    jupiter.SetMass_kg(1.89813E27) ;
    
    // Set the sofa planet ID
    jupiter.SetSofaID(5) ;
    
    return jupiter ;
}

/**********************************************************************//**
 * Returns an object representing Saturn.
 * 
 * @return CEPlanet object representing Saturn
 *************************************************************************/
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
    
    // Set planetary properties
    saturn.SetMeanRadius_m(58232000.0) ;
    saturn.SetAlbedo(0.47) ;
    saturn.SetMass_kg(5.68319E26) ;
    
    // Set the sofa planet ID
    saturn.SetSofaID(6) ;
    
    return saturn ;
}

/**********************************************************************//**
 * Returns an object representing Uranus.
 * 
 * @return CEPlanet object representing Uranus
 *************************************************************************/
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
    
    // Set planetary properties
    uranus.SetMeanRadius_m(25362000.0) ;
    uranus.SetAlbedo(0.51) ;
    uranus.SetMass_kg(8.68103E25) ;
    
    // Set the sofa planet ID
    uranus.SetSofaID(7) ;
    
    return uranus ;
}

/**********************************************************************//**
 * Returns an object representing Neptune.
 * 
 * @return CEPlanet object representing Neptune
 *************************************************************************/
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
    
    // Set planetary properties
    neptune.SetMeanRadius_m(24624000.0) ;
    neptune.SetAlbedo(0.41) ;
    neptune.SetMass_kg(1.0241E26) ;
    
    // Set the sofa planet ID
    neptune.SetSofaID(8) ;
    
    return neptune ;
}

/**********************************************************************//**
 * Returns an object representing Pluto. Note that Pluto's
 * position cannot be computed via the SOFA algorithms
 * 
 * @return CEPlanet object representing Pluto
 *************************************************************************/
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
    
    // Set planetary properties
    pluto.SetMeanRadius_m(1195000.0) ;
    pluto.SetAlbedo(0.3) ;
    pluto.SetMass_kg(1.307E22) ;
    
    // Set the sofa planet ID
    // Note that Pluto's positions cannot be computed via the SOFA method
    pluto.SetSofaID(9) ;
    
    return pluto ;
}


/**********************************************************************//**
 * Set the semi-major axis (in AU) and it's derivative
 * 
 * @param[in] semi_major_axis_au           Semi-major axis (units of AU)
 * @param[in] semi_major_axis_au_per_cent  Time derivative of semi-major axis (AU/century)
 *************************************************************************/
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

/**********************************************************************//**
 * Set the eccentricity
 * 
 * @param[in] eccentricity                 Eccentricity (0 -> 1)
 * @param[in] eccentricity_per_cent        Time derivative of eccentricity (/century)
 *************************************************************************/
void CEPlanet::SetEccentricity(double eccentricity,
                               double eccentricity_per_cent)
{
    eccentricity_ = eccentricity ;
    eccentricity_per_cent_ = eccentricity_per_cent ;
}

/**********************************************************************//**
 * Set the inclination angle of the planets orbit
 * 
 * @param[in] inclination              Inclination angle (units of 'angle_type')
 * @param[in] inclination_per_cent     Time derivative of the inclination (angle/century)
 * @param[in] angle_type               Angle type for above values (DEGREES or RADIANS)
 *************************************************************************/
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

/**********************************************************************//**
 * Set the mean longitude
 * 
 * @param[in] mean_longitude           Mean longitude (units of 'angle_type')
 * @param[in] mean_longitude_per_cent  Time Derivative of mean longitude (/century)
 * @param[in] angle_type               Angle type for provided values (DEGREES or RADIANS)
 *************************************************************************/
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

/**********************************************************************//**
 * Set the longitude at perihelion
 * 
 * @param[in] perihelion_lon           Longitude at perihelion
 * @param[in] perihelion_lon_per_cent  Time derivative of longitude at perihelion (/century)
 * @param[in] angle_type               Angle type for provided values (DEGREES or RADIANS)
 *************************************************************************/
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

/**********************************************************************//**
 * Set the longitude of the ascending node
 * 
 * @param[in] ascending_node_lon           Longitude of the ascending node
 * @param[in] ascending_node_lon_per_cent  Time derivative of longitude of the ascending node (/century)
 * @param[in] angle_type                   Angle type for provided values (DEGREES or RADIANS)
 *************************************************************************/
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

/**********************************************************************//**
 * Recomputes the coordinates of the planet if the date
 * has changed from the last time the coordinates were
 * computed.
 * 
 * @param[in] new_jd       Julian date
 *************************************************************************/
void CEPlanet::UpdateCoordinates(double new_jd) const
{
    // If no date was supplied, or if the date hasnt changed, do nothing
    if ((new_jd < -1.0e29) || (new_jd == cached_jd_)) return;

    // Update the positions given the current julian date
    UpdatePosition(new_jd);

    // Compute the position of the earth relative to the planet to account
    // for the time delay of the planet
    CEPlanet earth = CEPlanet::Earth();
    earth.SetAlgorithm(Algorithm());
    earth.UpdatePosition(new_jd);

    std::vector<double> pos = pos_icrs_;
    std::vector<double> vel = vel_icrs_;
    double x = pos[0] - earth.GetXICRS();
    double y = pos[1] - earth.GetYICRS();
    double z = pos[2] - earth.GetZICRS();

    // Compute the light travel time from the planet to Earth in days
    double dist_au = std::sqrt(x*x + y*y + z*z);
    double delay_d = dist_au / CppEphem::c_au_per_day();
    
    // Reupdate the position
    UpdatePosition(new_jd-delay_d);

    // Now compute the actual sky coordinates in ICRS
    iauC2s(&pos_icrs_[0], &xcoord_, &ycoord_);
    
    // Make sure the x-coordinate is in the appropriate range
    xcoord_ = iauAnp(xcoord_);

    // Reset the position and velocity
    pos_icrs_ = pos;
    vel_icrs_ = vel;

    // Now that the coordinates are updated, reset the time
    cached_jd_ = new_jd ;
}

/**********************************************************************//**
 * Recomputes the spatial position for a planet based on chosen algorithm
 * 
 * @param[in] jd            Julian date
 *************************************************************************/
void CEPlanet::UpdatePosition(const double& jd) const
{
    // If no date was supplied, or if the date hasnt changed, do nothing
    if ((jd < -1.0e29) || (jd == cached_jd_)) {
        return ;
    } else if (algorithm_type_ == CEPlanetAlgo::JPL) {
        Update_JPL(jd) ;
    } else if (algorithm_type_ == CEPlanetAlgo::SOFA) {
        Update_SOFA(jd) ;
    }
}

/**********************************************************************//**
 * Recomputes the coordinates of the planet based on the date
 * using the methods included in the sofa package (iauPlan94).
 * Note that this method is currently more accurate than the
 * JPL method implementated in this class.
 * 
 * @param[in] new_jd       Julian date
 *************************************************************************/
void CEPlanet::Update_SOFA(double new_jd) const
{
    // Compute the TDB based on supplied jd
    double tdb1(0.0);
    double tdb2(0.0);
    double mjd = new_jd - CEDate::GetMJD2JDFactor();
    CEDate::UTC2TDB(mjd, &tdb1, &tdb2);

    // Compute the Earth relative corrections
    double pvb[2][3];
    double pvhe[2][3];
    int err = iauEpv00(tdb1, tdb2, pvhe, pvb);

    // If this isn't the earth, compute the planet's heliocentric position
    if (sofa_planet_id_ != 3.5) {
        double pvh[2][3];
        err = iauPlan94(tdb1, tdb2, sofa_planet_id_, pvh) ;

        // Compute the corrected factors for heliocentric -> barycentric as
        //      bary_planet = helio_planet + (bary_earth - helio_earth)
        for (int i=0; i<2; i++) {
            for (int j=0; j<3; j++) {
                pvb[i][j] = pvh[i][j] + (pvb[i][j] - pvhe[i][j]);
            }
        }
    }
    
    // If the Error is not zero, compute the values
    if (err == 0) {
        // position
        pos_icrs_ = std::vector<double>(pvb[0], pvb[0]+3);
    
        // velocity
        vel_icrs_ = std::vector<double>(pvb[1], pvb[1]+3);
    } else {
        std::cerr << "[ERROR] failed to compute planet positions for " << Name() << ". ErrCode=" << err << "\n" ;
    }
}

/**********************************************************************//**
 * Recomputes the coordinates of the planet based on the date
 * using the keplerian method outlined by JPL
 * 
 * @param[in] new_jd       Julian date
 *************************************************************************/
void CEPlanet::Update_JPL(double new_jd) const
{
    /* Date has changed, so we need to recompute the coordinates of this object */

    // Compute the number of centuries since J2000 epoch
    double mjd = new_jd - CEDate::GetMJD2JDFactor();
    double tdb1;
    double tdb2;
    CEDate::UTC2TDB(mjd, &tdb1, &tdb2);
    double T = (tdb1+tdb2 - CppEphem::julian_date_J2000()) / DJC;

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
    pos_icrs_[0] = x_ecl ;
    pos_icrs_[1] = y_ecl * std::cos(obl) - z_ecl * std::sin(obl) ;
    pos_icrs_[2] = y_ecl * std::sin(obl) + z_ecl * std::cos(obl) ;
}


/**********************************************************************//**
 * Computes the mean anomaly
 * 
 * @param[in] mean_longitude_deg       Mean longitude of the planet (degrees)
 * @param[in] perihelion_lon_deg       Longitude at perihelion (degrees)
 * @param[in] T                        Ephemeris time
 * @return Mean anomaly
 *************************************************************************/
double CEPlanet::MeanAnomaly(double mean_longitude_deg,
                             double perihelion_long_deg,
                             double T) const
{
    double M = mean_longitude_deg - perihelion_long_deg + (b_*T*T)
                + c_*std::cos(f_*T*DD2R) + s_*std::sin(f_*T*DD2R) ;
    // Scale M to be in the range (-180,180)
    while (M>180.0) M-=360.0 ;
    while (M<-180.0) M+=360.0 ;
    
    return M ;
}

/**********************************************************************//**
 * Recursive method for computing the eccentric anomoly
 * 
 * @param[in] M            Mean anomoly
 * @param[in] e            Computed eccentricity for given date
 * @param[in] En           N-th computed eccentric anomoly
 * @param[in] del_E        Just used to prevent the need to allocate memory each loop
 *************************************************************************/
double CEPlanet::EccentricAnomoly(double& M,
                                  double& e,
                                  double& En,
                                  double& del_E) const
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


/**********************************************************************//**
 * Set the reference object for computing more accurate RA,Dec values
 * 
 * @param[in] reference    Reference object (i.e. planet where the observer is located
 *************************************************************************/
void CEPlanet::SetReference(CEPlanet* reference)
{
    if (reference_ != nullptr) {
        // redirect and delete
        delete reference_;
        reference_ = nullptr;
    }
    
    reference_ = reference;

    // Reset cached date so that coordinates are recomputed
    cached_jd_ = -1.0e30;
}


/**********************************************************************//**
 * Set the sofa planet id (note, only values from 1-8 are acceptable)
 * 
 * @param[in] new_id            New planet ID
 *************************************************************************/
void CEPlanet::SetSofaID(const double& new_id)
{
    // If the id is outside the acceptable range, then set the algorithm to jpl
    if ((new_id < 1.0)||(new_id > 8.0)) {
        SetAlgorithm(CEPlanetAlgo::JPL) ;
    }
    sofa_planet_id_ = new_id;
}


/**********************************************************************//**
 * Copy data members from another CEPlanet object
 * 
 * @param[in] other             CEPlanet object to copy from
 *************************************************************************/
void CEPlanet::copy_members(const CEPlanet& other)
{
    // Basic properties
    radius_m_ = other.radius_m_;
    mass_kg_  = other.mass_kg_;
    albedo_   = other.albedo_;
    
    // The following is a reference point for the observer
    // This will almost always be the Earth-Moon barycenter
    if (other.reference_ != nullptr) {
        if (reference_ != nullptr) {
            delete reference_;
            reference_ = nullptr;
        }
        reference_ = new CEPlanet(*other.reference_);
    }

    // Define the algorithm used to compute the planets position
    algorithm_type_ = other.algorithm_type_;
    /// Sofa planet id (note: 3.5 implies the earth-center which uses a different method
    /// than the other planets)
    sofa_planet_id_ = other.sofa_planet_id_;
    
    // The coordinates representing the current position will need to be
    // relative to some date, since planets move. This is the cached date
    cached_jd_ = other.cached_jd_;
    pos_icrs_  = other.pos_icrs_;
    vel_icrs_  = other.vel_icrs_;
    
    /******************************************
     * Properties for the JPL algorithm
     ******************************************/
    // Orbital properties (element 2 is the derivative)
    semi_major_axis_au_     = other.semi_major_axis_au_;
    eccentricity_           = other.eccentricity_;
    inclination_deg_        = other.inclination_deg_;
    mean_longitude_deg_     = other.mean_longitude_deg_;
    perihelion_lon_deg_     = other.perihelion_lon_deg_;
    ascending_node_lon_deg_ = other.ascending_node_lon_deg_;
    
    // Derivatives of orbital properties
    semi_major_axis_au_per_cent_     = other.semi_major_axis_au_per_cent_;
    eccentricity_per_cent_           = other.eccentricity_per_cent_;
    inclination_deg_per_cent_        = other.inclination_deg_per_cent_;
    mean_longitude_deg_per_cent_     = other.mean_longitude_deg_per_cent_;
    perihelion_long_deg_per_cent_    = other.perihelion_long_deg_per_cent_;
    ascending_node_lon_deg_per_cent_ = other.ascending_node_lon_deg_per_cent_;
    
    // The following is the tolerance for the computation of eccentric anomoly
    E_tol = other.E_tol;
    
    // Extra terms for outer planets (Jupiter, Saturn, Uranus, Neptune, and Pluto)
    b_ = other.b_;
    c_ = other.c_;
    s_ = other.s_;
    f_ = other.f_;
}


/**********************************************************************//**
 * Initialize data members
 *************************************************************************/
void CEPlanet::init_members(void)
{
    // Basic properties
    radius_m_ = 0.0;
    mass_kg_  = 0.0;
    albedo_   = 0.0;
    
    // The following is a reference point for the observer
    // This will almost always be the Earth-Moon barycenter
    reference_ = nullptr;//new CEPlanet(CEPlanet::EMBarycenter());
    
    // Define the algorithm used to compute the planets position
    algorithm_type_ = CEPlanetAlgo::SOFA;
    // Sofa planet id (note: 3.5 implies the earth-center which uses a different method
    // than the other planets)
    sofa_planet_id_ = 0;
    
    // The coordinates representing the current position will need to be
    // relative to some date, since planets move. This is the cached date
    cached_jd_ = -1.0e30;
    pos_icrs_ = std::vector<double>(3, 0.0);
    // Note, these velocities are only computed for "algorithm_type_=SOFA" at the moment
    vel_icrs_ = std::vector<double>(3, 0.0);
    
    /******************************************
     * Properties for the JPL algorithm
     ******************************************/
    // Orbital properties (element 2 is the derivative)
    semi_major_axis_au_     = 0.0;
    eccentricity_           = 0.0;
    inclination_deg_        = 0.0;
    mean_longitude_deg_     = 0.0;
    perihelion_lon_deg_     = 0.0;
    ascending_node_lon_deg_ = 0.0;
    
    // Derivatives of orbital properties
    semi_major_axis_au_per_cent_     = 0.0;
    eccentricity_per_cent_           = 0.0;
    inclination_deg_per_cent_        = 0.0;
    mean_longitude_deg_per_cent_     = 0.0;
    perihelion_long_deg_per_cent_    = 0.0;
    ascending_node_lon_deg_per_cent_ = 0.0;
    
    // The following is the tolerance for the computation of eccentric anomoly
    E_tol = 1.0e-6;
    
    // Extra terms for outer planets (Jupiter, Saturn, Uranus, Neptune, and Pluto)
    b_ = 0.0;
    c_ = 0.0;
    s_ = 0.0;
    f_ = 0.0;
}


/**********************************************************************//**
 * Free allocated memory
 *************************************************************************/
void CEPlanet::free_members(void)
{
    // Delete the reference object if we no longer need it
    if (reference_ != nullptr) {
        delete reference_ ;
        reference_ = nullptr;
    }
}

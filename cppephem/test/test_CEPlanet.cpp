/***************************************************************************
 *  test_CEPlanet.cpp: CppEphem                                            *
 * ----------------------------------------------------------------------- *
 *  Copyright © 2019 JCardenzana                                           *
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

#include "test_CEPlanet.h"
#include "CENamespace.h"


/**********************************************************************//**
 * Default constructor
 *************************************************************************/
test_CEPlanet::test_CEPlanet() :
    CETestSuite()
{
    // Interpolate corrections
    CppEphem::CorrectionsInterp(true);

    // Create the date object
    base_date_ = CEDate(CppEphem::julian_date_J2000(), CEDateType::JD);
    // Interpret the date object as a julian date
    base_date_.SetReturnType(CEDateType::JD);

    // Setup the observer object for observed coordinates
    base_observer_ = CEObserver(0.0, 0.0, 0.0, CEAngleType::DEGREES);
    base_observer_.SetTemperature_C(0.0);
    base_observer_.SetPressure_hPa(0.0);
    base_observer_.SetRelativeHumidity(0.0);
    base_observer_.SetWavelength_um(0.0);
}


/**********************************************************************//**
 * Destructor
 *************************************************************************/
test_CEPlanet::~test_CEPlanet()
{}


/**********************************************************************//**
 * Run tests
 * 
 * @return whether or not all tests succeeded
 *************************************************************************/
bool test_CEPlanet::runtests()
{
    std::cout << "\nTesting CEPlanet:\n";

    // Run each of the tests
    test_construct();
    test_Planets();

    return pass();
}


/**********************************************************************//**
 * Test constructors
 * 
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_construct(void)
{
    CECoordinates test_coord;

    // Default constructor
    CEPlanet test1;
    test_string(test1.Name(), "undefined", __func__, __LINE__);
    test(test1.GetCoordinates() == test_coord, __func__, __LINE__);

    // Default constructors
    CEPlanet test2("DefaultPlanet", 123.45, 67.89, CECoordinateType::ICRS, CEAngleType::DEGREES);
    test_string(test2.Name(), "DefaultPlanet", __func__, __LINE__);
    test_double(test2.XCoordinate_Deg(), 123.45, __func__, __LINE__);
    test_double(test2.YCoordinate_Deg(), 67.89, __func__, __LINE__);
    test_int(int(test2.GetCoordSystem()), int(CECoordinateType::ICRS), __func__, __LINE__);

    // Copy constructor
    test1.SetName("DefaultPlanet");
    test1.SetCoordinates(123.45, 67.89, CECoordinateType::ICRS, CEAngleType::DEGREES);
    CEPlanet test3(test1);
    //test_Planet(test1, test3, __func__, __LINE__);

    return pass();
}


/**********************************************************************//**
 * Test Mercury
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_mercury(void)
{
    // Create the planet object
    CEPlanet mercury = CEPlanet::Mercury();

    // Update the coordinates
    mercury.UpdateCoordinates(base_date_.JD());

    // Run the actual test with values derived from AstroPy
    test_planet(mercury,
                CECoordinates(251.1840266663606371, -25.3023875289787838, 
                              CECoordinateType::ICRS, 
                              CEAngleType::DEGREES),
                {-0.13721236, -0.4032437, -0.20141521},
                {0.02137206, -0.00493223, -0.00485005});

    return pass();
}


/**********************************************************************//**
 * Test Venus
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_venus(void)
{
    // Create the planet object
    CEPlanet venus = CEPlanet::Venus();

    // Update the coordinates
    venus.UpdateCoordinates(base_date_.JD());

    // Run the actual test with values derived from AstroPy
    test_planet(venus,
                CECoordinates(183.8496760951891247, 1.8722067172672485, 
                              CECoordinateType::ICRS, 
                              CEAngleType::DEGREES),
                {-0.72543765, -0.04893718, 0.02371176},
                {0.00080326, -0.01849847, -0.00837267});

    return pass();
}


/**********************************************************************//**
 * Test Earth
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_earth(void)
{
    // Create the planet object
    CEPlanet earth = CEPlanet::Earth();

    // Update the coordinates
    earth.UpdateCoordinates(base_date_.JD());

    // Run the actual test with values derived from AstroPy
    test_planet(earth,
                CECoordinates(101.7655134417398273, 23.0103434895188776, 
                              CECoordinateType::ICRS, 
                              CEAngleType::DEGREES),
                {-0.18428431, 0.88477935, 0.383819},
                {-0.01720221, -0.00290513, -0.00125952});
    
    return pass();
}


/**********************************************************************//**
 * Test Mars
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_mars(void)
{
    // Create the planet object
    CEPlanet mars = CEPlanet::Mars();

    // Update the coordinates
    mars.UpdateCoordinates(base_date_.JD());

    // Run the actual test with values derived from AstroPy
    test_planet(mars,
                CECoordinates(359.9442433037739306, -1.5700885004650793, 
                              CECoordinateType::ICRS, 
                              CEAngleType::DEGREES),
                {1.38356924, -0.0011989, -0.0378561},
                {0.00067763, 0.01380768, 0.00631503});
    
    return pass();
}


/**********************************************************************//**
 * Test Jupiter
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_jupiter(void)
{
    // Create the planet object
    CEPlanet jupiter = CEPlanet::Jupiter();

    // Update the coordinates
    jupiter.UpdateCoordinates(base_date_.JD());

    // Run the actual test with values derived from AstroPy
    test_planet(jupiter,
                CECoordinates(34.3822629405528843, 12.5158780867456674, 
                              CECoordinateType::ICRS, 
                              CEAngleType::DEGREES),
                {3.99442023, 2.7334608, 1.07451899},
                {-0.00455544, 0.00587705, 0.00263009});
    
    return pass();
}


/**********************************************************************//**
 * Test Saturn
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_saturn(void)
{
    // Create the planet object
    CEPlanet saturn = CEPlanet::Saturn();

    // Update the coordinates
    saturn.UpdateCoordinates(base_date_.JD());

    // Run the actual test with values derived from AstroPy
    test_planet(saturn,
                CECoordinates(43.9734819060061355, 14.3451097907081060, 
                              CECoordinateType::ICRS, 
                              CEAngleType::DEGREES),
                {6.39746262, 6.17262105, 2.2735304},
                {-0.00429156, 0.00350834, 0.00163369});
    
    return pass();
}


/**********************************************************************//**
 * Test Uranus
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_uranus(void)
{
    // Create the planet object
    CEPlanet uranus = CEPlanet::Uranus();

    // Update the coordinates
    uranus.UpdateCoordinates(base_date_.JD());

    // Run the actual test with values derived from AstroPy
    test_planet(uranus,
                CECoordinates(319.0662412483117691, -16.5756054581048744, 
                              CECoordinateType::ICRS, 
                              CEAngleType::DEGREES),
                {14.42492523, -12.50957402, -5.6830779},
                {0.00269067, 0.00244776, 0.00103396});
    
    return pass();
}


/**********************************************************************//**
 * Test Neptune
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_neptune(void)
{
    // Create the planet object
    CEPlanet neptune = CEPlanet::Neptune();

    // Update the coordinates
    neptune.UpdateCoordinates(base_date_.JD());

    // Run the actual test with values derived from AstroPy
    test_planet(neptune,
                CECoordinates(306.1731137010386306, -19.0396553491478997, 
                              CECoordinateType::ICRS, 
                              CEAngleType::DEGREES),
                {16.80489053, -22.98266171, -9.82533257},
                {0.00258607, 0.00165554, 0.00061312});

    return pass();
}


/**********************************************************************//**
 * Test parameters for a given planet
 * 
 * @param[in] test_planet           Planet information to be tested
 * @param[in] true_obs              Expected observed coordinates
 * @param[in] true_pos              Expected physical position (AU)
 * @param[in] true_vel              Expected physical velocity (AU/day)
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_planet(const CEPlanet&            test_planet,
                                const CECoordinates&       true_obs,
                                const std::vector<double>& true_pos,
                                const std::vector<double>& true_vel)
{
    // Define a name so that we actually know which planet we're testing
    std::string func_name = std::string(__func__) + " (" + test_planet.Name() + ")";

    CECoordinates icrs_coords(test_planet.XCoordinate_Rad(),
                              test_planet.YCoordinate_Rad(),
                              CECoordinateType::ICRS,
                              CEAngleType::RADIANS);
    
    if (!test(icrs_coords == true_obs, func_name, __LINE__)) {
        // Print information about the coordinates
        std::printf("   %s   %s   %s", icrs_coords.print().c_str(), true_obs.print().c_str(), icrs_coords.print().c_str());
        std::printf("   X-diff: %f arcsec\n", (icrs_coords.XCoordinate_Deg()-true_obs.XCoordinate_Deg())*3600.0);
        std::printf("   Y-diff: %f arcsec\n", (icrs_coords.YCoordinate_Deg()-true_obs.YCoordinate_Deg())*3600.0);
    }
    std::printf("      AngSep: %e arcsec\n", icrs_coords.AngularSeparation(true_obs, CEAngleType::DEGREES)*3600.0);

    // Update the tolerance
    double tol_old = DblTol();
    SetDblTol(1.0e-5);

    // Test the computed ICRS position values
    std::vector<double> test_pos = test_planet.PositionICRS();
    if (!test_vect(test_pos, true_pos, func_name, __LINE__)) {
        double dist(0.0);
        for (int i=0; i<test_pos.size(); i++) {
            double offset = (test_pos[i]-true_pos[i]);
            std::printf("      test: %f AU | expected: %f AU (rel. diff: %e AU)\n", 
                        test_pos[i], true_pos[i], offset/true_pos[i]);
            dist += (offset * offset);
        }
        std::printf("      Offset: %e AU\n", std::sqrt(dist));
    }

    // Test the computed ICRS velocities
    std::vector<double> test_vel = test_planet.VelocityICRS();
    if (!test_vect(test_vel, true_vel, func_name, __LINE__)) {
        double dist(0.0);
        for (int i=0; i<test_vel.size(); i++) {
            double offset = (test_vel[i]-true_vel[i]);
            std::printf("      test: %e AU/day | expected: %e AU/day (rel. diff: %e AU/day)\n", 
                        test_vel[i], true_vel[i], offset/true_vel[i]);
            dist += (offset * offset);
        }
        std::printf("      Offset: %e AU/day\n", std::sqrt(dist));
    }

    // Reset tolerance
    SetDblTol(tol_old);

    return pass();
}


/**********************************************************************//**
 * Compare 
 * 
 * @return whether the tests succeed
 *************************************************************************/
bool test_CEPlanet::test_Planets(void)
{
    test_mercury();
    test_venus();
    test_earth();
    test_mars();
    test_jupiter();
    test_saturn();
    test_uranus();
    test_neptune();

    return pass();
}


/**********************************************************************//**
 * Main method that actually runs the tests
 *************************************************************************/
int main(int argc, char** argv) 
{
    test_CEPlanet tester;
    return (!tester.runtests());
}

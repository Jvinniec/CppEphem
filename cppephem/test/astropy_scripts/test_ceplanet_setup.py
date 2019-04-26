
# Import the astropy modules that we need
import astropy.units as u
from astropy.coordinates import AltAz
from astropy.coordinates import SkyCoord
from astropy.coordinates import ICRS
from astropy.coordinates import solar_system
from astropy.coordinates import EarthLocation
from astropy.time import Time


# Print the results of the planet position test
def print_planet_position(name, observer, time):
    """
    Print information about the planets position

    Parameters
    ----------
    name : str
        String name of planet (e.g. 'mercury' or 'jupiter')
    observer : astropy.coordinates.EarthLocation
        Observer object
    time : astropy.time.Time
        Time of the observation
    """
    # Compute the observed coordinates of the object
    planet = solar_system.get_body(body     = name, 
                                   time     = time, 
                                   location = observer)
    aa   = AltAz(location=observer, obstime=time)
    obs  = planet.transform_to(aa)
    icrs = planet.transform_to('icrs')

    # Compute the position/velocity of the object
    pos, vel = solar_system.get_body_barycentric_posvel(body = name, 
                                                        time = time)
    print(f"{name:8}: obs=({obs.az:20.16f},{obs.zen:20.16f}) | icrs=({icrs.ra:20.16f},{icrs.dec:20.16f})")
    print(f"          pos={pos}, vel={vel}")


# Run the planet tests
def test_planets():
    """
    Compute the coordinates, position, and velocity for each major planet
    """
    # Observer position
    earth_pos = EarthLocation(lat=0*u.deg, lon=0*u.deg, height=0*u.m)

    # Observation time
    observing_time = Time('2000-01-01 12:00:00', scale='utc')

    # Names of each of the major planets
    planets = ['mercury', 'venus', 'earth', 'mars', 'jupiter', 'saturn', 'uranus', 'neptune']

    # Loop over each planet
    for planet in planets:
        print_planet_position(name     = planet,
                              observer = earth_pos,
                              time     = observing_time)


# Main method
if __name__ == "__main__":
    test_planets()

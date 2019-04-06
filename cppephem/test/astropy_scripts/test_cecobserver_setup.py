import astropy.units as u
from astropy.coordinates import AltAz
from astropy.coordinates import SkyCoord
from astropy.coordinates import EarthLocation
from astropy.time import Time

print("Computing values for test_CEObserver::test_get_obsCoords()")

earth_pos = EarthLocation(lat='0d0m', lon='0d0m', height=0*u.m)
observing_time = Time('2000-01-01 12:00')
aa = AltAz(location=earth_pos, obstime=observing_time)
test = SkyCoord(0.0*u.deg, 90.0*u.deg, frame='icrs')
obs_coords = test.transform_to(aa)
zenith = 90 - obs_coords.alt.deg

print(f"Input: {test}")
print(f"Output: (az,alt) = ({obs_coords.az}, {zenith} deg)")

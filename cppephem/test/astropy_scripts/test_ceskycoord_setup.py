import astropy.units as u
from astropy.coordinates import AltAz
from astropy.coordinates import SkyCoord
from astropy.coordinates import CIRS
from astropy.coordinates import EarthLocation
from astropy.coordinates import BarycentricMeanEcliptic
from astropy.coordinates import BarycentricTrueEcliptic
from astropy.time import Time


# ==================================
# INITIAL INPUT VALUE CALCULATIONS
# ==================================
print("Computing input values for test_CECoordinates")

# Coordinates to be observed
test = SkyCoord(83.633*u.deg, 22.0145*u.deg, frame='icrs')
# Observer position
earth_pos = EarthLocation(lat=0*u.deg, lon=0*u.deg, height=0*u.m)
# Observation time
observing_time = Time('2000-01-01 12:00:00')
# Observing & CIRS transformation system
aa = AltAz(location=earth_pos, obstime=observing_time)
ci = CIRS(obstime=observing_time)
# Ecliptic transformation system
ec_mean = BarycentricMeanEcliptic(equinox=observing_time)
ec_true = BarycentricTrueEcliptic(equinox=observing_time)

# Now generate the converted coordinates 
obs_coords = test.transform_to(aa)
zenith = 90 - obs_coords.alt.deg
cirs = test.transform_to(ci)
gal  = test.transform_to(frame='galactic')
ecm  = test.transform_to(ec_mean)
ect  = test.transform_to(ec_true)

# Print the results
print(f"Date    : {observing_time}")
print(f"In ICRS : (ra,dec) = ({test.ra}, {test.dec})")
print(f"CIRS    : (ra,dec) = ({cirs.ra}, {cirs.dec}")
print(f"Galactic: (l,b)    = ({gal.l}, {gal.b})")
print(f"Observed: (az,alt) = ({obs_coords.az}, {zenith} deg)")
print(f"Ecliptic (mean): (lon,lat) = ({ecm.lon}, {ecm.lat})")
print(f"Ecliptic (true): (lon,lat) = ({ect.lon}, {ect.lat})")

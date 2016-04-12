This documents the CppEphem package.

Author: Joshua V. Cardenzana
email: jvinniec@gmail.com
Creation Date: March 2016

DISCLAIMER:
----------------------------------------------------------
The author above had no connection or involvement in the
development of the SOFA software. Their software is included
in this package because it is the standard provided by the
International Astronomical Union (IAU). I dont claim to own
their code. You can view the readme file in the sofa directory
for more information on this software. (Can I not be sued now?)

In all seriousness, the SOFA software package is a rather
impressive and phenominally useful piece of code. I am really
grateful to the authors of that package for putting it together.

PURPOSE:
----------------------------------------------------------
The purpose of this package is to leverage the Standards
Of Fundamental Astronomy (SOFA) package in order to allow
simple coordinate conversion routines. This should allow
converting between [RA,Dec], [Galactice Long, Lat], and
local sky coordinates.

Here is a list of purposes I wish this code to serve:
* Basic coordinate conversion routines (currently not working, first priority)
  - CIRS (Earth centric RA,Dec)
  - ICRS (Solarsystem barycentric RA, Dec) 
  - Galactic (Long, Lat) 
  - Observed (Azimuth, Zenith)
* Planet ephemeris (currently not working, 3rd priority
  - Planet positions for a given observer
* Basic time conversion routines (currently not working, 2nd priority)
  - UTC
  - Local time
  - Greenwich apparent sidereal time
  - Local apparent sidereal time
* Date conversion routines (see CEDate, currently working)
  - Julian Date
  - Modified Julian Date
  - Gregorian Calendar year, month, day

If there is any feature that you would like to see implemented
feel free to contact the author or submit an issue and I'll
look into it.
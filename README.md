This documents the CppEphem package.

* Author: Joshua V. Cardenzana
* Email: jvinniec <at> gmail.com
* Creation Date: March 2016

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
* Basic coordinate conversion routines (partially implemented, first priority)
  - CIRS (Earth centric RA,Dec)
  - ICRS (Solarsystem barycentric RA, Dec) 
  - Galactic (Long, Lat) 
  - Observed (Azimuth, Zenith)
* Star & Planet ephemeris (currently not implemented, 3rd priority)
  - Star & Planet positions for a given observer at a given time
* Basic time conversion routines (currently not implemented, 2nd priority)
  - UTC
  - Local time
  - Greenwich apparent sidereal time
  - Local apparent sidereal time
* Date conversion routines (see CEDate, fully implemented)
  - Julian Date
  - Modified Julian Date
  - Gregorian Calendar year, month, day

More feature will be implemented as time permits. If there 
is any feature that you would like to see implemented feel 
free to contact the author or submit an issue and I'll
look into it.

The following is a list of currently implemented compiled
executables which can be run from the command line:
* Date conversion routines
  - __cal2jd__: Gregorian calendar to Julian date
  - __cal2mjd__: Gregorian calendar to modified Julian date
  - __jd2cal__: Julian date to Gregorian calendar date
  - __jd2mjd__: Julian date to modified Julian date
  - __mjd2cal__: Modified Julian date to Gregorian calendar date
  - __mjd2jd__: Modified Julian date to Julian date
* Coordinate conversion routines
  - __cirs2obs__: CIRS to Observed coordinates
  - __cirs2gal__: CIRS to Galactic coordinates

Downloading the code:
----------------------------------------------------------
To obtain the code, it should be as simple as cloning the
repository from github:
```bash
git clone https://github.com/Jvinniec/CppEphem.git CppEphem
```

Building the code:
----------------------------------------------------------
As long as the user has the most up to date version of autotools,
you should be able to build the software very easily using the 
standard "./configure -> make -> make install" method. Here is
a bit more detail.

First, download the repository as described above in "Downloading 
the code". Second, make sure that the "configure" file exists in 
the top directory. If not, then do:

``` . autogen.sh ```

Third, configure the software (note the "prefix" option is optional):

``` ./configure [--prefix=/your/install/directory] ```

Fourth, build the code:

``` make ```

Finally, to install the code in an accesible manner (i.e. so that 
your PATH and (DY)LD_LIBRARY_PATH environment variables know where
the executables and libraries are) type:

``` make install ```

And that should do it.


This documents the [CppEphem](https://github.com/Jvinniec/CppEphem) package.

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/f26c88aef73645d099d80584ec4e7ccc)](https://www.codacy.com/app/Jvinniec/CppEphem?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Jvinniec/CppEphem&amp;utm_campaign=Badge_Grade)

* Author: Joshua V. Cardenzana
* Creation Date: March 2016
* [Documentation available here](http://jvinniec.github.io/CppEphem/documentation/html/index.html)
* [Download available here](https://github.com/Jvinniec/CppEphem)
* [Wiki (on GitHub)](https://github.com/Jvinniec/CppEphem/wiki)

Interested in contributing? Checkout the [issues](https://github.com/Jvinniec/CppEphem/issues) 
and [projects](https://github.com/Jvinniec/CppEphem/projects) pages to see what
features have been suggested, or to suggest your own!

PURPOSE:
----------------------------------------------------------
The purpose of this package is to leverage the Standards
Of Fundamental Astronomy (SOFA) package in order to compute
several astrophysical quantities regarding the positions of
objects. CppEphem wraps the SOFA functions in easy to use
classes, so that astronomers can quickly and easily compute
the quantities they need in their own programs.

Here is a list of purposes I wish this code to serve:
* Basic coordinate conversion routines (see [CECoordinates](http://jvinniec.github.io/CppEphem/documentation/html/classCECoordinates.html), fully implemented except for ecliptic coordinates)
  - [CIRS](https://github.com/Jvinniec/CppEphem/wiki/Coordinate-Systems#cirs) (Earth centric RA,Dec)
  - [ICRS](https://github.com/Jvinniec/CppEphem/wiki/Coordinate-Systems#icrs) (Solarsystem barycentric RA, Dec)
  - [Galactic](https://github.com/Jvinniec/CppEphem/wiki/Coordinate-Systems#galactic) (Long, Lat)
  - [Observed](https://github.com/Jvinniec/CppEphem/wiki/Coordinate-Systems#observed) (Azimuth, Zenith angle) (Note: Zenith angle = 90&deg; - Altitude)
  - [Ecliptic](https://github.com/Jvinniec/CppEphem/wiki/Coordinate-Systems#ecliptic) (Long, Lat) Will allow for either heliocentric or Earth centric variants (Not yet implemented)
* Star & Planet ephemeris
  - __Star positions__ for a given observer at a given time (not accounting for proper motion yet)
  - __Planet positions__ for a given observer at a given time (implemented in [CEPlanet](http://jvinniec.github.io/CppEphem/documentation/html/classCEPlanet.html), see tutorials ce101, ce104, planetephem.cpp, and planetpositions.cpp)
* Basic time conversion routines (partially implemented, next priority)
  - __UTC__ (Current UTC obtainable from system clock)
  - __Local time__ (Current local time obtained from system clock, however user can hand set "UTC offset" if desired)
  - Greenwich apparent sidereal time
  - Local apparent sidereal time
  - Note: Methods also exist for passing in value for "DUT1" (i.e. UT1-UTC)
* Date conversion routines (see [CEDate](http://jvinniec.github.io/CppEphem/documentation/html/classCEDate.html), fully implemented)
  - __Julian Date__
  - __Modified Julian Date__
  - __Gregorian Calendar__ (year, month, day)

More features will be implemented as time permits. If there 
is any feature that you would like to see implemented feel 
free to contact the author or submit an issue and I'll
look into it.

The following is a list of currently fully implemented, compiled
executables which can be run from the command line:
* Date conversion routines
  - __cal2jd__: Gregorian calendar to Julian date
  - __cal2mjd__: Gregorian calendar to modified Julian date
  - __jd2cal__: Julian date to Gregorian calendar date
  - __jd2mjd__: Julian date to modified Julian date
  - __mjd2cal__: Modified Julian date to Gregorian calendar date
  - __mjd2jd__: Modified Julian date to Julian date
* Coordinate conversion routines (all angles are expected in degrees)
  - __cirs2obs__: CIRS to Observed coordinates
  - __cirs2gal__: CIRS to Galactic coordinates
  - __gal2cirs__: Galactic to CIRS coordinates
  - __gal2obs__: Galactic to Observed coordinates
  - __obs2cirs__: Observed to CIRS coordinates
  - __obs2icrs__: Observed to ICRS coordinates
  - __obs2gal__: Observed to Galactic coordinates
  - __angsep__: Compute the angular separation between two coordinates
* Planet ephemeris producer
  - [__planetephem__](https://github.com/Jvinniec/CppEphem/wiki/planetephem): Computes the ephemeris for a given planet using user supplied geographic and time values.
  - __planetpositions__: Computes current RA,Dec and Az,Alt for planets taking in user specific location (requires ncurses, not compiled by default)

Downloading the code:
----------------------------------------------------------
To obtain the code, it should be as simple as cloning the
repository from github:

```bash
git clone https://github.com/Jvinniec/CppEphem.git CppEphem
```

Building the code:
----------------------------------------------------------
The code can either be compiled using 'cmake' or autotools. To compile
via cmake use the following set of commands:

```bash
cmake [-Dprefix=/install/dir]
make
```

NOTE: "-Dprefix=" and "-DCMAKE_INSTALL_PREFIX=" will have equivalent behavior.
This will create a 'build' directory in the top level directory of the 
CppEphem distribution and initially put all of the compiled libraries and
executables in there. Once that's done, you can install the files by running

```bash
make install
```
This will install the headers, executables, and libraries in the default
installation directories (or alternatively the directory specified in 
'-Dprefix' during the initial run of cmake).

To install via autotools, you should be able to build the software 
very easily using the standard "./configure -> make -> make install" 
method. Here is a bit more detail.

First, download the repository as described above in "Downloading 
the code". Second, make sure that the "configure" file exists in 
the top directory. If not, then do:

```bash
. autogen.sh 
```

Third, configure the software (note the "prefix" option is optional):

```bash
./configure [--prefix=/your/install/directory] 
```

Fourth, build the code:

```bash
make 
```

Finally, to install the code in an accesible manner (i.e. so that 
your PATH and (DY)LD_LIBRARY_PATH environment variables know where
the executables and libraries are) type:

```bash
make install
```

And that should do it.

Uninstalling the code:
----------------------------------------------------------
To uninstall the code, it is advised to first run

```bash
make uninstall
```

in the top directory in order to remove the executables from your 
base install directory. Then you can delete the downloaded git repository.

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

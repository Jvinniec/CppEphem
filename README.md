CPPEPHEM
----------------------------------------------------------
[![Build Status](https://travis-ci.org/Jvinniec/CppEphem.svg?branch=master)](https://travis-ci.org/Jvinniec/CppEphem)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=cpp-ephem&metric=alert_status)](https://sonarcloud.io/dashboard?id=cpp-ephem)
[![Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=cpp-ephem&metric=sqale_rating)](https://sonarcloud.io/dashboard?id=cpp-ephem)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=cpp-ephem&metric=coverage)](https://sonarcloud.io/dashboard?id=cpp-ephem)

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
  - ~~[Ecliptic](https://github.com/Jvinniec/CppEphem/wiki/Coordinate-Systems#ecliptic)~~ (Not yet implemented, Long, Lat) Will allow for either heliocentric or Earth centric variants
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
  - __cirs2icrs__: CIRS to ICRS coordinates
  - __cirs2obs__: CIRS to Observed coordinates
  - __cirs2gal__: CIRS to Galactic coordinates
  - __gal2icrs__: Galactic to ICRS coordinates
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
Once the code is downloaded, the advised method for compiling the code is via `cmake`. To compile via `cmake` use the following instructions:

1. Create a new directory to build the code in. This enables you to create a completely clean build in the future by simply deleting and recreating this directory.
   ```bash
   mkdir /build/dir/
   cd /build/dir/
   ```
2. Now run `cmake` with your desired configuration options to build the code:
   ```bash
   # configure
   cmake [-DCMAKE_INSTALL_PREFIX=/install/dir] /path/to/cloned/CppEphem
   # build (N = number of threads for compilation)
   cmake --build . -- [-jN]
   ```
   NOTE: "-Dprefix=" and "-DCMAKE_INSTALL_PREFIX=" will have equivalent behavior of specifying the directory where the code will be installed into.

   OPTIONAL: You can now test that the code actually works
   ```bash
   cmake --build . --target test
   make test
   ```
3. Install the code into the specified installation directory:
   ```bash
   cmake --build . --target install
   ```
   This will install the headers, executables, and libraries in the default
installation directories (or alternatively the directory specified in 
'-DCMAKE_INSTALL_PREFIX' during the initial run of cmake). You should make sure to update your `$PATH` and `$(DY)LD_LIBRARY_PATH` if installing the code to a non-standard location:
   
   ```bash
   # On MAC:
   export PATH=${PATH}:/install/dir/bin
   export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:/install/dir/lib
   # On Linux
   export PATH=${PATH}:/install/dir/bin
   export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/install/dir/lib 
   ```


## Autotools Build
**NOTE:** *The code has grown to a point where it is no longer feasible to maintain two build systems. Because of this, it has been decided to depricate the autotools build system and remove it in v1.3.*

To install via autotools, you should be able to build the software 
very easily using the standard "./configure -> make -> make install" 
method. Here is a bit more detail.

First, download the repository as described above in "Downloading 
the code". Second, make sure that the "configure" file exists in 
the top directory. If not, then do:

```bash
./autogen.sh 
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

### Troubleshooting
If you run into a problem installing when using `cmake` and the `-Dprefix=` 
option along the following form:

```
CMake Error at cmake_install.cmake:44 (file):
  file INSTALL cannot set permissions on "~/Applications/CppEphem//include"
```

Make sure that you pass the full path to the installation directory (i.e. don't
use relative paths with '`~/`').

Uninstalling the code:
----------------------------------------------------------
To uninstall the code, it is advised to first run

```bash
make uninstall
```

in the top directory in order to remove the executables from your 
base install directory. Then you can delete the downloaded git repository.

Note: This only works if the code was compiled with autotools, since
cmake does not support `make uninstall`.

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

# Changelog
All notable changes to the CppEphem package will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## v1.2.99 : HEAD
 - Update SOFA to latest release (2019-07-22) [#27]
 - Deprecate CECoordinates class and make all classes and executables use CESkyCoord class
 - Add new CESkyCoord class as a simplified version of CECoordinates class [#20]


## v1.2.0 : 2019-05-16
- Fix Autotools build system [#22]
- Add CEExecOptions class defining all executable options in a standard way
- Update CEPlanet's conversion to observed coordinates
- Update SOFA files to 20180130 [#5]
- Add executables for cirs2icrs and gal2icrs [#14]
- Standardize all executable parameters [#10]
- Update CLOptions to fix issues related to command line parameters [#10]
- Add tests for all classes
- Add tests for all executables
- Add code coverage statistics to SonarCloud report
- Add download and loading for TT-UT1 correction [#13]
- Add download and loading for dut1 and polar motion corrections from file [#13]
- Add optional dependency of libcurl to download corrections file
- Remove date object from CEObserver class


## v1.1.1 : 2019-01-30
- Fix segfault in `cirs2obs` and `gal2obs` [#6]


## v1.1.0 : 2017-03-20
- Add angular separation computation between two coordinates (CECoordinates::AngularSeparation())


## v1.0.0 : 2017-02-02
- Add support for planet ephemerides


## v0.1.0 : 2016-07-15
- Initial release. 
- Contains a preliminary version of the code and latest (20160503) version of the SOFA software.

# Changelog
All notable changes to the CppEphem package will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## HEAD
- Add tests for all classes
- Add tests for all executables
- Add code coverage statistics to SonarCloud report
- Add download and loading for dut1 and polar motion corrections from file
- Add optional dependency of libcurl to download corrections file
- Remove date object from CEObserver class


## v1.1.1 : 2019-01-30
- Fix segfault in `cirs2obs` and `gal2obs` (#6)


## v1.1.0 : 2017-03-20
- Add angular separation computation between two coordinates (CECoordinates::AngularSeparation())


## v1.0.0 : 2017-02-02
- Add support for planet ephemerides


## v0.1.0 : 2016-07-15
- Initial release. 
- Contains a preliminary version of the code and latest (20160503) version of the SOFA software.
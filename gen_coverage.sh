#!/bin/bash

# ------------------------------------------------------
# This file is meant to be run on Travis-CI only
# ------------------------------------------------------

# Define some control variables
outdir="build/coverage_report"
cov_reports="${outdir}/coverage_datafiles.txt"
merge_report="${outdir}/cppephem.profdata"

# Change into the build directory
mkdir ${outdir}

if [[ "$TRAVIS_OS_NAME" == "linux" && "$CC" == "clang" ]] ; then
    # Build the linux system wrapper
    build-wrapper-linux-x86-64 --out-dir bw_output make clean all

    # Run all of the individual tests with output
    # NOTE: Make sure the coverage files end with '.profraw'
    echo "Running coverage"

    # Define corrections files for testing
    CORRFILES="--nutation=build/share/cppephem/nutation.txt --ttut1hist=build/share/cppephem/ttut1_historic.txt --ttut1pred=build/share/cppephem/ttut1_predicted.txt"

    LLVM_PROFILE_FILE="${outdir}/CEAngle.profraw"       ./build/bin/test_CEAngle
    LLVM_PROFILE_FILE="${outdir}/CEBody.profraw"        ./build/bin/test_CEBody
    LLVM_PROFILE_FILE="${outdir}/CECoordinates.profraw" ./build/bin/test_CECoordinates
    LLVM_PROFILE_FILE="${outdir}/CEDate.profraw"        ./build/bin/test_CEDate
    LLVM_PROFILE_FILE="${outdir}/CEException.profraw"   ./build/bin/test_CEException
    LLVM_PROFILE_FILE="${outdir}/CENamespace.profraw"   ./build/bin/test_CENamespace
    LLVM_PROFILE_FILE="${outdir}/CEObservation.profraw" ./build/bin/test_CEObservation
    LLVM_PROFILE_FILE="${outdir}/CEObserver.profraw"    ./build/bin/test_CEObserver
    LLVM_PROFILE_FILE="${outdir}/CEPlanet.profraw"      ./build/bin/test_CEPlanet
    LLVM_PROFILE_FILE="${outdir}/CERunningDate.profraw" ./build/bin/test_CERunningDate
    LLVM_PROFILE_FILE="${outdir}/CESkyCoord.profraw"    ./build/bin/test_CESkyCoord
    LLVM_PROFILE_FILE="${outdir}/CETime.profraw"        ./build/bin/test_CETime

    # Generic executables
    LLVM_PROFILE_FILE="${outdir}/angsep1.profraw"       ./build/bin/angsep --xcoord1=0.0 --ycoord1=0.0 --xcoord2=0.0 --ycoord2=1.0
    LLVM_PROFILE_FILE="${outdir}/angsep2.profraw"       ./build/bin/angsep --xcoord1=0.0 --ycoord1=0.0 --xcoord2=0.0 --ycoord2=0.01745329251 --InputDegrees=0 --OutputDegrees=0

    LLVM_PROFILE_FILE="${outdir}/planetephem.profraw"   ./build/bin/planetephem $CORRFILES --planet=4 --longitude=0 --latitude=42 --elevation=0 --startJD=2451545.0

    # Date conversions (multiple versions to allow testing all the code)
    LLVM_PROFILE_FILE="${outdir}/cal2jd1.profraw"       ./build/bin/cal2jd
    LLVM_PROFILE_FILE="${outdir}/cal2jd2.profraw"       ./build/bin/cal2jd 20000101.5
    LLVM_PROFILE_FILE="${outdir}/cal2jd3.profraw"       ./build/bin/cal2jd 2000 1 1.5

    LLVM_PROFILE_FILE="${outdir}/cal2mjd1.profraw"      ./build/bin/cal2mjd
    LLVM_PROFILE_FILE="${outdir}/cal2mjd2.profraw"      ./build/bin/cal2mjd 20000101.5
    LLVM_PROFILE_FILE="${outdir}/cal2mjd3.profraw"      ./build/bin/cal2mjd 2000 1 1.5

    LLVM_PROFILE_FILE="${outdir}/jd2cal1.profraw"       ./build/bin/jd2cal
    LLVM_PROFILE_FILE="${outdir}/jd2cal2.profraw"       ./build/bin/jd2cal 2458516.0
    LLVM_PROFILE_FILE="${outdir}/jd2cal3.profraw"       ./build/bin/jd2cal 2458516.0 1

    LLVM_PROFILE_FILE="${outdir}/jd2mjd1.profraw"       ./build/bin/jd2mjd
    LLVM_PROFILE_FILE="${outdir}/jd2mjd2.profraw"       ./build/bin/jd2mjd 2458516.0

    LLVM_PROFILE_FILE="${outdir}/mjd2cal1.profraw"      ./build/bin/mjd2cal
    LLVM_PROFILE_FILE="${outdir}/mjd2cal2.profraw"      ./build/bin/mjd2cal 58515.5
    LLVM_PROFILE_FILE="${outdir}/mjd2cal3.profraw"      ./build/bin/mjd2cal 58515.5 1

    LLVM_PROFILE_FILE="${outdir}/mjd2jd1.profraw"       ./build/bin/mjd2jd
    LLVM_PROFILE_FILE="${outdir}/mjd2jd2.profraw"       ./build/bin/mjd2jd 58515.5

    # Coordinate conversion tests
    LLVM_PROFILE_FILE="${outdir}/cirs2gal.profraw"      ./build/bin/test_cirs2gal --ra=83.633 --dec=22.0145 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/cirs2icrs.profraw"     ./build/bin/test_cirs2icrs --ra=83.633 --dec=22.0145 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/cirs2obs.profraw"      ./build/bin/test_cirs2obs --ra=83.633 --dec=22.0145 --longitude=0.0 --latitude=0.0 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/icrs2cirs.profraw"     ./build/bin/test_icrs2cirs --ra=83.633 --dec=22.0145 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/icrs2gal.profraw"      ./build/bin/test_icrs2gal --ra=83.633 --dec=22.0145
    LLVM_PROFILE_FILE="${outdir}/icrs2obs.profraw"      ./build/bin/test_icrs2obs --ra=83.633 --dec=22.0145 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/gal2cirs.profraw"      ./build/bin/test_gal2cirs --glon=184.553 --glat=-5.788 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/gal2icrs.profraw"      ./build/bin/test_gal2icrs --glon=184.553 --glat=-5.788
    LLVM_PROFILE_FILE="${outdir}/gal2obs.profraw"       ./build/bin/test_gal2obs --glon=184.553 --glat=-5.788 --longitude=0.0 --latitude=0.0  --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/obs2cirs.profraw"      ./build/bin/test_obs2cirs --longitude=0.0 --latitude=0.0 --azimuth=180 --zenith=20 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/obs2icrs.profraw"      ./build/bin/test_obs2icrs --longitude=0.0 --latitude=0.0 --azimuth=180 --zenith=20 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/obs2gal.profraw"       ./build/bin/test_obs2gal  --longitude=0.0 --latitude=0.0 --azimuth=180 --zenith=20 --juliandate=2451545.0

    LLVM_PROFILE_FILE="${outdir}/cirs2gal2.profraw"     ./build/bin/convcoord $CORRFILES --cirs=83.633,22.0145 --to=galactic --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/cirs2icrs2.profraw"    ./build/bin/convcoord $CORRFILES --cirs=83.633,22.0145 --to=icrs --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/cirs2obs2.profraw"     ./build/bin/convcoord $CORRFILES --cirs=83.633,22.0145 --to=observed --longitude=0.0 --latitude=0.0 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/cirs2ecl2.profraw"     ./build/bin/convcoord $CORRFILES --cirs=83.633,22.0145 --to=ecliptic --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/icrs2cirs2.profraw"    ./build/bin/convcoord $CORRFILES --icrs=83.633,22.0145 --to=cirs --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/icrs2gal2.profraw"     ./build/bin/convcoord $CORRFILES --icrs=83.633,22.0145 --to=galactic
    LLVM_PROFILE_FILE="${outdir}/icrs2obs2.profraw"     ./build/bin/convcoord $CORRFILES --icrs=83.633,22.0145 --to=observed --juliandate=2451545.0 --longitude=0.0 --latitude=0.0
    LLVM_PROFILE_FILE="${outdir}/icrs2ecl2.profraw"     ./build/bin/convcoord $CORRFILES --icrs=83.633,22.0145 --to=ecliptic --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/gal2cirs2.profraw"     ./build/bin/convcoord $CORRFILES --galactic=184.553,-5.788 --to=cirs --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/gal2icrs2.profraw"     ./build/bin/convcoord $CORRFILES --galactic=184.553,-5.788 --to=icrs
    LLVM_PROFILE_FILE="${outdir}/gal2obs2.profraw"      ./build/bin/convcoord $CORRFILES --galactic=184.553,-5.788 --to=observed --longitude=0.0 --latitude=0.0  --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/gal2ecl2.profraw"      ./build/bin/convcoord $CORRFILES --galactic=184.553,-5.788 --to=ecliptic --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/obs2cirs2.profraw"     ./build/bin/convcoord $CORRFILES --observed=180,20         --to=cirs --longitude=0.0 --latitude=0.0 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/obs2icrs2.profraw"     ./build/bin/convcoord $CORRFILES --observed=180,20         --to=icrs --longitude=0.0 --latitude=0.0 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/obs2gal2.profraw"      ./build/bin/convcoord $CORRFILES --observed=180,20         --to=galactic --longitude=0.0 --latitude=0.0 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/obs2ecl2.profraw"      ./build/bin/convcoord $CORRFILES --observed=180,20         --to=ecliptic --longitude=0.0 --latitude=0.0 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/ecl2cirs2.profraw"     ./build/bin/convcoord $CORRFILES --ecliptic=84.097,-1.294 --to=cirs --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/ecl2icrs2.profraw"     ./build/bin/convcoord $CORRFILES --ecliptic=84.097,-1.294 --to=icrs --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/ecl2gal2.profraw"      ./build/bin/convcoord $CORRFILES --ecliptic=84.097,-1.294 --to=galactic --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/ecl2obs2.profraw"      ./build/bin/convcoord $CORRFILES --ecliptic=84.097,-1.294 --to=observed --longitude=0.0 --latitude=0.0  --juliandate=2451545.0


    # Put all the coverage output files into a single file
    ls ${outdir}/*.profraw > ${cov_reports}

    # Merge the tests
    llvm-profdata merge \
        -sparse \
        -f ${cov_reports} \
        -o ${merge_report}

    # Generate the output text to be read by sonar-scanner
    llvm-cov show \
        -instr-profile ${merge_report} \
        -object ./build/bin/test_CEAngle \
        -object ./build/bin/test_CEBody \
        -object ./build/bin/test_CECoordinates \
        -object ./build/bin/test_CEDate \
        -object ./build/bin/test_CEException \
        -object ./build/bin/test_CENamespace \
        -object ./build/bin/test_CEObservation \
        -object ./build/bin/test_CEObserver \
        -object ./build/bin/test_CERunningDate \
        -object ./build/bin/test_CESkyCoord \
        -object ./build/bin/test_CETime \
        -object ./build/bin/angsep \
        -object ./build/bin/planetephem \
        -object ./build/bin/cal2jd \
        -object ./build/bin/cal2mjd \
        -object ./build/bin/jd2cal \
        -object ./build/bin/jd2mjd \
        -object ./build/bin/mjd2cal \
        -object ./build/bin/mjd2jd \
        -object ./build/bin/test_cirs2gal \
        -object ./build/bin/test_cirs2icrs \
        -object ./build/bin/test_cirs2obs \
        -object ./build/bin/test_icrs2cirs \
        -object ./build/bin/test_icrs2gal \
        -object ./build/bin/test_icrs2obs \
        -object ./build/bin/test_gal2cirs \
        -object ./build/bin/test_gal2icrs \
        -object ./build/bin/test_gal2obs \
        -object ./build/bin/test_obs2cirs \
        -object ./build/bin/test_obs2icrs \
        -object ./build/bin/test_obs2gal \
        -object ./build/bin/convcoord \
        > ${outdir}/coverage_report.txt

    # Run sonnar scanner to analyze code and coverage statistics
    sonar-scanner
fi

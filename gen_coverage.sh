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
    LLVM_PROFILE_FILE="${outdir}/CEBody.profraw"        ./build/bin/test_CEBody
    LLVM_PROFILE_FILE="${outdir}/CECoordinates.profraw" ./build/bin/test_CECoordinates
    LLVM_PROFILE_FILE="${outdir}/CEDate.profraw"        ./build/bin/test_CEDate
    LLVM_PROFILE_FILE="${outdir}/CEException.profraw"   ./build/bin/test_CEException
    LLVM_PROFILE_FILE="${outdir}/CENamespace.profraw"   ./build/bin/test_CENamespace
    LLVM_PROFILE_FILE="${outdir}/CEObservation.profraw" ./build/bin/test_CEObservation
    LLVM_PROFILE_FILE="${outdir}/CEObserver.profraw"    ./build/bin/test_CEObserver
    LLVM_PROFILE_FILE="${outdir}/CEPlanet.profraw"      ./build/bin/test_CEPlanet
    LLVM_PROFILE_FILE="${outdir}/CERunningDate.profraw" ./build/bin/test_CERunningDate
    LLVM_PROFILE_FILE="${outdir}/CETime.profraw"        ./build/bin/test_CETime

    # Generic executables
    LLVM_PROFILE_FILE="${outdir}/angsep.profraw"        ./build/bin/test_angsep --xcoord1=0.0 --ycoord1=0.0 --xcoord2=0.0 --ycoord2=1.0
    LLVM_PROFILE_FILE="${outdir}/planetephem.profraw"   ./build/bin/test_planetephem --planet=4 --longitude=0 --latitude=42 --elevation=0 --startJD=2451545.0

    # Date conversion tests
    LLVM_PROFILE_FILE="${outdir}/cal2jd.profraw"        ./build/bin/test_cal2jd 20000101.5
    LLVM_PROFILE_FILE="${outdir}/cal2mjd.profraw"       ./build/bin/test_cal2mjd 20000101.5
    LLVM_PROFILE_FILE="${outdir}/jd2cal.profraw"        ./build/bin/test_jd2cal 2458516.0
    LLVM_PROFILE_FILE="${outdir}/jd2mjd.profraw"        ./build/bin/test_jd2mjd 2458516.0
    LLVM_PROFILE_FILE="${outdir}/mjd2cal.profraw"       ./build/bin/test_mjd2cal 58515.5
    LLVM_PROFILE_FILE="${outdir}/mjd2jd.profraw"        ./build/bin/test_mjd2jd 58515.5

    # Coordinate conversion tests
    LLVM_PROFILE_FILE="${outdir}/cirs2gal.profraw"      ./build/bin/test_cirs2gal --ra=83.633 --dec=22.0145
    #LLVM_PROFILE_FILE="${outdir}/cirs2icrs.profraw"     ./build/bin/test_cirs2icrs --ra=83.633 --dec=22.0145
    LLVM_PROFILE_FILE="${outdir}/cirs2obs.profraw"      ./build/bin/test_cirs2obs --ra=83.633 --dec=22.0145 --longitude=0.0 --latitude=0.0 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/icrs2cirs.profraw"     ./build/bin/test_icrs2cirs --ra=83.633 --dec=22.0145 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/icrs2gal.profraw"      ./build/bin/test_icrs2gal --ra=83.633 --dec=22.0145
    LLVM_PROFILE_FILE="${outdir}/icrs2obs.profraw"      ./build/bin/test_icrs2obs --ra=83.633 --dec=22.0145 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/gal2cirs.profraw"      ./build/bin/test_gal2cirs --glon=184.553 --glat=-5.788 --juliandate=2451545.0
    #LLVM_PROFILE_FILE="${outdir}/gal2icrs.profraw"      ./build/bin/test_gal2icrs --glon=184.553 --glat=-5.788
    LLVM_PROFILE_FILE="${outdir}/gal2obs.profraw"       ./build/bin/test_gal2obs --glon=184.553 --glat=-5.788 --longitude=0.0 --latitude=0.0  --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/obs2cirs.profraw"      ./build/bin/test_obs2cirs --longitude=0.0 --latitude=0.0 --azimuth=180 --zenith=20 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/obs2icrs.profraw"      ./build/bin/test_obs2icrs --longitude=0.0 --latitude=0.0 --azimuth=180 --zenith=20 --juliandate=2451545.0
    LLVM_PROFILE_FILE="${outdir}/obs2gal.profraw"       ./build/bin/test_obs2gal  --longitude=0.0 --latitude=0.0 --azimuth=180 --zenith=20 --juliandate=2451545.0

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
        -object ./build/bin/test_CEBody \
        -object ./build/bin/test_CECoordinates \
        -object ./build/bin/test_CEDate \
        -object ./build/bin/test_CEException \
        -object ./build/bin/test_CENamespace \
        -object ./build/bin/test_CEObservation \
        -object ./build/bin/test_CEObserver \
        -object ./build/bin/test_CERunningDate \
        -object ./build/bin/test_CETime \
        -object ./build/bin/test_angsep \
        -object ./build/bin/test_cal2jd \
        -object ./build/bin/test_cal2mjd \
        -object ./build/bin/test_jd2cal \
        -object ./build/bin/test_jd2mjd \
        -object ./build/bin/test_mjd2cal \
        -object ./build/bin/test_mjd2jd \
        -object ./build/bin/test_cirs2gal \
        -object ./build/bin/test_cirs2obs \
        -object ./build/bin/test_icrs2cirs \
        -object ./build/bin/test_icrs2gal \
        -object ./build/bin/test_icrs2obs \
        -object ./build/bin/test_gal2cirs \
        -object ./build/bin/test_gal2obs \
        -object ./build/bin/test_obs2cirs \
        -object ./build/bin/test_obs2icrs \
        -object ./build/bin/test_obs2gal \
        > ${outdir}/coverage_report.txt

    # Run sonnar scanner to analyze code and coverage statistics
    sonar-scanner
fi

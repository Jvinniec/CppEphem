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
    LLVM_PROFILE_FILE="${outdir}/angsep.profraw"        ./build/bin/angsep --xcoord1=0.0 --ycoord1=0.0 --xcoord2=0.0 --ycoord2=1.0
    LLVM_PROFILE_FILE="${outdir}/icrs2gal.profraw"      ./build/bin/icrs2gal --ra=83.633 --dec=22.0145
    LLVM_PROFILE_FILE="${outdir}/cirs2gal.profraw"      ./build/bin/cirs2gal --ra=83.633 --dec=22.0145
    LLVM_PROFILE_FILE="${outdir}/planetephem.profraw"   ./build/bin/planetephem --planet=4 --longitude=0 --latitude=42 --elevation=0

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
        -object ./build/bin/angsep \
        -object ./build/bin/icrs2gal \
        -object ./build/bin/cirs2gal \
        > ${outdir}/coverage_report.txt

    # Run sonnar scanner to analyze code and coverage statistics
    sonar-scanner
fi

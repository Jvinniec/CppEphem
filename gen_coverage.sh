#!/bin/bash

# ------------------------------------------------------
# This file is meant to be run on Travis-CI only
# ------------------------------------------------------

# Define some control variables
outdir="build/coverage_report"
cov_reports="${outdir}/coverage_datafiles.txt"
merge_report="${outdir}/cppephem.profdata"

# Change into the build directory
echo "$PWD"
which bash
mkdir ${outdir}

if [[ "$TRAVIS_OS_NAME" == "linux" && "$CC" == "clang" ]] ; then
    # Build the linux system wrapper
    build-wrapper-linux-x86-64 --out-dir bw_output make clean all

    # Run all of the individual tests with output
    # NOTE: Make sure the coverage files end with '.profraw'
    echo "Running coverage"
    LLVM_PROFILE_FILE="${outdir}/CEDate.profraw" ./build/bin/test_CEDate

    # Put all the coverage output files into a single file
    ls ${outdir}/*.profraw > ${cov_reports}

    # Merge the tests
    llvm-profdata merge -sparse \
        -f ${cov_reports} \
        -o ${merge_report}

    # Generate the output text to be read by sonar-scanner
    llvm-cov show \
        -instr-profile ${merge_report} \
        -object ./build/bin/test_CEDate \
        > ${outdir}/coverage_report.txt

    # Run sonnar scanner to analyze code and coverage statistics
    sonar-scanner

    echo "ls:";ls
    echo "ls ${outdir}:";ls ${outdir}
fi

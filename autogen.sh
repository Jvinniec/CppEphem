#!/bin/bash
# ===================================================
# The purpose of this script is to install all of the
# necessary files for building the library.
# ===================================================

# First make sure that the 'm4' directory exists.
if [ ! -d "m4" ]; then
  # The directory doesnt exist, so we need to create it
    mkdir m4
fi

autoreconf -ifv -I m4
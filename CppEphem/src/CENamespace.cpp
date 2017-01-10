//
//  CENamespace.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 4/8/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#include <cmath>
#include <stdio.h>
#include "CENamespace.h"

//__________________________________________________
double CppEphem::dut1(double mjd) {
    // Round the MJD down to the nearest integer
    mjd = std::floor(mjd) ;
    
    // If the passed mjd is greater than the mininmum mjd or
    // larger than the largest mjd stored, then use the formula
    if (mjd < MINMJDFORDUT1) return 0.0 ;
    if (mjd > MAXMJDFORDUT1) return dut1Calc(mjd) ;
    
    // Load the list of dut1 values
    return dut1_list()[mjd] ;
}

//__________________________________________________
double CppEphem::dut1Error(double mjd) {
    // If the passed mjd is greater than the mininmum mjd or
    // larger than the largest mjd stored, then use the formula
    if (mjd < MINMJDFORDUT1) return 0.0 ;
    if (mjd > MAXMJDFORDUT1) return 0.0 ;
    
    // Load the list of dut1 values
    std::map<int, double> dut1Error = dut1Error_list() ;
    
    // Round the MJD down to the nearest integer
    mjd = std::floor(mjd) ;
    
    return dut1Error[mjd] ;
}

//__________________________________________________
double CppEphem::dut1Calc(double mjd)
{
    // There's a formula that could be used for calculating this value,
    // but I'm a bit lazy at the moment and since the correction is
    // very small, I'm just going to return 0 for the time being.
    return 0.0 ;
}

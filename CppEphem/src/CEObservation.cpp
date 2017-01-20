//
//  CELinkedCoordinates.cpp
//  CppEphem
//
//  Created by Josh Cardenzana on 1/19/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

#include <stdio.h>

#include "CEObservation.h"


////////////////////////////////////////////////////////////
/// Default constructor
CEObservation::CEObservation()
{
}

////////////////////////////////////////////////////////////
/// Constructor from a known observer and
///     @param observer         CEObserver linked with these coordinates
///     @param body             CEBody being observed
///     @param date             CEDate object linked with these coordinates
CEObservation::CEObservation(CEObserver* observer, CEBody* body, CEDate* date) :
    observer_(observer),
    body_(body),
    date_(date)
{
}
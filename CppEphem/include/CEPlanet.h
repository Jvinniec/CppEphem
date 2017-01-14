//
//  CEPlanet.h
//  CppEphem
//
//  Created by Josh Cardenzana on 1/13/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

#ifndef CEPlanet_h
#define CEPlanet_h

#include "CEBody.h"

class CEPlanet : public CEBody {
public:
    CEPlanet() ;
    CEPlanet(const std::string& name,
             CECoordinates coordinates) ;
    virtual ~CEPlanet() ;
protected:
    std::string name_ ;
};

#endif /* CEPlanet_h */

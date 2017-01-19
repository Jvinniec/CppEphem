//
//  CELinkedCoordinates.h
//  CppEphem
//
//  Created by Josh Cardenzana on 1/19/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

#ifndef CELinkedCoordinates_h
#define CELinkedCoordinates_h

#include "CECoordinates.h"
#include "CEDate.h"
#include "CEObserver.h"


class CELinkedCoordinates : public CECoordinates {
public:
    CELinkedCoordinates() ;
    CELinkedCoordinates(CEObserver
    virtual ~CELinkedCoordinates() ;
    
    /// Set the observer object
    virtual void SetObserver(CEObserver* new_observer) ;
    virtual void SetDate(CEDate* new_date) ;
    
protected:
    
    CEObserver* observer_ ;     /// Store a pointer to a CEObserver object
    CEDate* date_ ;             /// Store a pointer to a CEDate object
    
};

#endif /* CELinkedCoordinates_h */

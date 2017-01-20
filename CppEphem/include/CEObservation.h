//
//  CELinkedCoordinates.h
//  CppEphem
//
//  Created by Josh Cardenzana on 1/19/17.
//  Copyright © 2017 JCardenzana. All rights reserved.
//

#ifndef CEObservation_h
#define CEObservation_h

// CppEphem HEAD
#include "CECoordinates.h"
#include "CEBody.h"
#include "CEDate.h"
#include "CEObserver.h"


class CEObservation {
public:
    // Constructors
    CEObservation() ;
    CEObservation(CEObserver* observer, CEBody* body, CEDate* date) ;
    virtual ~CEObservation() ;
    
    /// Set the observer object
    virtual void SetObserver(CEObserver* new_observer) {observer_ = new_observer;}
    /// Set the object being observed
    virtual void SetBody(CEBody* new_body) {body_ = new_body;}
    /// Set the date of the observation
    virtual void SetDate(CEDate* new_date) {date_ = new_date;}
    
    /// Access the underlying objects
    CEObserver* Observer() {return observer_;}
    CEBody* Body() {return body_;}
    CEDate* Date() {return date_;}
    
protected:
    
    CEObserver* observer_ = nullptr;    /// Store a pointer to a CEObserver object
    CEDate* date_ = nullptr;            /// Store a pointer to a CEDate object
    CEBody* body_ = nullptr;            /// Object being observed
    
};

#endif /* CEObservation_h */

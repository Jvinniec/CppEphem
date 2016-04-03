//
//  CETime.h
//  AstroCoordinates
//
//  Created by Josh Cardenzana on 4/3/16.
//  Copyright © 2016 JCardenzana. All rights reserved.
//

#ifndef CETime_h
#define CETime_h

enum CETimeFormat {JD, MJD, Gregorian} ;

class CETime {
public:
    // Default constructor
    CETime() {} ;
    // Constructor from some date format
    CETime(double time, CETimeFormat time_format=CETimeFormat::JD) ;
    // Copy constructor
    CETime(const CETime& other) ;
    // Destructor
    virtual ~CETime() {}
    
    // Methods for converting between different formats
    static double GetMJDFromJD(double JD) ;
    static double GetJDFromMJD(double MJD) ;
    
protected:
    // Variables that hold the time information
    
private:
    
};

#endif /* CETime_h */

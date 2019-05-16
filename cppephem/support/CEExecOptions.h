

#ifndef CEExecOptions_h
#define CEExecOptions_h

#include "CppEphem.h"
#include "CLOptions.h"
#include "CEDate.h"
#include "CEObserver.h"

class CEExecOptions : public CLOptions {
public:

    /**********************************************************************//**
     * Default constructor
     *************************************************************************/
    CEExecOptions()
        {
            init_members();
        }
    /**********************************************************************//**
     * Construct from a program name
     * @param[in] prog_name     Name of the program
     *************************************************************************/
    CEExecOptions(const std::string& prog_name)
        {
            init_members();
            SetProgName(prog_name);
        }
    /**********************************************************************//**
     * Deconstruct
     *************************************************************************/
    virtual ~CEExecOptions() {};

    /**********************************************************************//**
     * Copy assignment operator
     * @param[in] other         CEExecOptions object to be copied
     *************************************************************************/
    CEExecOptions& operator=(const CEExecOptions& other)
        {
            // Make sure the two objects are not the same
            if (this != &other) {
                free_members();
                init_members();
                copy_members(other);
            }
            return *this;
        }

    /* Methods for adding variables */
    void AddCirsPars(void);
    void AddIcrsPars(void);
    void AddGalacticPars(void);
    void AddObservedPars(void);
    void AddObserverPars(void);
    void AddJDPar(void);

    void SetProgName(const std::string& prog_name);

private:

    /* Methods */
    void copy_members(const CEExecOptions& other);
    void init_members(void);
    void free_members(void);

    /* Variables */
    std::string prog_name_;
};


/**********************************************************************//**
 * Add parameters for CIRS coordinates
 * 
 * Adds the following parameters:
 * ------------------------------
 *  'r' or "ra"
 *      Right ascension in CIRS reference frame
 *  'd' or "dec"
 *      Declination in CIRS reference frame
 *************************************************************************/
inline
void CEExecOptions::AddCirsPars(void)
{
    AddDoubleParam("r,ra", "RA in the CIRS coordinate frame (degrees)", 0.0);
    AddDoubleParam("d,dec", "DEC in the CIRS coordinate frame (degrees)", 0.0);
}

/**********************************************************************//**
 * Add parameters for ICRS coordinates
 * 
 * Adds the following parameters:
 * ------------------------------
 *  'R' or "ra"
 *      Right ascension in ICRS reference frame
 *  'D' or "dec"
 *      Declination in ICRS reference frame
 *************************************************************************/
inline
void CEExecOptions::AddIcrsPars(void)
{
    AddDoubleParam("R,ra", "RA in ICRS coordinate frame (degrees)", 0.0);
    AddDoubleParam("D,dec", "DEC in the ICRS coordinate frame (degrees)", 0.0);
}

/**********************************************************************//**
 * Add parameters for Galactic coordinates
 * 
 * Adds the following parameters:
 * ------------------------------
 *  'l' or "glon"
 *      Galactic longitude (degrees)
 *  'b' or "glat"
 *      Galactic latitude (degrees)
 *************************************************************************/
inline
void CEExecOptions::AddGalacticPars(void)
{
    AddDoubleParam("l,glon", "Galactic longitude (degrees)", 0.0);
    AddDoubleParam("b,glat", "Galactic latitude (degrees)", 0.0);
}

/**********************************************************************//**
 * Add parameters for observed azimuth,zenith coordinates
 * 
 * Adds the following parameters:
 * ------------------------------
 *  'a' or "azimuth"
 *      Azimuth angle (degrees, north=0, east=90)
 *  'z' or "zenith"
 *      Zenith angle (degrees)
 *************************************************************************/
inline
void CEExecOptions::AddObservedPars(void)
{
    AddDoubleParam("a,azimuth", "Azimuth angle (degrees, north=0, east=90)", 0.0);
    AddDoubleParam("z,zenith", "Zenith angle (degrees)", 0.0);
}

/**********************************************************************//**
 * Add parameters defining an observer and their atmospheric properties
 * 
 * Adds the following parameters:
 * ------------------------------
 *  'x' or "longitude"
 *      Observer geographic longitude (degrees east positive)
 *  'y' or "latitude"
 *      Observer geographic latitude (degrees)
 *  "elevation"
 *      Observer elevation (meters)
 *  "pressure"
 *      Observer's atmospheric pressure (hPa)
 *  "humidity"
 *      Observer's relative humidity (0-1)
 *  "temperature"
 *      Observer's atmospheric temperature (Celsius)
 *  "wavelength"
 *      Observing wavelength (micrometers)
 *************************************************************************/
inline
void CEExecOptions::AddObserverPars(void)
{
    CEObserver obs;
    AddDoubleParam("x,longitude", "Observer geographic longitude (degrees east positive)", 0.0);
    AddDoubleParam("y,latitude", "Observer geographic latitude (degrees)", 0.0);
    AddDoubleParam("elevation", "Observer elevation (meters)", 0.0);
    AddDoubleParam("pressure", "Observer's atmospheric pressure (hPa)", obs.Pressure_hPa());
    AddDoubleParam("humidity", "Observer's atmospheric humicity (0-1)", obs.RelativeHumidity());
    AddDoubleParam("temperature", "Observer's atmospheric temperature (Celsius)", obs.Temperature_C());
    AddDoubleParam("wavelength", "Observing wavelength (micrometers)", obs.Wavelength_um());
}

/**********************************************************************//**
 * Add parameter for the Julian date
 * 
 * Adds the following parameters:
 * ------------------------------
 *  'j' or "juliandate"
 *      Input Julian date (defaults to current JD)
 *************************************************************************/
inline
void CEExecOptions::AddJDPar(void)
{
    AddDoubleParam("j,juliandate", "Input Julian date", CEDate::CurrentJD());
}

/**********************************************************************//**
 * Defines the program name and sets the version information
 * @param[in] prog_name         Program name
 *************************************************************************/
inline
void CEExecOptions::SetProgName(const std::string& prog_name)
{
    std::string vers_str = prog_name + " v" + CPPEPHEM_VERSION;
    prog_name_ = prog_name;
    AddVersionInfo(vers_str);
}

/**********************************************************************//**
 * Copy data members from another object of the same class
 * @param[in] other         CEExecOptions object to copy from
 *************************************************************************/
inline
void CEExecOptions::copy_members(const CEExecOptions& other)
{
    prog_name_ = other.prog_name_;
}

/**********************************************************************//**
 * Initialize data members
 *************************************************************************/
inline
void CEExecOptions::init_members(void)
{
    prog_name_ = "unnamed";
}

/**********************************************************************//**
 * Free explicitly allocated memory
 *************************************************************************/
inline
void CEExecOptions::free_members(void) 
{}

#endif /* CEExecOptions_h */

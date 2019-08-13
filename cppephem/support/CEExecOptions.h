

#ifndef CEExecOptions_h
#define CEExecOptions_h

#include "CppEphem.h"
#include "CLOptions.h"

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
    virtual ~CEExecOptions() {}

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
    
    void AddCorrFilePars(void);
    void AddCirsPar(void);
    void AddIcrsPar(void);
    void AddGalacticPar(void);
    void AddEclipticPar(void);
    void AddObservedPar(void);
    void AddObserverPars(void);
    void AddJDPar(void);

    CEObserver GenObserver(void);

    void SetCorrFiles(void);
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

    // Add date parameter
    AddJDPar();
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
 * Add parameter for the corrections file path
 * 
 * Adds the following parameters:
 * ------------------------------
 *  "corrfilepath""
 *      Input path to directory containing corrections files
 *************************************************************************/
inline
void CEExecOptions::AddCorrFilePars(void)
{
    AddStringParam("nutation", "File containing nutation corrections",
                   "");
    AddStringParam("ttut1hist", "File containing 'TT - UT1' corrections (historical)",
                   "");
    AddStringParam("ttut1pred", "File containing 'TT - UT1' corrections (predicted)",
                   "");
}

/**********************************************************************//**
 * Add parameters for CIRS coordinates
 * 
 * Adds the following parameters:
 * ------------------------------
 *  "cirs"
 *      CIRS "RA,Dec" (degrees)
 *************************************************************************/
inline
void CEExecOptions::AddCirsPar(void)
{
    AddStringParam("cirs", "Input CIRS RA,Dec coordinates (degrees)", "");
    if (!HasPar("delim")) {
        AddStringParam("delim", "Angle delimiter", "");
    }
}

/**********************************************************************//**
 * Add parameters for ICRS coordinates
 * 
 * Adds the following parameters:
 * ------------------------------
 *  "icrs"
 *      ICRS "RA,Dec" (degrees)
 *************************************************************************/
inline
void CEExecOptions::AddIcrsPar(void)
{
    AddStringParam("icrs", "Input ICRS RA,Dec coordinates (degrees)", "");
    if (!HasPar("delim")) {
        AddStringParam("delim", "Angle delimiter", "");
    }
}

/**********************************************************************//**
 * Add parameters for Galactic coordinates
 * 
 * Adds the following parameters:
 * ------------------------------
 *  "galactic"
 *      Galactic "longitude,latitude" (degrees)
 *************************************************************************/
inline
void CEExecOptions::AddGalacticPar(void)
{
    AddStringParam("galactic", "Input Galactic longitude,latitude coordinates (degrees)", 
                   "");
    if (!HasPar("delim")) {
        AddStringParam("delim", "Angle delimiter", "");
    }
}

/**********************************************************************//**
 * Add parameters for Ecliptic coordinates
 * 
 * Adds the following parameters:
 * ------------------------------
 *  "ecliptic"
 *      Ecliptic "longitude,latitude" (degrees)
 *************************************************************************/
inline
void CEExecOptions::AddEclipticPar(void)
{
    AddStringParam("ecliptic", "Ecliptic longitude,latitude (degrees)", "");
    if (!HasPar("delim")) {
        AddStringParam("delim", "Angle delimiter", "");
    }
}

/**********************************************************************//**
 * Add parameters for Observed coordinates
 * 
 * Adds the following parameters:
 * ------------------------------
 *  "observed"
 *      Observed "altitude,zenith" (degrees)
 *************************************************************************/
inline
void CEExecOptions::AddObservedPar(void)
{
    AddStringParam("observed", "Observed altitude,zenith (degrees)", "");
    if (!HasPar("delim")) {
        AddStringParam("delim", "Angle delimiter (single character)", "");
    }
}

/**********************************************************************//**
 * Generate an observer object
 *************************************************************************/
inline
CEObserver CEExecOptions::GenObserver(void)
{
    CEObserver obs(AsDouble("longitude"),
                   AsDouble("latitude"),
                   AsDouble("elevation"),
                   CEAngleType::DEGREES);
    obs.SetRelativeHumidity(AsDouble("humidity"));
    obs.SetPressure_hPa(AsDouble("pressure"));
    obs.SetWavelength_um(AsDouble("wavelength"));

    return obs;
}

/**********************************************************************//**
 * Defines the corrections terms from user supplied options
 *************************************************************************/
inline 
void CEExecOptions::SetCorrFiles(void)
{
    // Nutation
    if (AsString("nutation").size() > 0) {
        CppEphem::SetNutationFile(AsString("nutation"));
    }
    // TT - UT1 (historical)
    if (AsString("ttut1hist").size() > 0) {
        CppEphem::SetTtUt1HistFile(AsString("ttut1hist"));
    }
    // TT - UT1 (predictions)
    if (AsString("ttut1pred").size() > 0) {
        CppEphem::SetTtUt1PredFile(AsString("ttut1pred"));
    }
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

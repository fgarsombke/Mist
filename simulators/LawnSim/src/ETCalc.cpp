#pragma once

#include "LawnSimStd.h"

#include "ETCalc.h"

#include <cmath>

// Separate namespace for calculations
namespace Mist {
namespace FAO_ET {
// TODO: Modify based on current grass health


// ET_o reference evapotranspiration [mm day-1],
// R_n net radiation at the crop surface [MJ m-2 day-1],
// G soil heat flux density [MJ m-2 day-1],
// T mean daily air temperature at 2 m height [°C],
// u2 wind speed at 2 m height [m s-1],
// e_s saturation vapour pressure [kPa],
// e_a actual vapour pressure [kPa],
// e_s - e_a saturation vapour pressure deficit [kPa],
// D slope vapour pressure curve [kPa °C-1],
// g psychrometric constant [kPa °C-1].

// Constants for calculations
namespace Constants {
   // Specific heat of water at constant pressure
   const double c_p = .001013;

   // Latent heat of vaporization (water)
   const double lambda = 2.45;

   // ratio molecular weight of water vapour/dry air 
   const double epsilon_weight_ratio = 0.622;

   // STP Reference atmospheric pressure in KPa
   const double stp_atm = 101.325;

   // Exponent in atmospheric pressure computation
   const double gM_over_RL = 5.2558;

   // Albedo
   const double albedo = 0.23;

   // Dewpoint proportionality constants
   // See http://en.wikipedia.org/wiki/Dew_point for details
   const double dp_a = 0.6108;
   const double dp_b = 17.27;
   const double dp_c = 237.3;
}

using namespace Constants;

// Convert Celcius temperature to Kelvin
inline double CToK(double celciusTemp) {
   return celciusTemp + 273.15;
}

inline double CalculateAtmPressure(double altitude) {
   return stp_atm * exp(gM_over_RL * log(1 - altitude*(.0065/288.15)));
}

// Psychometric Constant
//    pressure is in KPa
inline double GammaFromPressure(double pressure) {
   return pressure*(c_p/(epsilon_weight_ratio*lambda));
}

// Temperature in Celcius
inline double SaturationVapourPressure_e_s(double temp) {
   return dp_a * exp((dp_b*temp)/(temp + dp_c));
}

// Temperature in Celcius
inline double delta_slope_e_s(double temp) {   
   return (4098*SaturationVapourPressure_e_s(temp))/((temp + dp_c)*(temp + dp_c));
}

// Dew point from temperature in celcius and relative humidity
inline double DewPoint(double temp, double rh) {
   double gam = log(rh) + temp*dp_b/(dp_c + temp);

   return dp_c*gam/(dp_b - gam);
}


inline bool IsLeapYear(unsigned int year) {
   if (year % 400 == 0) {
      return true;
   } else if (year % 100 == 0) {
      return false;
   } else if (year % 4 == 0) {
      return true;
   } else {
      return false;
   }
}


ETCalc::ETCalc(GeoLocale locale) {
   // Precompute solar parameters
}


// Calculates ET_o for the parameters given.

// Note that ET_o also depends on the previously
//    initialized location parameters.
//
// Temperature is in celcius
// Wind Speed is in m/s.
// 
double ETCalc::CalculateET_o(const ETCalcParameters &ETParams) const {
   return 0;
}




}
}
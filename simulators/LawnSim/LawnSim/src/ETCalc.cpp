#pragma once

#include "LawnSimStd.h"
#include <gsl/gsl_sf_exp.h>
#include <gsl/gsl_sf_log.h>

// Separate namespace for calculations
namespace ETCalc {
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



// Convert Celcius temperature to Kelvin
inline double CToK(double celciusTemp) {
   return celciusTemp + 273.15;
}

inline double CalculateAtmPressure(double altitude) {
   return 101.325*gsl_sf_exp(5.2558*gsl_sf_log(1-.0065*altitude/288.15));
}

// Indexed by 10m of altitude
inline double gamma_from_pressure(double pressure) {
   using namespace Constants;
   return (c_p*pressure)/(epsilon_weight_ratio*lambda);
}

// Temperature in Celcius
inline double saturation_vapour_pressure_e_s(double temp) {
   return 0.6108 * gsl_sf_exp((17.27*temp)/(temp + 237.3));
}

// Temperature in Celcius
inline double delta_slope_e_s(double temp) {
   return (4098*saturation_vapour_pressure_e_s(temp))/((temp + 237.3)*(temp + 237.3));
}

// Constants for calculations
namespace Constants {
   // Specific heat of water at constant pressure
   const double c_p = .001013;

   // Latent heat of vaporization (water)
   const double lambda = 2.45;

   // ratio molecular weight of water vapour/dry air 
   const double epsilon_weight_ratio = 0.622;
}


double ETCalc::CalculateET_o(){
   return 0;
}




}
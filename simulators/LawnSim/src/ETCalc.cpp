#include "LawnSimStd.h"

#include "ETCalc.h"

#include <cmath>

// Separate namespace for calculations
namespace Mist { namespace FAO_ET {
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
static inline double CToK(double celciusTemp) 
{
   return celciusTemp + 273.15;
}

static inline double CalculateAtmPressure(double altitude) 
{
   return stp_atm * exp(gM_over_RL * log(1 - altitude*(.0065/288.15)));
}

// Psychometric Constant
//    pressure is in KPa
static inline double GammaFromPressure(double pressure) 
{
   return pressure*(c_p/(epsilon_weight_ratio*lambda));
}

// Temperature in Celcius
// FAO Equation 3.11
static inline double SaturationVapourPressure(double temp) 
{
   return dp_a * exp((dp_b*temp)/(temp + dp_c));
}

// Mean Saturation Vapour Pressure
// FAO Equation 3.12
static inline double MeanSaturationVapourPressure(double minTemp, double maxTemp) 
{
   return (SaturationVapourPressure(minTemp) + SaturationVapourPressure(maxTemp))/2;
}

// Actual Vapour Pressure
// FAO Equation 3.17
static inline double ActualVapourPressure(double minTemp, double maxTemp, double minRH, double maxRH)
{
   return (minRH*SaturationVapourPressure(minTemp) + maxRH*SaturationVapourPressure(maxTemp))/2;
}


// Actual Vapour Pressure
// FAO Equation 3.19
static inline double ActualVapourPressure(double minTemp, double maxTemp, double meanRH)
{
   return meanRH*MeanSaturationVapourPressure(minTemp, maxTemp);
}

// Temperature in Celcius
// FAO Equation 3.13
static inline double SaturationVapourPressureSlope(double temp) 
{   
   return (4098*SaturationVapourPressure(temp))/((temp + dp_c)*(temp + dp_c));
}

static inline double NetRadiation()
{
   return 1;
}

static inline double SoilHFD()
{
   return 1;
}

// Dew point from temperature in celcius and relative humidity
static inline double DewPoint(double temp, double rh) {
   double gam = log(rh) + temp*dp_b/(dp_c + temp);

   return dp_c*gam/(dp_b - gam);
}


static inline bool IsLeapYear(unsigned int year) {
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
   // Implementation of Eq 6
   double delta = SaturationVapourPressureSlope(ETParams.avgTemp);
   double gamma = GammaFromPressure(ETParams.pressure);
   double R_n = NetRadiation();
   double G = SoilHFD();

   double u_2 = ETParams.windSpeed;
   double e_s = MeanSaturationVapourPressure(ETParams.minTemp, ETParams.maxTemp);
   double e_a = ActualVapourPressure(ETParams.minTemp, ETParams.maxTemp, ETParams.minRH, ETParams.maxRH);

   double num_left = 0.408*delta*(R_n-G);
   double num_right = gamma*(900/(CToK(ETParams.avgTemp)))*u_2*(e_s - e_a);
   double den = delta + gamma*(1+0.34*u_2);

   double ET_o = (num_left + num_right)/den;

   // Implementation of Eq 3


   return ET_o;
}

}}
#include "LawnSimStd.h"

#include "ETCalc.h"


#include <cmath>


// Separate namespace for calculations
namespace Mist { namespace FAO_ET {
   typedef double ET_float_t;

   
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
   const double PI  = 3.141592653589793238462;

   // Specific heat of water at constant pressure
   const ET_float_t c_p = .001013;

   // Latent heat of vaporization (water)
   const ET_float_t lambda = 2.45;

   // ratio molecular weight of water vapour/dry air 
   const ET_float_t epsilon_weight_ratio = 0.622;

   // STP Reference atmospheric pressure in KPa
   const ET_float_t stp_atm = 101.325;

   // Exponent in atmospheric pressure computation
   const ET_float_t gM_over_RL = 5.2558;

   // Albedo
   const ET_float_t albedo = 0.23;

   // Solar Constant
   const ET_float_t G_sc = 0.081672;

   const ET_float_t sigma = 4.903e-9;

   // Dewpoint proportionality constants
   // See http://en.wikipedia.org/wiki/Dew_point for details
   const ET_float_t dp_a = 0.6108;
   const ET_float_t dp_b = 17.27;
   const ET_float_t dp_c = 237.3;
}

using namespace Constants;

#ifdef _MSC_VER

#define __attribute__(_) 
#endif

typedef const ETCalcParameters &ETParams_t;
#define ETCalc_Func_decl(NAME, ...) static inline ET_float_t NAME(__VA_ARGS__) __attribute__((const))
#define ETCalc_Func_def(NAME, ...) static inline ET_float_t NAME(__VA_ARGS__) 

// FAO Equation 3.21
ETCalc_Func_decl(R_a_day, ETParams_t ETParams);

// FAO Equation 3.22
ETCalc_Func_decl(DegreesToRadians, ET_float_t degrees);

// FAO Equation 3.23
ETCalc_Func_decl(d_r, ETParams_t ETParams);

ETCalc_Func_decl(Omega_s,ETParams_t ETParams);

// FAO Equation 3.28
ETCalc_Func_decl(R_a_hourly, ETParams_t ETParams);

// FAO Equation 3.29
ETCalc_Func_decl(Omega_1, ETParams_t ETParams);

// FAO Equation 3.30
ETCalc_Func_decl(Omega_2, ETParams_t ETParams);

// FAO Equation 3.31
ETCalc_Func_decl(Omega, ETParams_t ETParams);

// FAO Equation 3.32
ETCalc_Func_decl(S_c, ETParams_t ETParams);

// FAO Equation 3.35
ETCalc_Func_decl(R_s, ETParams_t ETParams);

ETCalc_Func_decl(a_s);
ETCalc_Func_decl(b_s);

// Convert Celcius temperature to Kelvin
static inline ET_float_t CToK(double celciusTemp) 
{
   return celciusTemp + 273.15;
}

// Number of the day in the year
static inline int J(ETParams_t ETParams)
{
   return ETParams.interval.begin().date().day_of_year();
}

ETCalc_Func_def(CalculateAtmPressure, ET_float_t altitude) 
{
   return stp_atm * exp(gM_over_RL * log(1 - altitude*(.0065/288.15)));
}

// Pressure from Altitude
// FAO Equation 3.7
ETCalc_Func_def(PressureFromAltitude, ET_float_t altitude) 
{
   return 101.3*pow((293-0.0065*altitude)/293, 5.26);
}

// Psychometric Constant
//    pressure is in KPa
// FAO Equation 3.8
ETCalc_Func_def(GammaFromPressure, ET_float_t pressure) 
{
   return pressure*(c_p/(epsilon_weight_ratio*lambda));
}

// Temperature in Celcius, RH in fraction
// FAO Equation 3.9 
ETCalc_Func_def(Mean, ET_float_t min, ET_float_t max)
{
   return (max - min)/2;
}

// Temperature in Celcius
// FAO Equation 3.11
ETCalc_Func_def(SaturationVapourPressure, ET_float_t temp) 
{
   return dp_a * exp((dp_b*temp)/(temp + dp_c));
}

// Mean Saturation Vapour Pressure
// FAO Equation 3.12
ETCalc_Func_def(MeanSaturationVapourPressure, ETParams_t ETParams)
{
   return (SaturationVapourPressure(ETParams.minTemp) + SaturationVapourPressure(ETParams.maxTemp))/2;
}

// Temperature in Celcius
// FAO Equation 3.13
ETCalc_Func_def(SaturationVapourPressureSlope, ET_float_t temp) 
{   
   return (4098*SaturationVapourPressure(temp))/((temp + dp_c)*(temp + dp_c));
}

// Actual Vapour Pressure
// FAO Equation 3.17
ETCalc_Func_def(e_a, ETParams_t ETParams)
{
   return (ETParams.minRH*SaturationVapourPressure(ETParams.minTemp) + 
      ETParams.maxRH*SaturationVapourPressure(ETParams.maxTemp)
   )/2.0;
}

// Actual Vapour Pressure
// FAO Equation 3.19
ETCalc_Func_def(e_a_RHMean, ETParams_t ETParams)
{
   return Mean(ETParams.minRH, ETParams.maxRH)*MeanSaturationVapourPressure(ETParams);
}

// Extraterrestrial radiation for daily periods
// NOTE: This is only valid for time periods of a day
// FAO Equation 3.21
ETCalc_Func_def(R_a_day, ETParams_t ETParams)
{
   ET_float_t w_s = Omega_s(ETParams);
   ET_float_t delta = d_r(ETParams);

   return (24*60/PI)*G_sc*d_r(ETParams)*(
      w_s*sin(DegreesToRadians(ETParams.locale.latitude()))*sin(delta) + 
      cos(DegreesToRadians(ETParams.locale.latitude()))*cos(delta)*sin(w_s)
   );
}

// Latitude in Radians
// FAO Equation 3.22
ETCalc_Func_def(DegreesToRadians, ET_float_t degrees)
{
   return degrees*PI/180.0;
}

// Inverse relative distance from the Earth to the Sun
// FAO Equation 3.23
ETCalc_Func_def(d_r, ETParams_t ETParams)
{
   return 1.0 + 0.033*cos(2*PI*J(ETParams)/365.0);
}

// Solar declination
// FAO Equation 3.24
ETCalc_Func_def(SolarDeclination,ETParams_t ETParams)
{
   return 0.409*sin(J(ETParams)*2.0*PI/365.0 - 1.39);
}

// Sunset Hour Angle
// FAO Equation 3.25
ETCalc_Func_def(Omega_s,ETParams_t ETParams)
{
   return acos(-tan(DegreesToRadians(ETParams.locale.latitude()))*tan(SolarDeclination(ETParams)));
}

// Extraterrestrial radiation for hourly or shorter periods
// NOTE: This function is for less than a day only
// FAO Equation 3.28
ETCalc_Func_def(R_a_hourly, ETParams_t ETParams)
{
   ET_float_t phi = DegreesToRadians(ETParams.locale.latitude()); 
   ET_float_t delta = SolarDeclination(ETParams);

   ET_float_t w_1 = Omega_1(ETParams);
   ET_float_t w_2 = Omega_2(ETParams);

   return (12*60/PI)*G_sc*d_r(ETParams)*(
      ((w_2 - w_1)*sin(phi)*sin(delta) +
       cos(phi)*cos(delta)*(sin(w_2) - sin(w_1)))
   );
}

// FAO Equation 3.29
ETCalc_Func_def(Omega_1, ETParams_t ETParams)
{
   return Omega(ETParams) - 
      (PI/24.0)*ETParams.interval.length().total_milliseconds()/(1000*60*60);
}

// FAO Equation 3.30
ETCalc_Func_def(Omega_2, ETParams_t ETParams)
{
   return Omega(ETParams) + 
      (PI/24.0)*ETParams.interval.length().total_milliseconds()/(1000*60*60);
}

// FAO Equation 3.31
ETCalc_Func_def(Omega, ETParams_t ETParams)
{
   pt::time_duration half = ETParams.interval.length()/2;
   pt::ptime midpoint = ETParams.interval.begin() + half;
   pt::time_duration dayTime = midpoint.time_of_day();
   ET_float_t t = dayTime.hours() + dayTime.minutes()/60.0 + dayTime.seconds()/(60.0*60.0);

   return PI*((1/12.0)*(t + (2/30.0)*(ETParams.locale.Lz() - ETParams.locale.longitude()) + S_c(ETParams))-1);
}

// Seasonal correction for solar time
// FAO Equation 3.32
ETCalc_Func_def(S_c, ETParams_t ETParams)
{
   // FAO Equation 3.33
   ET_float_t b = (2*PI/364.0)*(J(ETParams)-81);

   return 0.1645*sin(2*b)-0.1255*cos(b)-0.025*sin(b);
}

// Daylight hours
// NOTE: This is only valid for time periods of a day
// FAO Equation 3.34
ETCalc_Func_def(N_duration, ETParams_t ETParams)
{
   return (24.0/PI)*Omega_s(ETParams);
}

// Solar Radiation
// TODO: Figure out day/hourly discrepency
// FAO Equation 3.35
ETCalc_Func_def(R_s, ETParams_t ETParams)
{
   // TODO: Get n_duration
   ET_float_t n_duration;

   return (a_s() + b_s()*(n_duration/N_duration(ETParams)))*R_a_hourly(ETParams);
}

// Uncalibrated always
ETCalc_Func_def(a_s)
{
   // TODO: Always use default?
   return 0.25;
}

// Uncalibrated always
ETCalc_Func_def(b_s)
{
   // TODO: Always use default?
   return 0.50;
}

// Net solar or net shortwave radiation
// TODO: Figure out day/hourly discrepency
// FAO Equation 3.37
ETCalc_Func_def(R_so, ETParams_t ETParams) 
{
   return (0.75 + 2e-5 * ETParams.locale.elevation())*R_a_hourly(ETParams);
}

// Net solar or net shortwave radiation
// FAO Equation 3.38
ETCalc_Func_def(R_ns, ETParams_t ETParams) 
{
   return (1.0 - Constants::albedo)*R_s(ETParams);
}

// Net longwave radiation
// TODO: Correct for the 24 hour temperature period requirement
// FAO Equation 3.39
ETCalc_Func_def(R_nl, ETParams_t ETParams) 
{
   return sigma*((pow(ETParams.maxTemp, 4) + pow(ETParams.minTemp,4))/2.0)*
      (0.34-0.14*sqrt(e_a(ETParams))*
      (1.35*(R_s(ETParams)/R_so(ETParams)) - 0.35)
   );
}

// Difference between incoming and outgoing radiations
// FAO Equation 3.40
ETCalc_Func_def(NetRadiation, ETParams_t ETParams)
{
   return R_ns(ETParams) - R_nl(ETParams);
}

ETCalc_Func_def(SoilHFD, ETParams_t ETParams)
{
   throw std::runtime_error("Not Implemented");
}

// Dew point from temperature in celcius and relative humidity
ETCalc_Func_def(DewPoint, double temp, double rh) {
   double gam = log(rh) + temp*dp_b/(dp_c + temp);

   return dp_c*gam/(dp_b - gam);
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
double ETCalc::CalculateET_o(ETParams_t ETParams) const {
   // Implementation of Eq 2.6
   ET_float_t delta = SaturationVapourPressureSlope(ETParams.avgTemp);
   ET_float_t gamma = GammaFromPressure(ETParams.pressure);
   ET_float_t R_n = NetRadiation(ETParams);
   ET_float_t G = SoilHFD(ETParams);

   ET_float_t u_2 = ETParams.windSpeed;
   ET_float_t e_s = MeanSaturationVapourPressure(ETParams);

   ET_float_t num_left = 0.408*delta*(R_n-G);
   ET_float_t num_right = gamma*(900/(CToK(ETParams.avgTemp)))*u_2*(e_s - e_a(ETParams));
   ET_float_t den = delta + gamma*(1+0.34*u_2);

   ET_float_t ET_o = (num_left + num_right)/den;

   // TODO: Implementation of Eq 2.3


   return ET_o;
}

}}
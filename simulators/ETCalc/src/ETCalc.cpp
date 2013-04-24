#include "ETCalc.h"

#include <cmath>


// Separate namespace for calculations
namespace Mist { namespace ETCalc {   
   // TODO: Modify based on current grass health


// ET_o reference evapotranspiration [mm day-1],
// R_n net radiation at the crop surface [MJ m-2 day-1],
// G soil heat flux density [MJ m-2 day-1],
// T mean daily air temperature at 2 m height [°C],
// u2 wind speed at 2 m height [m s-1],
// e_s saturation vapour Pressure() [kPa],
// e_a actual vapour Pressure() [kPa],
// e_s - e_a saturation vapour Pressure() deficit [kPa],
// D slope vapour Pressure() curve [kPa °C-1],
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

#define ETCalc_Func_def(NAME, ...) inline ET_float_t ETCalc::NAME(__VA_ARGS__) const

// Convert Celcius temperature to Kelvin
static inline ET_float_t CToK(double celciusTemp) 
{
   return celciusTemp + 273.15;
}

// Number of the day in the year
static inline int J(ETParam_t ETParam)
{
   // TODO: Should this be the beginning or the middle??
   return ETParam.Interval().begin().date().day_of_year();
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
//    Pressure() is in KPa
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

// Saturation Vapour Pressure
// Temperature in Celcius
// FAO Equation 3.11
ETCalc_Func_def(e_o, ET_float_t temp) 
{
   return dp_a * exp((dp_b*temp)/(temp + dp_c));
}

// Mean Saturation Vapour Pressure
// FAO Equation 3.12
ETCalc_Func_def(MeanSaturationVapourPressure, ETParam_t ETParam)
{
   return (e_o(ETParam.MinTemp()) + e_o(ETParam.MaxTemp()))/2;
}

// Temperature in Celcius
// FAO Equation 3.13
ETCalc_Func_def(SaturationVapourPressureSlope, ET_float_t temp) 
{   
   return (4098*e_o(temp))/((temp + dp_c)*(temp + dp_c));
}

// Actual Vapour Pressure
// FAO Equation 3.17
ETCalc_Func_def(e_a, ETParam_t ETParam)
{
   return (ETParam.MinRH()*e_o(ETParam.MinTemp()) + 
      ETParam.MaxRH()*e_o(ETParam.MaxTemp())
   )/2.0;
}

// Actual Vapour Pressure
// FAO Equation 3.19
ETCalc_Func_def(e_a_RHMean, ETParam_t ETParam)
{
   return Mean(ETParam.MinRH(), ETParam.MaxRH())*MeanSaturationVapourPressure(ETParam);
}

// Extraterrestrial radiation for daily periods
// NOTE: This is only valid for time periods of a day or longer
// FAO Equation 3.21
ETCalc_Func_def(R_a_days, ETParam_t ETParam)
{
   ET_float_t w_s = Omega_s(ETParam);
   ET_float_t delta = d_r(ETParam);

   return (24*60/PI)*G_sc*d_r(ETParam)*(
      w_s*sin(DegreesToRadians(locale_.latitude()))*sin(delta) + 
      cos(DegreesToRadians(locale_.latitude()))*cos(delta)*sin(w_s)
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
ETCalc_Func_def(d_r, ETParam_t ETParam)
{
   return 1.0 + 0.033*cos(2*PI*J(ETParam)/365.0);
}

// Solar declination
// FAO Equation 3.24
ETCalc_Func_def(SolarDeclination,ETParam_t ETParam)
{
   return 0.409*sin(J(ETParam)*2.0*PI/365.0 - 1.39);
}

// Sunset Hour Angle
// Modified slightly to include refraction
// FAO Equation 3.25
ETCalc_Func_def(Omega_s,ETParam_t ETParam)
{
   double phi = DegreesToRadians(locale_.latitude());
   double delta = SolarDeclination(ETParam);

   return acos(
            (sin(-0.0144862327915529) - sin(phi)*sin(delta))
            /
            (cos(phi)*cos(delta))
          );
}

// Extraterrestrial radiation for hourly or shorter periods
// NOTE: This function is for less than a day only
// FAO Equation 3.28
ETCalc_Func_def(R_a_hours, ETParam_t ETParam)
{
   ET_float_t phi = DegreesToRadians(locale_.latitude()); 
   ET_float_t delta = SolarDeclination(ETParam);

   ET_float_t w_1 = Omega_1(ETParam);
   ET_float_t w_2 = Omega_2(ETParam);

   return (12*60/PI)*G_sc*d_r(ETParam)*(
      ((w_2 - w_1)*sin(phi)*sin(delta) +
       cos(phi)*cos(delta)*(sin(w_2) - sin(w_1)))
   );
}

// FAO Equation 3.29
ETCalc_Func_def(Omega_1, ETParam_t ETParam)
{
   return Omega(ETParam) - 
      (PI/24.0)*ETParam.Interval().length().total_milliseconds()/(1000*60*60);
}

// FAO Equation 3.30
ETCalc_Func_def(Omega_2, ETParam_t ETParam)
{
   return Omega(ETParam) + 
      (PI/24.0)*ETParam.Interval().length().total_milliseconds()/(1000*60*60);
}

// FAO Equation 3.31
ETCalc_Func_def(Omega, ETParam_t ETParam)
{
   pt::time_duration half = ETParam.Interval().length()/2;
   pt::ptime midpoint = ETParam.Interval().begin() + half;
   pt::time_duration dayTime = midpoint.time_of_day();
   ET_float_t t = dayTime.hours() + dayTime.minutes()/60.0 + dayTime.seconds()/(60.0*60.0);

   return PI*((1/12.0)*(t + (2/30.0)*(locale_.Lz() - locale_.longitude()) + S_c(ETParam))-1);
}

// Seasonal correction for solar time
// FAO Equation 3.32
ETCalc_Func_def(S_c, ETParam_t ETParam)
{
   // FAO Equation 3.33
   ET_float_t b = (2*PI/364.0)*(J(ETParam)-81);

   return 0.1645*sin(2*b)-0.1255*cos(b)-0.025*sin(b);
}

// Solar Radiation
// FAO Equation 3.35ii
ETCalc_Func_def(R_s_days, ETParam_t ETParam)
{
   return (a_s() + b_s()*ETParam.SunlightFraction())*R_a_days(ETParam);
}

// Solar Radiation
// TODO: Figure out day/hourly discrepency
// FAO Equation 3.35
ETCalc_Func_def(R_s_hours, ETParam_t ETParam)
{
   return (a_s() + b_s()*ETParam.SunlightFraction())*R_a_hours(ETParam);
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
// FAO Equation 3.37ii
ETCalc_Func_def(R_so_days, ETParam_t ETParam) 
{
   return (0.75 + 2e-5 * locale_.elevation())*R_a_days(ETParam);
}

// Net solar or net shortwave radiation
// FAO Equation 3.37
ETCalc_Func_def(R_so_hours, ETParam_t ETParam) 
{
   return (0.75 + 2e-5 * locale_.elevation())*R_a_hours(ETParam);
}

// Net solar or net shortwave radiation
// FAO Equation 3.38ii
ETCalc_Func_def(R_ns_days, ETParam_t ETParam) 
{
   return (1.0 - Constants::albedo)*R_s_days(ETParam);
}

// Net solar or net shortwave radiation
// FAO Equation 3.38
ETCalc_Func_def(R_ns_hours, ETParam_t ETParam) 
{
   return (1.0 - Constants::albedo)*R_s_hours(ETParam);
}

// Net longwave radiation
// FAO Equation 3.39ii
ETCalc_Func_def(R_nl_days, ETParam_t ETParam) 
{
   return sigma*((pow(ETParam.MaxTemp(), 4) + pow(ETParam.MinTemp(),4))/2.0)*
      (0.34-0.14*sqrt(e_a(ETParam))*
      (1.35*(R_s_days(ETParam)/R_so_days(ETParam)) - 0.35)
   );
}

// Net longwave radiation
// FAO Equation 3.39
ETCalc_Func_def(R_nl_hours, ETParam_t ETParam) 
{
   return sigma*((pow(ETParam.MaxTemp(), 4) + pow(ETParam.MinTemp(),4))/2.0)*
      (0.34-0.14*sqrt(e_a(ETParam))*
      (1.35*(R_s_hours(ETParam)/R_so_hours(ETParam)) - 0.35)
   );
}

// Difference between incoming and outgoing radiations
// FAO Equation 3.40
ETCalc_Func_def(NetRadiation, ET_float_t R_ns, ET_float_t R_nl)
{
   return R_ns - R_nl;
}

// Soil Heat Flux
// FAO Equation 3.42
ETCalc_Func_def(G_days)
{
   return 0;
}

// Soil Heat Flux
// FAO Equation 3.43/3.44
ETCalc_Func_def(G_months, ETParam_t ETParam)
{
   // The actual FAO equation is a little different, but since
   //    theirs is a very rough approximation I think that we can
   //    just use the same rough model without problems
   return (ETParam.Interval().length().total_seconds()*(0.07/(60*60*24*30.44)))*
      (ETParam.EndTemp() - ETParam.StartTemp());
}

// Soil Heat Flux
// FAO Equation 3.45
ETCalc_Func_def(G_hours_day, ET_float_t Rn)
{
   return 0.1 * Rn;
}

// Soil Heat Flux
// FAO Equation 3.46
ETCalc_Func_def(G_hours_night, ET_float_t Rn)
{
   return 0.5 * Rn;
}

// Dew point from temperature in celcius and relative humidity
ETCalc_Func_def(DewPoint, double temp, double rh) 
{
   double gam = log(rh) + temp*dp_b/(dp_c + temp);

   return dp_c*gam/(dp_b - gam);
}

// Calculates ET_o for the parameters given.
//
// The function processes
//
// Note that ET_o also depends on the previously
//    initialized location parameters.
//
// Temperature is in celcius
// Wind Speed is in m/s.
// 
ET_float_t ETCalc::CalculateET_0(ETParam_t ETParam) const
{		
	// Implementation of Eq 2.6
	ET_float_t delta = SaturationVapourPressureSlope(ETParam.AvgTemp());
	ET_float_t gamma = GammaFromPressure(ETParam.AvgPressure());
	ET_float_t u_2 = ETParam.AvgWindSpeed();
	ET_float_t R_n;
	ET_float_t G;
	ET_float_t e_s = MeanSaturationVapourPressure(ETParam);

	R_n = NetRadiation(R_ns_hours(ETParam), R_nl_hours(ETParam));
	switch(ETParam.LengthType()) {
		case ETCalcLengthType::HoursDay:
			R_n = NetRadiation(R_ns_hours(ETParam), R_nl_hours(ETParam));
			G = G_hours_day(R_n);
			break;
		case ETCalcLengthType::HoursNight:
			R_n = NetRadiation(R_ns_hours(ETParam), R_nl_hours(ETParam));
			G = G_hours_night(R_n);
			break;
		case ETCalcLengthType::Days:
			R_n = NetRadiation(R_ns_days(ETParam), R_nl_days(ETParam));
			G = G_days();
			break;
		case ETCalcLengthType::Months:
			R_n = NetRadiation(R_ns_days(ETParam), R_nl_days(ETParam));
			G = G_months(ETParam);
			break;
		default:
				return 0;
	}

	ET_float_t num_left = 0.408*delta*(R_n-G);
	ET_float_t num_right = gamma*(900/(CToK(ETParam.AvgTemp())))*u_2*(e_s - e_a(ETParam));
	ET_float_t den = delta + gamma*(1+0.34*u_2);
	ET_float_t ET_o = (num_left + num_right)/den;

	// TODO: Implementation of Eq 2.3

   // Units of mm/day
	return ET_o;
}

}}
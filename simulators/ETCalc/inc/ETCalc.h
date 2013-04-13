#pragma once

#include "MistStd.h"
#include "ETCalcParameters.h"

namespace Mist { namespace ETCalc {

class ETCalc {
public:
	ETCalc(double latitude, double longitude)
		:	locale_(GeoLocale(latitude, longitude))
	{
	}

   explicit ETCalc(GeoLocale locale);


   double CalculateET_o(ETParam_t ETParams) const;

private:
   GeoLocale locale_;


   #define ETCalc_Func_decl(NAME, ...) inline ET_float_t NAME(__VA_ARGS__) const __attribute__((const))

   ETCalc_Func_decl(CalculateAtmPressure, ET_float_t altitude);

   // FAO Equation 3.7
   ETCalc_Func_decl(PressureFromAltitude, ET_float_t altitude);

   // FAO Equation 3.8
   ETCalc_Func_decl(GammaFromPressure, ET_float_t pressure);

   // FAO Equation 3.9 
   ETCalc_Func_decl(Mean, ET_float_t min, ET_float_t max);

   // FAO Equation 3.11
   ETCalc_Func_decl(e_o, ET_float_t temp);

   // FAO Equation 3.12
   ETCalc_Func_decl(MeanSaturationVapourPressure, ETParam_t ETParams);

   // FAO Equation 3.13
   ETCalc_Func_decl(SaturationVapourPressureSlope, ET_float_t temp);

   // FAO Equation 3.17
   ETCalc_Func_decl(e_a, ETParam_t ETParams);

   // FAO Equation 3.19
   ETCalc_Func_decl(e_a_RHMean, ETParam_t ETParams);

   // FAO Equation 3.21
   ETCalc_Func_decl(R_a_days, ETParam_t ETParams);

   // FAO Equation 3.22
   ETCalc_Func_decl(DegreesToRadians, ET_float_t degrees);

   // FAO Equation 3.23
   ETCalc_Func_decl(d_r, ETParam_t ETParams);

   // FAO Equation 3.24
   ETCalc_Func_decl(SolarDeclination,ETParam_t ETParams);

   // FAO Equation 3.25
   ETCalc_Func_decl(Omega_s,ETParam_t ETParams);

   // FAO Equation 3.28
   ETCalc_Func_decl(R_a_hours, ETParam_t ETParams);

   // FAO Equation 3.29
   ETCalc_Func_decl(Omega_1, ETParam_t ETParams);

   // FAO Equation 3.30
   ETCalc_Func_decl(Omega_2, ETParam_t ETParams);

   // FAO Equation 3.31
   ETCalc_Func_decl(Omega, ETParam_t ETParams);

   // FAO Equation 3.32
   ETCalc_Func_decl(S_c, ETParam_t ETParams);

   // FAO Equation 3.34
   ETCalc_Func_decl(N_duration, ETParam_t ETParams);

	// FAO Equation 3.35ii
	ETCalc_Func_decl(R_s_days, ETParam_t ETParam);

   // FAO Equation 3.35
   ETCalc_Func_decl(R_s_hours, ETParam_t ETParams);

	// FAO Equation 3.37ii
	ETCalc_Func_decl(R_so_days, ETParam_t ETParam);

   // FAO Equation 3.37
   ETCalc_Func_decl(R_so_hours, ETParam_t ETParams);

	// FAO Equation 3.38ii
	ETCalc_Func_decl(R_ns_days, ETParam_t ETParam);

   // FAO Equation 3.38
   ETCalc_Func_decl(R_ns_hours, ETParam_t ETParams); 

	// FAO Equation 3.39ii
   ETCalc_Func_decl(R_nl_days, ETParam_t ETParams);

   // FAO Equation 3.39
   ETCalc_Func_decl(R_nl_hours, ETParam_t ETParams);

   // FAO Equation 3.40
   ETCalc_Func_decl(NetRadiation, ET_float_t R_ns, ET_float_t R_nl);

   // FAO Equation 3.42
   ETCalc_Func_decl(G_days);

   // FAO Equation 3.43/3.44
   ETCalc_Func_decl(G_months, ETParam_t ETParams);

   // FAO Equation 3.45
   ETCalc_Func_decl(G_hours_day,  ET_float_t Rn);
   
   // FAO Equation 3.46
   ETCalc_Func_decl(G_hours_night,  ET_float_t Rn);

   ETCalc_Func_decl(DewPoint, double temp, double rh);

   ETCalc_Func_decl(a_s);
   ETCalc_Func_decl(b_s);

};

}}
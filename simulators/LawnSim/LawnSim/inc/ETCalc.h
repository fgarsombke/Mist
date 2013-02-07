#pragma once

#include "LawnSimStd.h"
#include "GeoLocale.h"

namespace FAO_ET {

struct ETCalcParameters {
   const double windSpeed;    // In m/s
   const double humidity;     // In fractional form (<1)
   const double pressure;     // In KPa

   const pt::ptime intervalStart;
   const double startTemp;    // In Celcius

   const pt::ptime intervalEnd;
   const double endTemp;      // In Celcius
};


class ETCalc {
public:
   ETCalc(LawnSim::GeoLocale locale);


   double CalculateET_o(const ETCalcParameters &ETParams) const;

};

}
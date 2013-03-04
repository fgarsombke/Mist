#pragma once

#include "LawnSimStd.h"
#include "GeoLocale.h"

namespace Mist { namespace FAO_ET {

struct ETCalcParameters {
   double windSpeed;    // In m/s

   double maxRH;        // In fractional form (<1)
   double minRH;

   double humidity;     
   double pressure;     // In KPa

   pt::ptime intervalStart;
   pt::ptime intervalEnd;

   double startTemp;    // In Celcius
   double minTemp;
   double avgTemp;
   double endTemp;      // In Celcius
   double maxTemp;
};


class ETCalc {
public:
   ETCalc(GeoLocale locale);


   double CalculateET_o(const ETCalcParameters &ETParams) const;

};

}}
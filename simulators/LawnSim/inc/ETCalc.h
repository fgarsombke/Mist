#pragma once

#include "LawnSimStd.h"
#include "GeoLocale.h"

namespace Mist { namespace FAO_ET {

struct ETCalcParameters {
   ETCalcParameters()
      : interval(pt::ptime(), pt::ptime())
   {
   }

   GeoLocale locale;
   
   double windSpeed;    // In m/s

   double maxRH;        // In fractional form (<1)
   double minRH;

   double humidity;     // Fractional 
   double pressure;     // In KPa

   pt::time_period interval;

   double startTemp;    // In Celcius
   double minTemp;
   double avgTemp;
   double endTemp;     
   double maxTemp;
};


class ETCalc {
public:
   ETCalc(GeoLocale locale);


   double CalculateET_o(const ETCalcParameters &ETParams) const;

};

}}
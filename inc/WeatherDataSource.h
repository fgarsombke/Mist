#pragma once
#include "LawnSimStd.h"
#include "GeoLocale.h"
#include "WeatherData.h"

namespace Mist {

class WeatherDataSource {
public:

   
   WeatherData GetData(GeoLocale locale, pt::time_period period) {
      // TODO: Implement

      //Create URL request to weather API - parameters are lat, long & time.
      //the API initially will just find the closest weather station and get
      //the 'WeatherData' closest to the requested time
      
      return WeatherData();
   }

private:



};



}


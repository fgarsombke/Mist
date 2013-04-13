#pragma once
#include "MistStd.h"

namespace Mist {

class WeatherDataSource;
typedef std::unique_ptr<WeatherDataSource> uPtrWeatherDataSource;
typedef std::shared_ptr<WeatherDataSource> sPtrWeatherDataSource;

class WeatherDataSource {
public:
   virtual WeatherData GetWeatherData(GeoLocale locale, pt::time_period period, unsigned int timeout = -1) = 0;
   virtual ~WeatherDataSource() { };
};


}
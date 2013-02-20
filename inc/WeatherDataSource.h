#pragma once
#include "MistStd.h"
#include "GeoLocale.h"
#include "WeatherData.h"

using std::string;
using std::vector;
using std::ostream;

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

   int GetHtml(string &host_,string &port_, string url_path, ostream &out_, vector<string> &headers, unsigned int timeout) const;

private:
   


};



}


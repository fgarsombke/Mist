#pragma once
#include "MistStd.h"

#include "WeatherData.h"
#include "HTMLSource.h"
#include "ScheduleSource.h"
#include "MistSchedule.h"

namespace Mist {

class MistDataSource;
typedef std::unique_ptr<MistDataSource> uPtrMistDataSource;
typedef std::shared_ptr<MistDataSource> sPtrMistDataSource;

class MistDataSource : public ScheduleSource, public WeatherDataSource {
public:
   static sPtrMistDataSource GetDefaultDataSource();

   virtual WeatherData GetWeatherData(GeoLocale locale, pt::time_period period) override;
   virtual MistSchedule GetSchedule(product_id_t id, unsigned int timeout = -1) const override;
private:
   explicit MistDataSource(const std::string &host)
      : data_source_(host, "8080")
   {
   }

   // Reference class only
   MistDataSource &operator=(const MistDataSource &other);
   MistDataSource(const MistDataSource &other);

   HTMLSource data_source_;
};


}
#pragma once
#include "MistStd.h"

#include "WeatherData.h"
#include "HTMLSource.h"
#include "ScheduleSource.h"
#include "MistSchedule.h"
#include "FeedbackSink.h"

namespace Mist {

class MistDataSource;
typedef std::unique_ptr<MistDataSource> uPtrMistDataSource;
typedef std::shared_ptr<MistDataSource> sPtrMistDataSource;

class MistDataSource 
   : public ScheduleSource, public WeatherDataSource, public FeedbackSink
{
public:
   static sPtrMistDataSource GetDefaultDataSource();
	static sPtrMistDataSource GetNullSource();

   virtual int SubmitFeedback(product_id_t id, 
                              pt::ptime intervalEnd, 
                              const std::vector<FeedbackList_t> feedback, 
                              unsigned int timeout = -1) const override;

   virtual WeatherData GetWeatherData(GeoLocale locale, pt::time_period period, unsigned int timeout = -1) override;
   virtual MistSchedule GetSchedule(product_id_t id, unsigned int timeout = -1) const override;

protected:
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
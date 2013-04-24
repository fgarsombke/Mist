#pragma once
#include "MistStd.h"

#include "MistDataSource.h"
#include "PAL.h"

#ifdef _DEBUG_DATA

namespace Mist {

class DbgDataSource : public MistDataSource
{
public:
	DbgDataSource()
		: MistDataSource("")
	{}

   virtual int SubmitFeedback(product_id_t id, 
										const std::vector<FeedbackList_t> feedback, 
										unsigned int timeout = -1) const 
	{
		SleepForMS(FeedbackWaitTime);
		return 0;
	}

   virtual WeatherData GetWeatherData(GeoLocale locale, 
												  pt::time_period period, 
												  unsigned int timeout = -1) 
	{
		SleepForMS(WeatherDataWaitTime);

      return WeatherData::CreateDebug();
	}

   virtual MistSchedule GetSchedule(product_id_t id, 
												unsigned int timeout = -1) const 
	{
		SleepForMS(ScheduleWaitTime);
		return MistSchedule();
	}

private:
   // Reference class only
   DbgDataSource &operator=(const DbgDataSource &other);
   DbgDataSource(const MistDataSource &other);

	static const int FeedbackWaitTime = 100;
	static const int WeatherDataWaitTime = 100;
	static const int ScheduleWaitTime = 100;
};


}

#endif
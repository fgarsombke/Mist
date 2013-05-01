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
                              pt::ptime intervalEnd, 
                              const std::vector<FeedbackList_t> feedback, 
                              unsigned int timeout = -1) const override
	{
		SleepForMS(FeedbackWaitTime);
		return 0;
	}

   virtual WeatherData GetWeatherData(GeoLocale locale, 
												  pt::time_period period, 
												  unsigned int timeout = -1) override
	{
		SleepForMS(WeatherDataWaitTime);

      return WeatherData::CreateDebug();
	}

   virtual MistSchedule GetSchedule(product_id_t id, 
												unsigned int timeout = -1) const override
	{
		SleepForMS(ScheduleWaitTime);
		return MistSchedule();
	}

    virtual product_id_t AddDevice(user_id_t userID, 
                          GeoLocale locale,
                          size_t numZones,
                          unsigned int timeout) const override
   {
      SleepForMS(AddDeviceWaitTime);
      return 1;
   }

private:
   // Reference class only
   DbgDataSource &operator=(const DbgDataSource &other);
   DbgDataSource(const MistDataSource &other);

	static const int FeedbackWaitTime = 100;
	static const int WeatherDataWaitTime = 100;
	static const int ScheduleWaitTime = 100;
   static const int AddDeviceWaitTime = 100;
};


}

#endif
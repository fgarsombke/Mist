#include "MistStd.h"
#include "MistDataSource.h"

#include "DbgDataSource.h"
#include "PAL.h"

using std::string;
using std::ostream;
using std::vector;


namespace Mist {
   static const std::string srcURL("wireless-128-62-91-67.public.utexas.edu");

   static const std::string scheduleStr("/api/schedule?deviceID=");
   static const std::string feedbackStr("/api/feedback?simulator=1");
	static const char* weatherFmtStr = "/api/weather?latitude=%g&longitude=%g&begin=%llu&end=%llu";

   static const char* addDeviceFmtStr = "/api/device?userID=%llu&initTime=%llu&latitude=%g&longitude=%g&numZones=%llu";

   sPtrMistDataSource MistDataSource::GetDefaultDataSource()
   {
      return sPtrMistDataSource(new MistDataSource(srcURL));
   }

	sPtrMistDataSource MistDataSource::GetNullSource()
	{
		return sPtrMistDataSource(new DbgDataSource());
	}

   int MistDataSource::SubmitFeedback(product_id_t id, 
                                      pt::ptime intervalEnd, 
                                      std::vector<FeedbackList_t> feedback, 
                                      unsigned int timeout) const
   {
      std::vector<std::string> headers;
      std::string fbStr = Feedback::PackFeedbackJson(feedback, id, intervalEnd);
      std::stringstream garbage;

      std::cout << "FEEDBACK:\n" << fbStr << std::endl;

      int result = data_source_.PostHtml(feedbackStr, "application/json", fbStr, garbage, headers, timeout);
      if (result < 200 || result >= 300) {
         throw std::logic_error(std::string("Error Posting Feedback: HTTP ") + std::to_string(result) + "\n");
      } else {
         return 0;
      }
   }

   WeatherData MistDataSource::GetWeatherData(GeoLocale locale, pt::time_period period,  unsigned int timeout) 
   {
      char urlbuff[255];
      std::vector<std::string> headers;
      std::stringstream weather_out;

      sprintf(urlbuff, weatherFmtStr, 
         locale.latitude(), 
         locale.longitude(),
         GetEpochTime(period.begin()), 
         GetEpochTime(period.end())
      );

      std::string getWeatherStr(urlbuff);
      std::cout << "GET WEATHER: " << getWeatherStr << std::endl;
      
      int result = data_source_.GetHtml(getWeatherStr, weather_out, headers, timeout);
      if (result != 200) {
         throw std::logic_error(std::string("Error Getting Weather Data: HTML ") + std::to_string(result) + "\n");
      } else {
         std::cout << "GOT WEATHER\n" << weather_out.str() << std::endl;
         return WeatherData::CreateFromJson(weather_out);
      }
   }

   MistSchedule MistDataSource::GetSchedule(product_id_t id, unsigned int timeout) const
   {
      std::vector<std::string> headers;
      std::stringstream schedule_out;

      int result = data_source_.GetHtml(scheduleStr + std::to_string(id), schedule_out, headers, timeout);
      if (result != 200) {
         throw std::logic_error(std::string("Error Getting Schedule: HTML ") + std::to_string(result) + "\n");
      } else {
         return MistSchedule::CreateFromJson(schedule_out);
      }
   }

   // Adds the controller to the database
   product_id_t MistDataSource::AddDevice(user_id_t userID, 
                          GeoLocale locale,
                          pt::ptime initTime,
                          size_t numZones,
                          unsigned int timeout) const 
   {
      char urlbuff[255];
      std::vector<std::string> headers;

      sprintf(urlbuff, addDeviceFmtStr, 
         userID, 
         GetEpochTime(initTime),
         locale.latitude(), 
         locale.longitude(),
         numZones);

      std::string addDeviceStr(urlbuff);
      std::stringstream deviceIDStr;

      std::cout << "ADD DEVICE:\n" << urlbuff << std::endl;

      int result = data_source_.PostHtml(addDeviceStr, 
                                          "application/x-www-form-urlencoded", 
                                          "",
                                          deviceIDStr, 
                                          headers, 
                                          timeout);
      
      std::string devStr = deviceIDStr.str();
      
      // HACK: It's the second number!
      std::getline(deviceIDStr, devStr, '\r');
      std::getline(deviceIDStr, devStr, '\r');
      boost::algorithm::trim(devStr);

      if (result < 200 || result >= 300) {
         throw std::logic_error(std::string("ERROR: Couldn't POST Add Device: HTTP ") + std::to_string(result));
      } else {
         try {
            return boost::lexical_cast<product_id_t>(devStr);
         } catch (boost::bad_lexical_cast const& e) {
             throw std::logic_error("ERROR: Received bad deviceID from server: " + devStr + ".\n" + std::string(e.what()));
         }
      } 
   }

}
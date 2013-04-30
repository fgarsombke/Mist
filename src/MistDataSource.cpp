#include "MistStd.h"
#include "MistDataSource.h"

#include "DbgDataSource.h"

using std::string;
using std::ostream;
using std::vector;


namespace Mist {
   static const std::string srcURL("www.quattrotap.com");

   static const std::string scheduleStr("/api/schedule?deviceID=");
   static const std::string feedbackStr("/api/feedback?simulator=1");
	static const std::string weatherStr("/api/weatherData?");

   static const std::string addDeviceStr("/api/device?userID");

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

      std::cout << "FEEDBACK:\n" << fbStr << std::endl;

      int result = data_source_.PostHtml(feedbackStr, "application/json", fbStr, headers, timeout);
      if (result < 200 || result >= 300) {
         throw std::logic_error(std::string("Error Posting Feedback: HTTP-") + std::to_string(result));
      } else {
         return 0;
      }
   }

   WeatherData MistDataSource::GetWeatherData(GeoLocale locale, pt::time_period period,  unsigned int timeout) 
   {
      std::vector<std::string> headers;
      std::stringstream weather_out;

      int result = data_source_.GetHtml(weatherStr, weather_out, headers, timeout);
      if (result != 200) {
         throw std::logic_error(std::string("Error Getting HTML: ") + std::to_string(result));
      } else {
         return WeatherData::CreateFromJson(weather_out);
      }
   }

   MistSchedule MistDataSource::GetSchedule(product_id_t id, unsigned int timeout) const
   {
      std::vector<std::string> headers;
      std::stringstream schedule_out;

      int result = data_source_.GetHtml(scheduleStr + std::to_string(id), schedule_out, headers, timeout);
      if (result != 200) {
         throw std::logic_error(std::string("Error Getting HTML: ") + std::to_string(result));
      } else {
         return MistSchedule::CreateFromJson(schedule_out);
      }
   }

      // Adds the controller to the database
   product_id_t MistDataSource::AddDevice(user_id_t userID, 
                          GeoLocale locale,
                          size_t numZones,
                          unsigned int timeout) const 
   {
      std::vector<std::string> headers;

      std::string addDeviceStr;
      size_t deviceID = 0;

      std::cout << "ADD DEVICE:\n" << addDeviceStr << std::endl;

      int result = data_source_.PostHtml(feedbackStr, "application/json", addDeviceStr, headers, timeout);
      if (result < 200 || result >= 300) {
         throw std::logic_error(std::string("Error Posting Add Device: HTTP-") + std::to_string(result));
      } else {
         return deviceID;
      } 
   }

}
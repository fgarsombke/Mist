#include "MistStd.h"
#include "MistDataSource.h"

using std::string;
using std::ostream;
using std::vector;


namespace Mist {
   static const std::string srcURL("www.quattrotap.com");
   static const std::string scheduleStr("/api/schedule?deviceID=");

	// TODO: Ask what this should be
   static const std::string feedbackStr("/api/feedback?deviceID=");
	static const std::string weatherStr("/api/weatherData?");

   sPtrMistDataSource MistDataSource::GetDefaultDataSource()
   {
      return sPtrMistDataSource(new MistDataSource(srcURL));
   }

   int MistDataSource::SubmitFeedback(product_id_t id, const std::vector<FeedbackList_t> feedback, unsigned int timeout) const
   {
      std::vector<std::string> headers;
      std::string fbStr = Feedback::PackFeedbackJson(feedback);

      int result = data_source_.PostHtml(feedbackStr + std::to_string(id),
         "application/json", fbStr, headers, timeout);
      if (result != 200) {
         throw std::logic_error(std::string("Error Posting HTML: ") + std::to_string(result));
      } else {
         return 0;
      }
   }

   WeatherData MistDataSource::GetWeatherData(GeoLocale locale, pt::time_period period,  unsigned int timeout) 
   {
      std::vector<std::string> headers;
      std::stringstream weather_out;

      return WeatherData();
      // TODO: Finish
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

}
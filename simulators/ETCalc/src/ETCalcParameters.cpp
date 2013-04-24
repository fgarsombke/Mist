#include "MistStd.h"

#include "ETCalcParameters.h"

namespace Mist { namespace ETCalc {

	ETCalcParameters::ETCalcParameters(pt::time_period interval, const WeatherData& data)
      : interval_(interval)
   {
      for(WeatherData::const_iterator i = data.AllSetValues();
         i < data.end(); ++i) {
         if ((*i).first == WeatherDataValue_t::MinTemp) {
            SetValue(ETCalcData_t::MinTemp, (*i).second);
         } else if ((*i).first == WeatherDataValue_t::MaxTemp) {
            SetValue(ETCalcData_t::MaxTemp, (*i).second);
         } else if ((*i).first == WeatherDataValue_t::AvgTemp) {
            SetValue(ETCalcData_t::AvgTemp, (*i).second);
         } else if ((*i).first == WeatherDataValue_t::MinRH) {
            SetValue(ETCalcData_t::MinRH, (*i).second);
         } else if ((*i).first == WeatherDataValue_t::MaxRH) {
            SetValue(ETCalcData_t::MaxRH, (*i).second);
         } else if ((*i).first == WeatherDataValue_t::StartTemp) {
            SetValue(ETCalcData_t::StartTemp, (*i).second);
         } else if ((*i).first == WeatherDataValue_t::EndTemp) {
            SetValue(ETCalcData_t::EndTemp, (*i).second);
         } else if ((*i).first == WeatherDataValue_t::AvgPressure) {
            SetValue(ETCalcData_t::AvgPressure, (*i).second);
         } else if ((*i).first == WeatherDataValue_t::AvgWindSpeed) {
            SetValue(ETCalcData_t::AvgWindSpeed, (*i).second);
         }
      }

      SetLengthType();
   }

   ETCalcParameters::ETCalcParameters(pt::time_period interval,
                    BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(TYPED_PARAMS,ET_float_t,ETDATA_PARAMS)))
   : interval_(interval)
   {
#define SET_VAL_FUNC(r, data, elem) SetValue(data::elem, elem);
      BOOST_PP_SEQ_FOR_EACH(SET_VAL_FUNC, ETCalcData_t, ETDATA_PARAMS);
#undef SET_VAL_FUNC

      SetLengthType();
   }


   void ETCalcParameters::SetLengthType()
   {
      if (interval_.length() > pt::time_duration(24*11,0,0,0)) {
         // Longer than 11 days
         length_type_ = ETCalcLengthType::Months;
      } else if (interval_.length() >= pt::time_duration(22, 0, 0, 0)) {
         // Longer than 22 hours
         length_type_ = ETCalcLengthType::Days;
      } else {
         // Hourly.  Need to determine if it's day or night
         // TODO: Actually figure out sunrise, sunset times.
         //    Here we're just using 7 am and 8 pm.
         //    Maybe: http://souptonuts.sourceforge.net/code/sunrise.c.html

         // Calculate total daylight hours

         // First calculate the daylight period on start day
         pt::ptime startSunrise = pt::ptime(interval_.begin().date()) + pt::hours(7);
         pt::ptime startSunset = pt::ptime(interval_.begin().date()) + pt::hours(20);

         auto daylightTotal = interval_.intersection(pt::time_period(startSunrise, startSunset)).length();
         // If the interval spans two days, calculate the daylight period
         //    on the second day.
         if (interval_.begin().date() != interval_.end().date()) {
             pt::ptime endSunrise = pt::ptime(interval_.begin().date()) + pt::hours(7);
             pt::ptime endSunset = pt::ptime(interval_.begin().date()) + pt::hours(20);

             daylightTotal += interval_.intersection(pt::time_period(endSunrise, endSunset)).length();
         }

         length_type_ = daylightTotal > interval_.length()/2? 
                           ETCalcLengthType::HoursDay: 
                           ETCalcLengthType::HoursNight;
      }
   }
}}
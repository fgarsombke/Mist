#pragma once

#include "LawnSimStd.h"
#include "GeoLocale.h"

namespace Mist { namespace FAO_ET {


enum class ETCalcData
{
   WindSpeed,
   MaxRH,
   MinRH,
   AvgRH,
   Pressure,
   StartTemp,
   EndTemp,
   MinTemp,
   MaxTemp,
   AvgTemp,

   SunlightFraction,

   // Last
   NumDataTypes,
};

enum class ETCalcLengthType
{
   HoursDay,
   HoursNight,
   Days,
   Months,
};

class ETCalcParameters { 
public:
   pt::time_period Interval() const { return interval_; }

   ETCalcLengthType LengthType() const { return length_type_; }

   // Use this regex s/([\w\d_]+?),/inline ET_float_t $1() const { return data_[static_cast<size_t>(ETCalcData::$1)]; }/g
   inline ET_float_t WindSpeed() const { return data_[static_cast<size_t>(ETCalcData::WindSpeed)]; }
   inline ET_float_t MaxRH() const { return data_[static_cast<size_t>(ETCalcData::MaxRH)]; }
   inline ET_float_t MinRH() const { return data_[static_cast<size_t>(ETCalcData::MinRH)]; }
   inline ET_float_t AvgRH() const { return data_[static_cast<size_t>(ETCalcData::AvgRH)]; }
   inline ET_float_t Pressure() const { return data_[static_cast<size_t>(ETCalcData::Pressure)]; }
   inline ET_float_t StartTemp() const { return data_[static_cast<size_t>(ETCalcData::StartTemp)]; }
   inline ET_float_t EndTemp() const { return data_[static_cast<size_t>(ETCalcData::EndTemp)]; }
   inline ET_float_t MinTemp() const { return data_[static_cast<size_t>(ETCalcData::MinTemp)]; }
   inline ET_float_t MaxTemp() const { return data_[static_cast<size_t>(ETCalcData::MaxTemp)]; }
   inline ET_float_t AvgTemp() const { return data_[static_cast<size_t>(ETCalcData::AvgTemp)]; }
   inline ET_float_t SunlightFraction() const { return data_[static_cast<size_t>(ETCalcData::SunlightFraction)]; }

private:
   friend class ETCalcParametersBuilder;


   ETCalcParameters(pt::time_period interval)
      :interval_(interval)
   {
      if (interval.length() > pt::time_duration(24*11,0,0,0)) {
         // Longer than 11 days
         length_type_ = ETCalcLengthType::Months;
      } else if (interval.length() >= pt::time_duration(22, 0, 0, 0)) {
         // Longer than 22 hours
         length_type_ = ETCalcLengthType::Days;
      } else {
         // Hourly.  Need to determine if it's day or night
         // TODO: Actually figure out sunrise, sunset times.
         //    Here we're just using 7 am and 8 pm.
         //    Maybe: http://souptonuts.sourceforge.net/code/sunrise.c.html

         // Calculate total daylight hours

         // First calculate the daylight period on start day
         pt::ptime startSunrise = pt::ptime(interval.begin().date()) + pt::hours(7);
         pt::ptime startSunset = pt::ptime(interval.begin().date()) + pt::hours(20);

         auto daylightTotal = interval.intersection(pt::time_period(startSunrise, startSunset)).length();
         // If the interval spans two days, calculate the daylight period
         //    on the second day.
         if (interval.begin().date() != interval.end().date()) {
             pt::ptime endSunrise = pt::ptime(interval.begin().date()) + pt::hours(7);
             pt::ptime endSunset = pt::ptime(interval.begin().date()) + pt::hours(20);

             daylightTotal += interval.intersection(pt::time_period(endSunrise, endSunset)).length();
         }

         length_type_ = daylightTotal > interval.length()/2? 
                           ETCalcLengthType::HoursDay: 
                           ETCalcLengthType::HoursNight;
      }
   }

   pt::time_period interval_;
   ETCalcLengthType length_type_;


   ET_float_t data_[static_cast<size_t>(ETCalcData::NumDataTypes)];
};

typedef const ETCalcParameters &ETParam_t;

}}
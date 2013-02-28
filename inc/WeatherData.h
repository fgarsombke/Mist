#pragma once
#include "MistStd.h"
#include <boost/optional.hpp>

namespace Mist { 

class WeatherData {
public:
   WeatherData()
      : //period_(pt::time_period(pt::not_a_date_time, pt::seconds(0))),
        start_temp_(decltype(start_temp_)()),
        end_temp_(decltype(end_temp_)()),
        avg_temp_(decltype(avg_temp_)()),
        wind_velocity_(decltype(wind_velocity_)()),
        avg_humidity_(decltype(avg_humidity_)())
   {
   }

   //const pt::time_period period() const { return period_; }

   const boost::optional<double> rainfall() const { return rainfall_; }

private:
   // The posix time period from which the data comes
   //pt::time_period period_;


   // Effective precipitation in mm
   boost::optional<double> rainfall_;

   // Not sure of units here
   // Need solar radiation

   // All in celcius
   boost::optional<double> start_temp_;
   boost::optional<double> end_temp_;
   boost::optional<double> avg_temp_;

   // first := direction in degrees from north
   // second := magnitude in meters per second
   boost::optional<std::pair<double, double> > wind_velocity_;

   // As a fraction (not percentage)
   boost::optional<double> avg_humidity_;

   // Barometric Pressure 
   boost::optional<double> avg_pressure_;
};

}


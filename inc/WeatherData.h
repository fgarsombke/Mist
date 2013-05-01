#pragma once
#include "MistStd.h"
#include "DataSetOptional.h"

namespace Mist { 

#define WDATA_VALUES (Rainfall)\
                     (MinTemp)(MaxTemp)(AvgTemp)\
                     (MinRH)(MaxRH)\
                     (StartTemp)(EndTemp)\
                     (AvgPressure)\
                     (AvgWindSpeed)\
                     (SrStart)(SrEnd)\
                     (SsStart)(SsEnd)\
                     (ENUM_COUNT)

// Effective precipitation in mm
// All in celcius


// magnitude in meters per second
// As a fraction (not percentage)

enum class WeatherDataValue_t
{
	BOOST_PP_SEQ_ENUM(WDATA_VALUES)
};

namespace WeatherDataInteral {
   extern const char* ID_LABEL;
}

typedef double WeatherDataNumeric_t;

class WeatherData 
	: public DataSetOptional<WeatherDataNumeric_t, WeatherDataValue_t, static_cast<size_t>(WeatherDataValue_t::ENUM_COUNT)>
{
public:
	template<class strT>
   static WeatherData CreateFromJson(strT &inSchedule)
   {
      boost::property_tree::ptree scheduleTree;

      bJP::read_json(inSchedule, scheduleTree);

      // HACK Added the patch from here to json_parser_read.hpp
      //    https://svn.boost.org/trac/boost/attachment/ticket/5033/property.tree.read.UTF-8.patch
      // Just kidding, I had to fix it myself. 
      // Changed 248 to this:
      // string
      // =   +(lexeme_d[confix_p('\"', *character, '\"')])
      //   | (ch_p("u") >> (+(lexeme_d[confix_p('\'', *character, '\'')])))
      //   ;
      return CreateFromPTree(scheduleTree);
   } 

   static WeatherData CreateDebug();

   WeatherData()
   {
   }

private:
   // The posix time period from which the data comes
   //pt::time_period period_;

	static WeatherData CreateFromPTree(boost::property_tree::ptree &weatherTree);
};

}


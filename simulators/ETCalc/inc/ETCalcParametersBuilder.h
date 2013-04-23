#pragma once

#include "MistStd.h"
#include "DataSetOptional.h"
#include "ETCalcParameters.h"

namespace Mist { namespace ETCalc {
   
class ETCalcParametersBuilder 
	: public DataSetOptional<ET_float_t, ETCalcData_t>
{
public:
   explicit ETCalcParametersBuilder(pt::time_period interval)
      : interval_(interval)
   {
   }

	void AddWeatherData(const WeatherData& data);

   inline ETCalcParameters Build() const
	{
		ETCalcParameters retVal(interval_);




		return retVal;
	}

private:
   pt::time_period interval_;
};




}} 
#include "MistStd.h"
#include "ETCalcParametersBuilder.h"

namespace Mist { namespace ETCalc {

ETCalcParameters ETCalcParametersBuilder::Build() const
{
   ETCalcParameters retVal(interval_);




   return retVal;
}

void ETCalcParametersBuilder::AddWeatherData(const WeatherData& data)
{

}

}}
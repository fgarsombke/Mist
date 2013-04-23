#include "MistStd.h"

#include "WeatherData.h"

#include <map>
#include "boost/assign.hpp"
using namespace std;
using namespace boost::assign;

namespace Mist { 
	using boost::property_tree::ptree;

	namespace WeatherDataInternal {
		const std::map<std::string, WeatherDataValue_t> WeatherDataMap = map_list_of
			("AvgPressure", WeatherDataValue_t::AvgPressure)
			("AvgRH", WeatherDataValue_t::AvgRH)
			("AvgTemp", WeatherDataValue_t::AvgTemp)
			("EndTemp", WeatherDataValue_t::EndTemp)
			("Rainfall", WeatherDataValue_t::Rainfall)
			("StartTemp", WeatherDataValue_t::StartTemp)
			("WindVelocity", WeatherDataValue_t::WindVelocity)
		;
	}

	using namespace WeatherDataInternal;

	WeatherData WeatherData::CreateFromPTree(ptree &weatherTree)
	{
		WeatherData data;

		// The weather data tree is a single level
		for (const pair<string, ptree> &dataChild : weatherTree) {
			WeatherDataValue_t currValue = WeatherDataMap.at(dataChild.first);
			data.SetValue(currValue, dataChild.second.get_value<WeatherDataNumeric_t>());
		}

		return data;
	}
}
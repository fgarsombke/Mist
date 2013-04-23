#include "MistStd.h"

#include "WeatherData.h"

#include <map>
#include "boost/assign.hpp"
using namespace std;
using namespace boost::assign;

namespace Mist { 
	using boost::property_tree::ptree;

	namespace WeatherDataInternal {
      #define LIST_MAP(r, data, elem) ("elem", data::elem)

		const std::map<std::string, WeatherDataValue_t> WeatherDataMap = map_list_of
         BOOST_PP_SEQ_FOR_EACH(LIST_MAP,WeatherDataValue_t,WDATA_VALUES)
		;

      #undef LIST_MAP
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
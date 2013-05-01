#include "MistStd.h"

#include "WeatherData.h"

#include <map>
#include "boost/assign.hpp"
using namespace std;
using namespace boost::assign;

namespace Mist { 
	using boost::property_tree::ptree;


	namespace WeatherDataInternal {
#define LIST_MAP(r, data, elem) (BOOST_PP_STRINGIZE_I(elem), data::elem)

		const std::map<std::string, WeatherDataValue_t> WeatherDataMap = map_list_of
         BOOST_PP_SEQ_FOR_EACH(LIST_MAP,WeatherDataValue_t,WDATA_VALUES)
		;

      #undef LIST_MAP
	}

	using namespace WeatherDataInternal;

   WeatherData WeatherData::CreateDebug()
   {
      WeatherData data;

      std::stringstream ss;

      ss <<  "{u'Rainfall':10.0, \"StartTemp\": 14.4444, \"EndTemp\": 21.6667, \"MinTemp\":13.68, \"MaxTemp\":22.10, \"AvgTemp\": 17.377778,"
         "\"AvgWindSpeed\": 11.51875, \"AvgPressure\": 1012.245, \"MinRH\": 0.52, \"MaxRH\": 0.89}";

      return WeatherData::CreateFromJson(ss);
   }

	WeatherData WeatherData::CreateFromPTree(ptree &weatherTree)
	{
		WeatherData data;

		// The weather data tree is a single level
		for (const pair<string, ptree> &dataChild : weatherTree) {
         try {
            WeatherDataValue_t currValue = WeatherDataMap.at(dataChild.first);
			   data.SetValue(currValue, dataChild.second.get_value<WeatherDataNumeric_t>());
         } catch (std::out_of_range) {
            DbgPrint("Unexpected weather data read from server: " + dataChild.first + "\n");
         }
		}

		return data;
	}
}
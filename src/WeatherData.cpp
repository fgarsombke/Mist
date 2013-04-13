#include "MistStd.h"

#include "WeatherData.h"

namespace Mist { 
	using boost::property_tree::ptree;

	WeatherData WeatherData::CreateFromPTree(ptree &weatherTree)
	{
		WeatherData wData = WeatherData();	
         
		return wData;
	}


}
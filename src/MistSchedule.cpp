#include "MistStd.h"
#include "MistSchedule.h"

namespace Mist {

namespace MistScheduleInteral{
   const char* ID_LABEL = "id";
   const char* ZONE_LABEL = "z";
   const char* ZONE_NUM_LABEL = "zoneNumber";
   const char* TIMES_ARRAY_LABEL = "times";
   const char* START_LABEL = "startTime";
   const char* END_LABEL = "endTime";
}


MistSchedule MistSchedule::CreateFromPTree(boost::property_tree::ptree &scheduleTree)
{
   using boost::property_tree::ptree;
   using namespace MistScheduleInteral;
   
   std::vector<ZoneInfo> infos(8, ZoneInfo());

   for (ptree::value_type &v : scheduleTree.get_child(ZONE_LABEL)) {
      ZoneInfo info;
      size_t zoneNum = v.second.get(ZONE_NUM_LABEL, 0) - ZONE_OFFSET;
      auto last = info.OnTimes.before_begin();

      for (ptree::value_type &t : v.second.get_child(TIMES_ARRAY_LABEL)) {
         pt::ptime start(pt::from_time_t(t.second.get<json_time_parse_t>(START_LABEL)));
         pt::ptime end(pt::from_time_t(t.second.get<json_time_parse_t>(END_LABEL)));
         
         info.OnTimes.insert_after(last,pt::time_period(start, end));
         ++last;
      }

      infos.at(zoneNum) = info;
   }
         
   return MistSchedule(infos);
}


}
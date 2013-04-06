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

pt::ptime MistSchedule::LongTimeToPTime(json_time_parse_t seconds) 
{
   const pt::ptime epoch(boost::gregorian::date(1970, 1, 1));

   return epoch + pt::seconds((long)seconds);
}

MistSchedule MistSchedule::CreateFromPTree(boost::property_tree::ptree &scheduleTree)
{
   using boost::property_tree::ptree;
   std::string idStr = scheduleTree.get<std::string>(ID_LABEL, EmptyUUIDString);
   std::vector<ZoneInfo> infos(8, ZoneInfo());

   for (ptree::value_type &v : scheduleTree.get_child(ZONE_LABEL)) {
      ZoneInfo info;
      size_t zoneNum = v.second.get(ZONE_NUM_LABEL, 0) - ZONE_OFFSET;
      auto last = info.OnTimes.before_begin();

      for (ptree::value_type &t : v.second.get_child(TIMES_ARRAY_LABEL)) {
         pt::ptime start(LongTimeToPTime(t.second.get<json_time_parse_t>(START_LABEL)));
         pt::ptime end(LongTimeToPTime(t.second.get<json_time_parse_t>(END_LABEL)));
         
         info.OnTimes.insert_after(last,pt::time_period(start, end));
         ++last;
      }

      infos.at(zoneNum) = info;
   }
         
   return MistSchedule(bUUID::string_generator()(idStr), infos);
}


}
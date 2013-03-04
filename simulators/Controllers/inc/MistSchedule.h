#pragma once
#include "ControllersStd.h"


namespace Mist { namespace Controllers {

namespace MistScheduleInteral {
   class ZoneInfo 
   {
   public:
      size_t ZoneNumber;

      std::vector<std::pair<pt::ptime, pt::ptime>> OnTimes;
   };

}

using namespace MistScheduleInteral;

class MistSchedule 
{
public:
   template<class strT>
   static MistSchedule CreateFromJson(strT &inSchedule)
   {
      MistSchedule ret;
      ptree scheduleTree;

      bJP::read_json(inSchedule, scheduleTree);
      std::string idStr = scheduleTree.get<std::string>("id");

      return MistSchedule(MistSchedule(bUUID::string_generator()(idStr)));                 
   } 

   MistSchedule()
   {
   }

   MistSchedule(uuid scheduleId)
      : schedule_id_(scheduleId)
   {

   }

   // Read the schedule in 
   

   bool operator==(const MistSchedule &other) {return schedule_id_ == other.schedule_id_;}

private:
   uuid schedule_id_;

   std::vector<ZoneInfo> schedule_data_;
};


}}
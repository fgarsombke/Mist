#pragma once
#include "MistStd.h"

namespace Mist {

namespace MistScheduleInteral {
   extern const char* ID_LABEL;
   extern const char* ZONE_LABEL;
   extern const char* ZONE_NUM_LABEL;
   extern const char* TIMES_ARRAY_LABEL;
   extern const char* START_LABEL;
   extern const char* END_LABEL;

	typedef double json_time_parse_t;
}

class ZoneInfo 
{
public:
   std::forward_list<pt::time_period> OnTimes;
};

class MistSchedule 
{
public:
   template<class strT>
   static MistSchedule CreateFromJson(strT &inSchedule)
   {
      boost::property_tree::ptree scheduleTree;

      bJP::read_json(inSchedule, scheduleTree);

      return CreateFromPTree(scheduleTree);
   } 

   MistSchedule()
   {
   }

   explicit MistSchedule(uuid scheduleId)
      : schedule_id_(scheduleId)
   {
   }

   //// Move constructor
   //MistSchedule(MistSchedule &&other) 
   //   : schedule_id_(other.schedule_id_), zone_data_(std::move(other.zone_data_))
   //{
   //   
   //}

   //MistSchedule& operator=(MistSchedule &&other) 
   //{
   //   std::swap(schedule_id_, other.schedule_id_);
   //   schedule_id_ = other.schedule_id_;
   //   return *this;
   //}

   const uuid id() const { return schedule_id_; }

   std::vector<ZoneInfo> &zone_data() { return zone_data_; }

   bool operator==(const MistSchedule &other) {return schedule_id_ == other.schedule_id_;}

   static pt::ptime LongTimeToPTime(MistScheduleInteral::json_time_parse_t ms);
private:
   uuid schedule_id_;

   std::vector<ZoneInfo> zone_data_;

   MistSchedule(uuid scheduleId, std::vector<ZoneInfo> &zoneInfos)
      : schedule_id_(scheduleId), zone_data_(std::move(zoneInfos))
   {
   }

   static MistSchedule CreateFromPTree(boost::property_tree::ptree &scheduleTree);
};


}
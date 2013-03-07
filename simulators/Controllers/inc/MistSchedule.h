#pragma once
#include "ControllersStd.h"


namespace Mist { namespace Controllers {

const int ZONE_OFFSET = 1;


namespace MistScheduleInteral {
extern const char* ID_LABEL;
extern const char* ZONE_LABEL;
extern const char* ZONE_NUM_LABEL;
extern const char* TIMES_ARRAY_LABEL;
extern const char* START_LABEL;
extern const char* END_LABEL;
}

class ZoneInfo 
{
public:
   std::forward_list<pt::time_period> OnTimes;
};

using namespace MistScheduleInteral;

class MistSchedule 
{
public:
   template<class strT>
   static MistSchedule CreateFromJson(strT &inSchedule)
   {
      ptree scheduleTree;

      bJP::read_json(inSchedule, scheduleTree);

      return CreateFromPTree(scheduleTree);
   } 

   MistSchedule()
   {
   }

   MistSchedule(uuid scheduleId)
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

   // Read the schedule in 
   bool operator==(const MistSchedule &other) {return schedule_id_ == other.schedule_id_;}

   static pt::ptime LongTimeToPTime(uint64_t ms);
private:
   uuid schedule_id_;

   std::vector<ZoneInfo> zone_data_;

   MistSchedule(uuid scheduleId, std::vector<ZoneInfo> &zoneInfos)
      : schedule_id_(scheduleId), zone_data_(std::move(zoneInfos))
   {
   }

   static MistSchedule CreateFromPTree(ptree &scheduleTree);
};


}}
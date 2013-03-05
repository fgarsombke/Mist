#include "ControllersStd.h"

#include "MistController.h"
#include "MistFileController.h"
#include "MistRealController.h"

namespace Mist { namespace Controllers {

void MistController::Reset(pt::ptime startTime)
{
   ResetSchedule(MistSchedule::LongTimeToPTime(0), MistSchedule());
}

void MistController::ResetSchedule(pt::ptime updateTime, MistSchedule &&newSchedule) 
{
   last_update_time_ = updateTime;
   current_schedule_ = newSchedule; 
}

void MistController::ElapseTime(pt::time_period interval, 
                                std::vector<pt::time_duration> &sprinklerOnDurations)
{
   using namespace pt;

   size_t numSprinklers = sprinklerOnDurations.size();
   for (size_t i = 0; i < numSprinklers; ++i) {
      ZoneInfo &zone = current_schedule_.zone_data()[i];
      sprinklerOnDurations[i] = seconds(0);

      while(!zone.OnTimes.empty()) {
         const time_period currOnTime = zone.OnTimes.front();
         
         if (currOnTime.end() > interval.begin()) {
            if (currOnTime.begin() < interval.end()) {
               if (currOnTime.end() < interval.end()) {
                  sprinklerOnDurations[i] += currOnTime.length();
               } else {
                  sprinklerOnDurations[i] += currOnTime.intersection(interval).length();
                  break;
               }
            } else {
               // Don't bother iterating through this zone any more
               break;
            }
         } 

         zone.OnTimes.pop_front();
      }
   }
}

// File Controller specific code
/////////////////////////////////////////////////////////////////////////////////////

void MistFileController::Reset(pt::ptime startTime)
{
   // The File Controller just reads it's data source in on reset.
   // It doesn't update otherwise.
   std::ifstream scheduleStream(config().data_source_);
   MistController::ResetSchedule(startTime, MistSchedule::CreateFromJson(scheduleStream));
}

// Real Controller specific code
/////////////////////////////////////////////////////////////////////////////////////

pt::ptime MistController::NextUpdateTimeAfter(pt::ptime afterTime) const 
{
   double updateMillis = (double)update_period().total_milliseconds();

   unsigned int overDiff = (unsigned int)(((afterTime - last_update_time()).total_milliseconds())/updateMillis + 1);

   return last_update_time() + pt::millisec((int64_t)(overDiff*updateMillis));
}

// TODO: Fix this
void MistRealController::ElapseTime(pt::time_period interval, 
                        std::vector<pt::time_duration> &sprinklerOnDurations)
{
   using namespace pt;

   ptime elapseEnd = interval.end();

   ptime subElapseStart = interval.begin();
   ptime subElapseEnd;

   while (subElapseStart < elapseEnd) {
      ptime nextUpdate = NextUpdateTimeAfter(subElapseStart);
      subElapseEnd = min(nextUpdate, elapseEnd);

      if (MistController::HasUpdatePeriodPassed(subElapseEnd)) {
         ScheduleSource::schedule_ret_t ss;
         source_->GetSchedule(ss, 2000);
         ResetSchedule(nextUpdate, MistSchedule::CreateFromJson(ss));
      }

      MistController::ElapseTime(time_period(subElapseStart, subElapseEnd), sprinklerOnDurations);
      
      subElapseStart = subElapseEnd;
   }
}

}}
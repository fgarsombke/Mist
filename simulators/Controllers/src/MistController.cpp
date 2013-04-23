#include "ControllersStd.h"

#include "MistController.h"
#include "MistUpdatingController.h"

namespace Mist { namespace Controllers {

void MistController::Reset(pt::ptime startTime)
{
   ResetSchedule(startTime, schedule_source_->GetSchedule(id_, -1));
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

   size_t numSprinklers = current_schedule_.zone_data().size();

	if (sprinklerOnDurations.size() < numSprinklers) {
		throw std::logic_error("sprinklerOnDurations is smaller than numSprinklers");
	}

   for (size_t i = 0; i < numSprinklers; ++i) {
      ZoneInfo &zone = current_schedule_.zone_data()[i];
      sprinklerOnDurations[i] = seconds(0);

      while(!zone.OnTimes.empty()) {
         const time_period currOnTime = zone.OnTimes.front();
         
         // Fall through to removing the OnTime if the scheduled
         //    on period is completely before the interval in question.
         if (currOnTime.end() > interval.begin()) {
            // Move onto the next zone if the OnTime comes after the 
            //    interval in question.
            if (currOnTime.begin() < interval.end()) {
               // Fall through to remove the current on time if it is completely
               //    inside the interval in question.
               sprinklerOnDurations[i] += currOnTime.intersection(interval).length();
               if (currOnTime.end() >= interval.end()) {
                  break;
               }
            } else {
               // Don't bother iterating through this zone any more
               break;
            }
         } 
         
         // Remove the current on time
         zone.OnTimes.pop_front();
      }
   }
}

// Real Controller specific code
/////////////////////////////////////////////////////////////////////////////////////

pt::ptime MistController::NextUpdateTimeAfter(pt::ptime afterTime) const 
{
   double updateMillis = (double)update_period().total_milliseconds();

   unsigned int overDiff = (unsigned int)(((afterTime - last_update_time()).total_milliseconds())/updateMillis + 1);

   return last_update_time() + pt::millisec((int64_t)(overDiff*updateMillis));
}

void MistUpdatingController::ElapseTime(pt::time_period interval, 
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
         ResetSchedule(nextUpdate, schedule_source().GetSchedule(id(), 2000));
      }

      MistController::ElapseTime(time_period(subElapseStart, subElapseEnd), sprinklerOnDurations);
      
      subElapseStart = subElapseEnd;
   }
}

}}
#include "ControllersStd.h"

#include "MistController.h"
#include "MistFileController.h"
#include "MistRealController.h"

namespace Mist { namespace Controllers {

void MistController::Reset(pt::ptime startTime)
{
   last_update_time_ = startTime;

   // TODO: Clear Current Schedules
}

void MistController::ElapseTime(pt::time_period interval, 
                        std::vector<pt::time_duration> &sprinklerOnDurations)
{

}


// File Controller specific code
/////////////////////////////////////////////////////////////////////////////////////

void MistFileController::Reset(pt::ptime startTime)
{
   // The File Controller just reads it's data source in on reset.
   // It doesn't update otherwise.
   MistController::Reset(startTime);

   std::ifstream scheduleStream(config().data_source_);
   MistController::ResetSchedule(MistSchedule::CreateFromJson(scheduleStream));
}

// Real Controller specific code
/////////////////////////////////////////////////////////////////////////////////////

void MistRealController::ElapseTime(pt::time_period interval, 
                        std::vector<pt::time_duration> &sprinklerOnDurations)
{

}

}}
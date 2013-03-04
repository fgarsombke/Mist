#pragma once
#include "ControllersStd.h"
#include "Controller.h"

#include "MistSchedule.h"

namespace Mist { namespace Controllers {

// The NullController never waters the grass
class MistController : public Controller {
public:
   // Resets the last update time to startTime
   virtual void Reset(pt::ptime startTime) override;

   // Implements basic Mist schedule elapse based on the current schedule
   virtual void ElapseTime(pt::time_period interval, 
                           std::vector<pt::time_duration> &sprinklerOnDurations) override;

protected: 
   MistController(const ControllerConfig &config)
      : update_period_(config.update_period_), Controller(config)
   {
      is_valid_ = false;
   }
   
   void ResetSchedule(MistSchedule &&newSchedule) { current_schedule_ = std::move(newSchedule); }

   // Returns true if update period has passed since the last call to Reset
   bool HasUpdatePeriodPassed(pt::ptime now) const { return last_update_time_ + update_period_ > now; }

   // Reads in a new schedule from the specified stream
   template<class strT>
   void ReadInNewSchedule(strT &inSchedule);

private:
   const pt::time_duration update_period_;

   MistSchedule current_schedule_;
   pt::ptime last_update_time_;

   bool is_valid_;
};

}}

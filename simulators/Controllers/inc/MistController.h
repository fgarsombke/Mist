#pragma once
#include "ControllersStd.h"
#include "Controller.h"

#include "MistSchedule.h"

namespace Mist { namespace Controllers {

// The NullController never waters the grass
class MistController : public Controller {
public:
   explicit MistController(ControllerConfig &&config)
      :  locale_(config.Locale),
         num_zones_(config.numZones),
         update_period_(config.UpdatePeriod),
         schedule_source_(std::move(config.ScheduleSource))
   {
      has_started_ = false;
   }

   // Resets the last update time to startTime
   virtual void Reset(pt::ptime startTime) override;

   // Implements basic Mist schedule elapse based on the current schedule
   virtual void ElapseTime(pt::time_period interval, 
                           std::vector<pt::time_duration> &sprinklerOnDurations) override;

   const product_id_t id() const override { return id_; }
   const MistSchedule &current_schedule() const { return current_schedule_; }
   const pt::time_duration update_period() const { return update_period_; }
   const pt::ptime last_update_time() const { return last_update_time_; }

protected:   
   void ResetSchedule(pt::ptime now, MistSchedule &&newSchedule);

   // Returns true if update period has passed since the last call to Reset
   bool HasUpdatePeriodPassed(pt::ptime now) const 
   { 
      return last_update_time_ + update_period_ <= now; 
   }

   pt::ptime NextUpdateTimeAfter(pt::ptime afterTime) const;

   sPtrScheduleSource schedule_source() const { return schedule_source_; }
   sPtrScheduleSource schedule_source() { return schedule_source_; }

   // Reads in a new schedule from the specified stream
   template<class strT>
   void ReadInNewSchedule(strT &inSchedule);

private:
   // Reference class only
   MistController &operator=(MistController other);
   MistController(const MistController &other);

   const GeoLocale locale_;
   const size_t num_zones_;
   const pt::time_duration update_period_;

   const sPtrScheduleSource schedule_source_;

   MistSchedule current_schedule_;
   pt::ptime last_update_time_;

   pt::ptime start_time_;

   product_id_t id_;
   bool has_started_;
};

}}

#pragma once
#include "Controller.h"

#include "ScheduleSource.h"

namespace Mist { namespace Controllers {

// The NullController never waters the grass
class MistRealController : public MistController {
public:
   MistRealController(const ControllerConfig &config)
      : MistController(config), 
        source_(uPtrScheduleSource(new ScheduleSource(config.data_source_, config.id)))
   {
   }

   virtual void ElapseTime(pt::time_period interval, 
                           std::vector<pt::time_duration> &sprinklerOnDurations) override;

private:
   uPtrScheduleSource source_;

};

}}

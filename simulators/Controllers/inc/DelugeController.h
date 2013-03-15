#pragma once
#include "ControllersStd.h"

#include "Controller.h"

namespace Mist { namespace Controllers {

// The NullController never waters the grass
class DelugeController : public Controller {
public:
   DelugeController(const ControllerConfig &config)
      : Controller(config)
   {
   }

   virtual void Reset(boost::posix_time::ptime startTime) override 
   {
      // Do nothing, always water
   }

   virtual void ElapseTime(pt::time_period interval, 
                           std::vector<pt::time_duration> &sprinklerOnDurations) 
   {
      // Always water
      for (pt::time_duration &dur : sprinklerOnDurations) {
         dur = interval.length();
      }
   }
};

}}

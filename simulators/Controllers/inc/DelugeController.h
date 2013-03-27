#pragma once
#include "ControllersStd.h"

#include "Controller.h"

namespace Mist { namespace Controllers {

// The NullController never waters the grass
class DelugeController : public Controller {
public:
   virtual void ElapseTime(pt::time_period interval, 
                           std::vector<pt::time_duration> &sprinklerOnDurations) override
   {
      // Always water
      for (pt::time_duration &dur : sprinklerOnDurations) {
         dur = interval.length();
      }
   }
};

}}

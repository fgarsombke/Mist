#pragma once
#include "Controller.h"

namespace Mist { namespace Controllers {

// The NullController never waters the grass
class NullController : public Controller {
public:
   virtual void Reset(boost::posix_time::ptime startTime) override 
   {
      // Do nothing, never water
   }

   virtual void ElapseTime(pt::time_period interval, 
                           std::vector<pt::time_duration> &sprinklerOnDurations) 
   {
      // Never water
      for (pt::time_duration &dur : sprinklerOnDurations) {
         dur = pt::seconds(0);
      }
   }
};

}}

#pragma once
#include "Controller.h"

namespace Mist { namespace Controllers {

// The NullController never waters the grass
class MistRealController : public MistController {
public:
   MistRealController(const ControllerConfig &config)
      : MistController(config)
   {
   }

   virtual void ElapseTime(pt::time_period interval, 
                           std::vector<pt::time_duration> &sprinklerOnDurations) override;

};

}}

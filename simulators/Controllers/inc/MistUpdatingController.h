#pragma once
#include "Controller.h"

#include "MistController.h"

namespace Mist { namespace Controllers {

// The NullController never waters the grass
class MistUpdatingController : public MistController {
public:
   explicit MistUpdatingController(ControllerConfig &&config)
      : MistController(std::forward<ControllerConfig>(config))
   {
   }

   virtual void ElapseTime(pt::time_period interval, 
                           std::vector<pt::time_duration> &sprinklerOnDurations) override;
};

}}

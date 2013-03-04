#pragma once
#include "ControllersStd.h"
#include "Controller.h"

namespace Mist { namespace Controllers {

// The NullController never waters the grass
class MistFileController : public MistController {
public:
   MistFileController(const ControllerConfig &config)
      : MistController(config)
   {
   }

   virtual void Reset(pt::ptime startTime) override;

};

}}

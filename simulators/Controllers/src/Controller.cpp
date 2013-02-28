#include "Controller.h"

#include "NullController.h"

namespace Mist { namespace Controllers {

unique_ptr<Controller> Controller::GetControllerByName(const string &controllerName, const ControllerConfig &config)
{
   unique_ptr<Controller> ret;

   if (controllerName == "Default" ||
       controllerName == "NullController") {
      ret = unique_ptr<Controller>(new NullController(config));
   } 

   return ret;
}


}}

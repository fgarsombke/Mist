#include "ControllersStd.h"

#include "Controller.h"

#include "NullController.h"
#include "DelugeController.h"
#include "MistController.h"
#include "MistFileController.h"
#include "MistRealController.h"

namespace Mist { namespace Controllers {

uPtrController Controller::GetControllerByName(const string &controllerName, const ControllerConfig &config)
{
   uPtrController ret;

   if (controllerName == "Default" ||
       controllerName == "Null") {
      ret = uPtrController(new NullController(config));
   } else if (controllerName == "Deluge" ) {
      ret = uPtrController(new DelugeController(config));
   } else if (controllerName == "MistReal") {
      ret = uPtrController(new MistRealController(config));
   } else if (controllerName == "MistFile") { 
      ret = uPtrController(new MistFileController(config));
   }

   return ret;
}


}}

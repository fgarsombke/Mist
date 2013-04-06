#include "ControllersStd.h"

#include "Controller.h"

#include "NullController.h"
#include "DelugeController.h"
#include "MistController.h"
#include "MistUpdatingController.h"

namespace Mist { namespace Controllers {

uPtrController Controller::GetControllerByName(const string &controllerName, ControllerConfig &&config)
{
   Controller *retPtr = nullptr;

   if (controllerName == "Default" ||
       controllerName == "Null") {
      retPtr = new NullController();
   } else if (controllerName == "Deluge" ) {
      retPtr = new DelugeController();
   } else if (controllerName == "Mist") { 
      retPtr = new MistController(std::forward<ControllerConfig>(config));
   } else if (controllerName == "MistReal") {
      retPtr = new MistUpdatingController(std::forward<ControllerConfig>(config));
   } 

   return uPtrController(retPtr);
}


}}

#include "Controller.h"

namespace Mist { 
namespace Controllers {

// The NullController never waters the grass
class NullController : public Controller {
public:
   NullController(const ControllerConfig &config)
      : Controller(config)
   {
      
   }

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


unique_ptr<Controller> Controller::GetControllerByName(const string &controllerName, const ControllerConfig &config)
{
   unique_ptr<Controller> ret;

   if (controllerName == "NullController") {
      ret = unique_ptr<Controller>(new NullController(config));
   } 

   return ret;
}


}}

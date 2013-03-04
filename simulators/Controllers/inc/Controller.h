#pragma once

#include "ControllersStd.h"
#include "ControllerConfig.h"

namespace pt = boost::posix_time;
using namespace std;

#define UPTR_TYPEDEF(T) typedef std::unique_ptr<T> uPtr_T

namespace Mist { namespace Controllers  {

class Controller;
typedef std::unique_ptr<Controller> uPtrController;

class Controller {
public:
   static uPtrController GetControllerByName(const string &controllerName, const ControllerConfig &config);

   // Resets the controller to startTime
   virtual void Reset(pt::ptime startTime) = 0;
   
   // Indicates to the controller that an interval of time has elapsed.
   //    
   // The Controller will fill onDurations with total duration of sprinkler on time 
   //    during that interval.
   //
   // ElapseTime may throw an exception if the interval requested overlaps a 
   //    previously requested interval (always ask for future intervals).
   virtual void ElapseTime(pt::time_period interval, 
                           std::vector<pt::time_duration> &sprinklerOnDurations) = 0;

protected:
   // Creates a controller based on config
   Controller(const ControllerConfig &config)
      :config_(config)
   {
   }

   const ControllerConfig config() const { return config_; }
private:
   // Reference class only
   Controller &operator=(const Controller &other);
   Controller(const Controller &other);


   const ControllerConfig config_;   
};

}}
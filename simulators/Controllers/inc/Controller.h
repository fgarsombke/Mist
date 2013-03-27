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
   static uPtrController GetControllerByName(const string &controllerName, ControllerConfig &&config);

   Controller() {}

   // Resets the controller to startTime
   virtual void Reset(pt::ptime startTime)
   {
      // Do nothing by default
   }
   
   // Indicates to the controller that an interval of time has elapsed.
   //    
   // The Controller will fill onDurations with total duration of sprinkler on time 
   //    during that interval.
   //
   // ElapseTime may throw an exception if the interval requested overlaps a 
   //    previously requested interval (always ask for future intervals).
   virtual void ElapseTime(pt::time_period interval, 
                           std::vector<pt::time_duration> &sprinklerOnDurations) = 0;

  virtual ~Controller() { }
private:
   // Reference class only
   Controller &operator=(const Controller &other);
   Controller(const Controller &other);
};

}}
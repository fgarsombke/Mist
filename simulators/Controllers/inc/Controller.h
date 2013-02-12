#pragma once

#include <boost/date_time.hpp>
#include "ControllerConfig.h"

namespace pt = boost::posix_time;
using namespace std;

namespace Mist {
namespace Controllers  {


class Controller {
public:
   static unique_ptr<Controller> GetControllerByName(const string &controllerName, const ControllerConfig &config);

   // Resets the controller to startTime
   virtual void Reset(boost::posix_time::ptime startTime) = 0;
   
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

   const ControllerConfig config () const { return config_; }

private:
   const ControllerConfig config_;   
};


}}
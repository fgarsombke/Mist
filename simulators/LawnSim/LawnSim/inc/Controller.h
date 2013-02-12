#pragma once

#include <boost/date_time.hpp>
#include "ControllerConfig.h"

namespace pt = boost::posix_time;
namespace Mist {
namespace MistControllers {


class Controller {
public:
   // Creates a controller with no configuration
   Controller();
   
   // Creates a controller based on config
   Controller(const ControllerConfig& config);

   // Resets the controller to startTime
   virtual void Reset(boost::posix_time::ptime startTime) = 0;
   
   // Indicates to the controller that an interval of time has elapsed.
   //    
   // The interval is defined as the simulated period of time between:
   //    startTime passed to the most recent call of Reset if Reset was called more 
   //       recently than ElapseTime.
   //    OR
   //    intervalEnd passed to the most recent call ElapseTime if ElapseTime was called
   //       more recently than Reset.
   //
   // The Controller will fill onDurations with total duration of sprinkler on time 
   //    during that interval.
   virtual void ElapseTime(pt::ptime intervalEnd, 
                           const std::vector<pt::time_duration> &sprinklerOnDurations) = 0;

};

}
}
#pragma once

#include "LawnSimStd.h"

#include "YardInfo.h"
#include "Yard.h"
#include "Controller.h"

using namespace boost::posix_time;
using namespace Mist::Controllers;

namespace Mist { namespace LawnSim {

class Simulator {
public:
   Simulator(const YardInfo &yardInfo, 
             uPtrController &controller,
             sPtrWeatherDataSource weatherSource);

   void Reset(ptime simStartTime, 
               ptime simEndTime,
               unsigned int simTickPeriod,
               unsigned int simSpeedMultiplier);

   void Start();
   void Stop();
   void Pause();

   const Yard &yard() const { return yard_; }

private:

   // Simulation Parameters
   ptime sim_start_time_;              // Simulated start time in POSIX time
   ptime sim_end_time_;                // Simulated end time in POSIX time
   time_duration sim_tick_duration_;   // Simulated time quantum

   unsigned int tick_period_ms_;       // Real tick period.  

   
   /////////////////////////////
   Yard yard_;
   
   uPtrController controller_;
   sPtrWeatherDataSource weather_source_;

   unsigned int real_start_time_;      // Last Real world (simulator) start time 

};

}}
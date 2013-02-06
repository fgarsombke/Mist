#pragma once

#include "LawnSimStd.h"

#include "Yard.h"

using namespace boost::posix_time;
namespace LawnSim {

class Simulator {
private:

   ptime sim_start_time_;              // Simulated start time in POSIX time
   ptime sim_end_time_;                // Simulated end time in POSIX time

   unsigned int real_start_time_;      // Real world (simulator) start time 
   unsigned int next_tick_time_;


   unsigned int tick_period_ms_;       // Real tick period.  
   unsigned int sim_time_multiplier_;  // (simulated time)/(real time) 


   inline void doTickWork(ptime tick_time, 
                          time_duration tick_duration);

public:
   Simulator(const Yard &yard) {}


   void Initialize(ptime simStartTime, 
                   ptime simEndTime,
                   unsigned int simTickPeriod,
                   unsigned int simSpeedMultiplier
   );

   void Run();



};

}
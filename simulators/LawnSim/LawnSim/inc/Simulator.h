#pragma once

#include "LawnSimStd.h"

#include "Yard.h"

using namespace boost::posix_time;

namespace Mist {
namespace LawnSim {

class Simulator {
private:

   // Simulation Parameters
   ptime sim_start_time_;              // Simulated start time in POSIX time
   ptime sim_end_time_;                // Simulated end time in POSIX time
   time_duration sim_tick_duration_;   // Simulated time quantum

   unsigned int tick_period_ms_;       // Real tick period.  

   
   /////////////////////////////


   unsigned int real_start_time_;      // Last Real world (simulator) start time 


   Yard yard_;

   inline void doTickWork(const ptime tickStartTime, 
                          const time_duration tickDuration);

public:
   Simulator(const Yard &yard);


   void Reset(ptime simStartTime, 
               ptime simEndTime,
               unsigned int simTickPeriod,
               unsigned int simSpeedMultiplier
   );

   void Run();

   void Stop();

};

}
}
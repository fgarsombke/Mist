#include "LawnSimStd.h"

#include "Simulator.h"

#ifdef _MSC_VER 

#include <Windows.h>
//#include <mmsystem.h>

#define GetSystemTimeMS() GetTickCount() //timeGetTime()  

inline int SleepForMS(int x) { 
   return SleepEx(x, false);
}

#else

#include <time.h>
inline int SleepForMS(int x) {  
   struct timespec tim; 
   int sleepSec = (x)/1000; 
   int sleepRem = (x)%1000; 
   tim.tv_sec = sleepSec; 
   tim.tv_nsec = sleepRem * 1000000L; 

   nanosleep(&tim, NULL);
}



#endif

namespace Mist {
namespace LawnSim {

   
Simulator::Simulator(const YardInfo &yard)
   : yard_(yard)
{
   // Set start time ahead of end time so the sim won't run
   sim_start_time_ = ptime(boost::gregorian::date(1970,1,2));
   sim_end_time_ = ptime(boost::gregorian::date(1970,1,1));
}


void Simulator::Reset(ptime simStartTime, 
                           ptime simEndTime,
                           unsigned int simTickPeriod,
                           unsigned int simSpeedMultiplier
) {
   // TODO: Reset the running simulation
   
   sim_start_time_ = simStartTime;
   sim_end_time_ = simEndTime;
   sim_tick_duration_ = time_duration(pt::milliseconds(simTickPeriod));

   if (simTickPeriod%simSpeedMultiplier != 0) {
      throw std::invalid_argument(Constants::SimMultDoesNotDivide);
   }

   tick_period_ms_ = simTickPeriod/simSpeedMultiplier;
}

void Simulator::Run() 
{
   // Basic logic: 
   //    -Do one tick
   //    -Set time for next tick
   //    -Sleep until it's time to tick again

   real_start_time_ = GetSystemTimeMS();
   int nextTickTime = real_start_time_;       //Set so that computation below will work

   pt::ptime this_tick_sim_time = sim_start_time_;
   
   while(this_tick_sim_time <= sim_end_time_) {
      time_duration tick_duration = sim_tick_duration_;

      // TODO: Add boost Logging
      // Do the tick work
      yard_.ElapseTime(weather_source_.GetData(yard_.locale(), pt::time_period(this_tick_sim_time, tick_duration)));
      
      // Compute next tick time
      nextTickTime += tick_period_ms_;

      // Compute the simulated time
      this_tick_sim_time += tick_duration;

      decltype(GetSystemTimeMS()) sysTime = GetSystemTimeMS();
      int sleepTime = nextTickTime - sysTime;

      // Don't sleep if we're already behind
      // TODO: Figure out a better way to handle slowdown
      if (sleepTime > 0) {
         SleepForMS(sleepTime);
      } else {
         std::cout << "Tick Period: " << tick_period_ms_ << std::endl;
         // Slow down a little
         tick_period_ms_ = (unsigned int)(ceil(tick_period_ms_ * 1.01f));
      }
   }
  
}

}
}
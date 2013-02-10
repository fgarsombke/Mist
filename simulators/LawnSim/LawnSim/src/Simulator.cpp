#include "LawnSimStd.h"
#include "Constants.h"

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

namespace LawnSim {
   

void Simulator::Initialize(ptime simStartTime, 
                           ptime simEndTime,
                           unsigned int simTickPeriod,
                           unsigned int simSpeedMultiplier
) {
   // TODO: Reset the running simulation
   
   sim_start_time_ = simStartTime;
   sim_end_time_ = simEndTime;

   if (simTickPeriod%simSpeedMultiplier != 0) {
      throw std::invalid_argument(Constants::SimMultDoesNotDivide);
   }

   tick_period_ms_ = simTickPeriod/simSpeedMultiplier;
   sim_time_multiplier_ = simSpeedMultiplier;
}

void Simulator::Run() 
{
   // Basic logic: 
   //    -Do one tick
   //    -Set time for next tick
   //    -Sleep until it's time to tick again

   real_start_time_ = GetSystemTimeMS();
   unsigned int nextTickTime = real_start_time_;       //Set so that computation below will work

   pt::ptime this_tick_sim_time = sim_start_time_;
   
   while(this_tick_sim_time <= sim_end_time_) {
      time_duration tick_duration = pt::milliseconds(sim_time_multiplier_ * tick_period_ms_);

      // TODO: Add boost Logging
      // Do the tick work
      doTickWork(this_tick_sim_time, tick_duration);
      
      // Compute next tick time
      nextTickTime += tick_period_ms_;

      // Compute the simulated time
      this_tick_sim_time += tick_duration;

      long long sleepTime = nextTickTime - GetSystemTimeMS();

      // Don't sleep if we're already behind
      // TODO: Figure out a better way to handle slowdown
      if (sleepTime > 0) {
         SleepForMS((int)sleepTime);
      } else {
         // Slow down a little
         tick_period_ms_ = (unsigned int)(tick_period_ms_ * 1.1f);
      }
   }
  
}


void Simulator::doTickWork(pt::ptime tickStartTime,  pt::time_duration tickDuration) 
{
   pt::ptime tickEndTime = tickStartTime + tickDuration;


   printf("Tick: %s\n", pt::to_simple_string(tickStartTime).c_str());
}

}
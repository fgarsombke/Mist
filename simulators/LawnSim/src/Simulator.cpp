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

namespace Mist { namespace LawnSim {

Simulator::Simulator(std::unique_ptr<YardInfo> &yardInfo, unique_ptr<Controllers::Controller> &controller)
   : yard_(*yardInfo.get()), controller_(std::move(controller))
{
   // Set start time ahead of end time so the sim won't run
   sim_start_time_ = ptime(boost::gregorian::date(1970,1,2));
   sim_end_time_ = ptime(boost::gregorian::date(1970,1,1));

   yardInfo.release();
}


void Simulator::Reset(ptime simStartTime, 
                      ptime simEndTime,
                      unsigned int simTickPeriod,
                      unsigned int simSpeedMultiplier
) {
   // TODO: Reset the running simulation
   Stop();


   sim_start_time_ = simStartTime;
   sim_end_time_ = simEndTime;
   sim_tick_duration_ = time_duration(pt::milliseconds(simTickPeriod));

   if (simTickPeriod%simSpeedMultiplier != 0) {
      throw std::invalid_argument(Constants::SimMultDoesNotDivide);
   }

   tick_period_ms_ = simTickPeriod/simSpeedMultiplier;

   // Put the yard into it's initial state
   yard_.ResetState();
}

void Simulator::Run() 
{
   std::vector<time_duration> sprinklerDurations = std::vector<pt::time_duration>(yard_.SprinklersCount());

   // Basic logic: 
   //    -Do one tick
   //    -Set time for next tick
   //    -Sleep until it's time to tick again

   real_start_time_ = GetSystemTimeMS();
   int nextTickTime = real_start_time_;       //Set so that computation below will work

   pt::ptime this_tick_sim_time = sim_start_time_;
   
   while(this_tick_sim_time <= sim_end_time_) {
      time_duration tick_duration = sim_tick_duration_;
      time_period tick_period = time_period(this_tick_sim_time, tick_duration);

      // TODO: Add boost Logging
      // TODO: Precompute WeatherData for the next tick
      // Do the tick work

      WeatherData weatherData = weather_source_.GetData(yard_.locale(), tick_period);
      controller_->ElapseTime(tick_period, sprinklerDurations);

      // Let the yard check whether or not the sprinkler durations list was resized
      yard_.ElapseTime(tick_period, weatherData, sprinklerDurations);
      
      // Compute next tick time
      nextTickTime += tick_period_ms_;

      // Compute the simulated time
      this_tick_sim_time += tick_duration;

      int sysTime = GetSystemTimeMS();
      int sleepTime = nextTickTime - sysTime;

      // Don't sleep if we're already behind
      // TODO: Figure out a better way to handle slowdown
      if (sleepTime > 0) {
         SleepForMS(sleepTime);
      } else {
         std::cout << "Tick Period: " << tick_period_ms_ << std::endl;
         // Slow down a little
         nextTickTime = sysTime;
         tick_period_ms_ = (unsigned int)(ceil(tick_period_ms_ * 1.1f));
      }
   }
  
}



void Simulator::Stop() 
{
   // TODO: Implement
}

}}
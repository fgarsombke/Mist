#include "LawnSimStd.h"

#include "Simulator.h"
#include "PAL.h"


#if _DEBUG_SPRINKLER_DURATIONS

void DbgPrintDurations(const sprinkler_durations_t &durations) 
{
   cout << "Sprinkler Durations:" << endl;
   for (auto t : durations) {
      cout << "\t" << t.total_seconds() << endl;
   }
   cout << endl;
}

#else

#define DbgPrintDurations(X) //Nothing

#endif

namespace Mist { namespace LawnSim {

Simulator::Simulator(const YardInfo &yardInfo, 
                     uPtrController &controller,
                     sPtrFeedbackSink feedbackSink,
                     sPtrWeatherDataSource weatherSource)
   : yard_(yardInfo), 
      controller_(std::move(controller)), 
      feedback_sink_(feedbackSink), 
      weather_source_(weatherSource),
		sim_period_(ptime(boost::gregorian::date(1970,1,2)), ptime(boost::gregorian::date(1970,1,1)))
{
   // Set start time ahead of end time so the sim won't run
}

void Simulator::Reset(time_period simPeriod,
							 unsigned int simTickDuration,
                      unsigned int simSpeedMultiplier) 
{
   // TODO: Reset the running simulation
   sim_period_ = simPeriod;
   sim_tick_duration_ = time_duration(pt::milliseconds(simTickDuration));

   if (simTickDuration%simSpeedMultiplier != 0) {
      throw std::invalid_argument(Constants::SimMultDoesNotDivide);
   }

   real_tick_period = simTickDuration/simSpeedMultiplier;

   // Put the yard into it's initial state
   controller_->Reset(sim_period_.begin());
   yard_.ResetState();
}

void Simulator::Start() 
{
	// The world was void and witout form
	std::array<std::thread, num_tick_pipes_> pipeThreads;

	ptime simStartTime = sim_period_.begin();
	real_start_time_ = GetSystemTimeMS();
	next_real_tick_end_target_ = real_start_time_ + real_tick_period;

	// Breath of life
	for (size_t t_i = 0; t_i < pipeThreads.size(); ++t_i) {
		pipeThreads[t_i] = std::move(
			std::thread([this, t_i](pt::ptime firstTickTime) {
            try {
               DoTickWork(firstTickTime);
            } catch(std::exception e) {
               async_exceptions_[t_i] = std::current_exception();
            }
         }, 
         simStartTime+sim_tick_duration_*((int)t_i))
		);
	}
	
	// Death comes for us all
	for (size_t t_i = 0; t_i < pipeThreads.size(); ++t_i) {
		pipeThreads[t_i].join();

      if (async_exceptions_[t_i] != nullptr) {
         std::rethrow_exception(async_exceptions_[t_i]);
      }
	}
}

void Simulator::DoTickWork(pt::ptime firstTickTime)
{
	WeatherData weatherData;
	sprinkler_durations_t sprinklerDurations = sprinkler_durations_t(yard_.SprinklersCount());
   ZoneFeedback_t zoneFeedback = ZoneFeedback_t(yard_.ZoneCount());

   // Basic logic: 
   //    -Do one tick
   //    -Set time for next tick
   //    -Sleep until it's time to tick again
   pt::ptime thisTickSimTime = firstTickTime;
	ptime simEndTime = sim_period_.end();
	
   while(thisTickSimTime <= simEndTime) {
      time_period tickPeriod = time_period(thisTickSimTime, sim_tick_duration_);

      // The process below is a pipline in several independent steps
      // Each step occurs concurrently with that of the (n-Ns+1)th stage.

      // Get weather data from the server
      // STAGE 1
      ProcessWeather(tickPeriod, weatherData);
      
      // Allow the controller to report sprinkler on durations
      // STAGE 2
      ProcessController(tickPeriod, sprinklerDurations);

		// Send the sprinkler times to the yard
		// Here is where actual watering and growing takes place
      // STAGE 3
      ProcessYard(tickPeriod, weatherData, sprinklerDurations, zoneFeedback);

		// Determine whether or not to give feedback to the server
      // STAGE 4
      ProcessFeedback(tickPeriod, zoneFeedback);

		// Wait until tick time is over
		// STAGE 5
		// Compute the simulated time
      thisTickSimTime += (sim_tick_duration_*num_tick_pipes_);
		ProcessWait();
   }
}

inline void Simulator::ProcessWeather(const pt::time_period tickPeriod,
															WeatherData &weatherData) const
{
	std::lock_guard<std::mutex> lock(process_weather_lock_);
	weatherData = weather_source_->GetWeatherData(yard_.locale(), tickPeriod);
}


inline void Simulator::ProcessController(const pt::time_period tickPeriod, 
										sprinkler_durations_t sprinklerDurations)
{
	std::lock_guard<std::mutex> lock(process_controller_lock_);
	controller_->ElapseTime(tickPeriod, sprinklerDurations);
   DbgPrintDurations(sprinklerDurations);
}

inline void Simulator::ProcessYard(pt::time_period tickPeriod,
								const WeatherData &weatherData,
								const sprinkler_durations_t &sprinklerDurations,
								ZoneFeedback_t &zoneFeedback)
{
	std::lock_guard<std::mutex> lock(process_controller_lock_);
	yard_.ElapseTime(tickPeriod, weatherData, sprinklerDurations, true, zoneFeedback);
}


inline void Simulator::ProcessFeedback(pt::time_period tickPeriod,
													const ZoneFeedback_t &zoneFeedback) const
{
	std::lock_guard<std::mutex> lock(process_controller_lock_);
	feedback_sink_->SubmitFeedback(controller_->id(), tickPeriod.end(), zoneFeedback);
}

inline void  Simulator::ProcessWait()
{
	std::lock_guard<std::mutex> lock(process_wait_lock_);

	int now = GetSystemTimeMS();
	int sleepTime = (next_real_tick_end_target_ - now) - SleepModConstant;

	// Don't sleep if we're already behind
	// TODO: Figure out a better way to handle slowdown
	if (sleepTime > 0) {
		SleepForMS(sleepTime);
		real_tick_period -= 1;
		std::cout << "Sped Tick Period: " << real_tick_period << std::endl;
		next_real_tick_end_target_ += real_tick_period;
	} else {
		// Slow down a little
		real_tick_period = static_cast<unsigned int>(ceil(real_tick_period * 1.1f));
		std::cout << "Slowed Tick Period: " << real_tick_period << std::endl;
		next_real_tick_end_target_ = now + real_tick_period;
	}
}

void Simulator::Pause()
{
	// TODO: Implement
}

}}

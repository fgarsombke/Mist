#pragma once

#include "LawnSimStd.h"

#include "YardInfo.h"
#include "Yard.h"
#include "Controller.h"
#include "Feedback.h"
#include "FeedbackSink.h"

using namespace boost::posix_time;
using namespace Mist::Controllers;

namespace Mist { namespace LawnSim {

typedef std::vector<time_duration> sprinkler_durations_t;

class Simulator {
public:
   Simulator(const YardInfo &yardInfo, 
             uPtrController &controller,
             sPtrFeedbackSink feedbackSink,
             sPtrWeatherDataSource weatherSource);

   void Reset(time_period simPeriod, 
				  unsigned int simTickPeriod,
				  unsigned int simSpeedMultiplier);

   void Start();
   void Pause();

   const Yard &yard() const { return yard_; }

private:
	// Reference class only
	Simulator(const Simulator& other);
	Simulator& operator =(Simulator other);

	inline void ProcessWeather(const pt::time_period tickPeriod,
										WeatherData &weatherData) const;

   inline void ProcessController(const pt::time_period tickPeriod, 
											sprinkler_durations_t sprinklerDurations);

	inline void ProcessYard(const pt::time_period tickPeriod,
									const WeatherData &weatherData,
									const sprinkler_durations_t &sprinklerDurations,
									ZoneFeedback_t &zoneFeedback);

	inline void ProcessFeedback(const pt::time_period tickPeriod,
										 const ZoneFeedback_t &zoneFeedback) const;

	inline void ProcessWait();

	void DoTickWork(pt::ptime firstTickTime);

   // Simulation Parameters
   pt::time_period sim_period_;			// Simulated period time in POSIX time
	time_duration sim_tick_duration_;   // Simulated time quantum


	unsigned int next_real_tick_end_target_;
   unsigned int real_tick_period;       // Real tick period.  

   /////////////////////////////
   Yard yard_;
   
   const uPtrController controller_;
   const sPtrFeedbackSink feedback_sink_;
   const sPtrWeatherDataSource weather_source_;

   unsigned int real_start_time_;      // Last Real world (simulator) start time 

	// One mutex for each stage of the pipeline
	mutable std::mutex process_weather_lock_;
	mutable std::mutex process_controller_lock_;
	mutable std::mutex process_yard_lock_;
	mutable std::mutex process_feedback_lock_;
	mutable std::mutex process_wait_lock_;
	
	// Use single threaded version for debug
	static const int num_tick_pipes_ = 
#if _DEBUG_NO_PIPES == 1
		1;
#else
		5;
#endif

	static const int SleepModConstant = 0;
};

}}
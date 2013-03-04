#pragma once

#include "LawnSimStd.h"
#include "GeoLocale.h"

using namespace boost::posix_time;

namespace Mist { namespace LawnSim {

class SimOptions;
typedef std::unique_ptr<SimOptions> uPtrSimOptions;

class SimOptions {
public:
   SimOptions(int argc, char * argv[]);

   bool should_generate_random() const { return generate_random_; }

   // Get functions
   unsigned int sim_tick_period() const { return sim_tick_period_; }
   unsigned int sim_speed_ratio() const { return sim_speed_ratio_; }
   const GeoLocale geo_locale() const {  return geo_locale_; }

   ptime start_time() const { return start_time_; }
   ptime end_time() const { return end_time_; }

private:
   // Ref class only
   SimOptions & operator= (const SimOptions & other);
   SimOptions(const SimOptions& other);

   GeoLocale geo_locale_;

   std::string controller_;

   ptime start_time_;
   ptime end_time_;

   unsigned int sim_tick_period_;       // Length of tick in simulated time in milliseconds
   unsigned int sim_speed_ratio_;       // (simulator speed)/(real time)   

   bool generate_random_;

   std::string config_file_path_;
};

}}
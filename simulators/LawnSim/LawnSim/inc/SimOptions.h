#pragma once

#include "LawnSimStd.h"
#include "GeoLocale.h"

using namespace std;
using namespace boost::posix_time;

namespace LawnSim {

struct SimOptions {
private:
   string controller_;

   ptime start_time_;
   ptime end_time_;

   GeoLocale geo_locale_;

   unsigned int sim_tick_period_;       // Length of tick in simulated time in milliseconds
   unsigned int sim_speed_ratio_;       // (simulator speed)/(real time)   

   bool generate_random_;

   std::string config_file_path_;

   SimOptions & operator= (const SimOptions & other);
   SimOptions(const SimOptions& other);

public:
   SimOptions(int argc, char * argv[]);


   bool should_generate_random() const { return generate_random_; }


   // Get functions
   unsigned int sim_tick_period() const { return sim_tick_period_; }
   unsigned int sim_speed_ratio() const { return sim_speed_ratio_; }
   const GeoLocale geo_locale() const {  return geo_locale_; }

};

}
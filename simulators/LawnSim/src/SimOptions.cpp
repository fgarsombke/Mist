#include "LawnSimStd.h"
#include "Constants.h"

#include "SimOptions.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace bt = boost::posix_time;

using namespace std;

namespace Mist { namespace LawnSim {

SimOptions::SimOptions(int argc, char * argv[]) {
   double latitude;
   double longitude;

   string startDateString;
   string endDateString;

   po::options_description description("Allowed Options");

   description.add_options()
      ("help", "Show help message")
      ("T", po::value<unsigned int>(&sim_tick_period_)->default_value(Constants::DefaultSimTickPeriod), 
         "Tick Period in milliseconds")
      ("Spd", po::value<unsigned int>(&sim_speed_ratio_)->default_value(Constants::DefaultSimSpeedMultiplier), 
         "Simulator speed to real-time ratio")
      ("St", po::value<string>(&startDateString)->required(), "Simulated start date")
      ("End", po::value<string>(&endDateString)->required(), "Simulated end date")
      ("La", po::value<double>(&latitude)->required(), "Longitude to Simulate")
      ("Lo", po::value<double>(&longitude)->required(), "Latitude to Simulate")
      //("Rand", "Generate random configuration")
      //("Conf", po::value<string>(&config_file_path_), "Simulator configuration file path")
   ;

   po::variables_map vm;
   po::store(po::parse_command_line(argc, argv, description), vm);

   if (argc <=1 || vm.count("help")) {
      stringstream ss;
      ss << description << endl;
      throw std::invalid_argument(ss.str());
   }

   /* Raises exceptions for missing options */
   po::notify(vm);

   geo_locale_ = GeoLocale(latitude, longitude);


   // Parse the start and end times
   //

   // Accept a few different formatting options
   const std::locale formats[] = {
      locale(locale::classic(),new bt::time_input_facet("%m/%d/%Y")),
      locale(locale::classic(),new bt::time_input_facet("%m-%d-%Y")),
      locale(locale::classic(),new bt::time_input_facet("%Y-%m-%d %H:%M:%S")),
      locale(locale::classic(),new bt::time_input_facet("%Y/%m/%d %H:%M:%S")),
      locale(locale::classic(),new bt::time_input_facet("%d.%m.%Y %H:%M:%S")),
   };
   const size_t formats_n = sizeof(formats)/sizeof(formats[0]);

   bt::ptime empty_ptime;

   // The format is "2002-01-20 23:59:59.000"
   for(size_t i=0; i<formats_n; ++i)
    {
        std::istringstream is(startDateString);
        is.imbue(formats[i]);
        is >> start_time_;
        if(start_time_ != empty_ptime) break;
    }

   for(size_t i=0; i<formats_n; ++i)
    {
        std::istringstream is(endDateString);
        is.imbue(formats[i]);
        is >> end_time_;
        if(end_time_ != empty_ptime) break;
    }

   if (start_time_ == empty_ptime) {
      throw std::invalid_argument("Start time was in an invalid format.\n");
   }

   if (end_time_ == empty_ptime) {
      throw std::invalid_argument("End time was in an invalid format.\n");
   }
}

}}
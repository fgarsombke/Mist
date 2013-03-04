#include "LawnSimStd.h"

#include "main.h"


#include <boost/program_options/errors.hpp>

#include "GeoLocale.h"
#include "YardInfo.h"
#include "Yard.h"
#include "SimOptions.h"
#include "Simulator.h"
#include "LawnGenerator.h"

#include "Controller.h"

using namespace Mist::LawnSim;
using namespace std;
namespace dt = boost::date_time;
namespace pt = boost::posix_time;
namespace gt = boost::gregorian;


int main(int argc, char * argv[]) 
{
   try {
      // Speed up debugging io
      ios_base::sync_with_stdio(false);

      std::unique_ptr<SimOptions> options = nullptr;
      
      try {
         options.reset(new SimOptions(argc, argv));
      } catch (boost::program_options::error& e) {
         cerr << "Error: " << e.what() << endl;
         return 1;
      }

      auto yardInfo = LawnGenerator().Generate(options->geo_locale(), 5, 5);

      Mist::Controllers::ControllerConfig cConfig;
      cConfig.locale_ = options->geo_locale();
      cConfig.update_period_ = time_duration(hours(1));
      cConfig.data_source_ = "exSchedule.json";
      Mist::Controllers::uPtrController controller = Controller::GetControllerByName("MistFile", cConfig);

      Simulator sim(*yardInfo, controller);
      yardInfo.release();

      pt::ptime start(options->start_time());
      pt::ptime end(options->end_time());

      sim.Reset(start, end, options->sim_tick_period(), options->sim_speed_ratio());
      
      options.release();
      
      sim.Run();
   } catch (std::exception& e) {
      cout << "Fatal Exception: " << e.what() << endl;
      return 1;
   }

   return 0;
} 

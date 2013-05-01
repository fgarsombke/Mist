#include "LawnSimStd.h"

#include "GeoLocale.h"
#include "YardInfo.h"
#include "Yard.h"
#include "SimOptions.h"
#include "Simulator.h"
#include "LawnGenerator.h"
#include "MistDataSource.h"
#include "ETCalcPython.h"

#include "Controller.h"

#include "PAL.h"

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
      // Seed


      std::unique_ptr<SimOptions> options = nullptr;
      
      try {
         options.reset(new SimOptions(argc, argv));
      } catch (boost::program_options::error& e) {
         cerr << "Error: " << e.what() << endl;
         return 1;
      }

      uPtrYardInfo yardInfo;
      
      if (options->config_dir().empty()) {
         yardInfo = LawnGenerator::Generate(options->geo_locale(), 
                                               options->yard_rows(), 
                                               options->yard_cols(), "");
      } else {
         yardInfo = LawnGenerator::LoadYard(options->config_dir());
      }
      
      yardInfo->DebugPrint("yardHeights.csv");
      // Use Mist Data for everything
#if _DEBUG_DATA == 1
		Mist::sPtrMistDataSource mistDataSource = Mist::MistDataSource::GetNullSource();
#else
      Mist::sPtrMistDataSource mistDataSource = Mist::MistDataSource::GetDefaultDataSource();
#endif

      // Build controller configuration
      Mist::Controllers::ControllerConfig cConfig;
      cConfig.Locale = options->geo_locale();
      cConfig.UpdatePeriod = time_duration(hours(1));
      cConfig.numZones = yardInfo->zone_count();
      cConfig.ScheduleSource = mistDataSource;

      Mist::Controllers::uPtrController controller = Controller::GetControllerByName("MistReal", std::move(cConfig));

      Simulator sim(*yardInfo, controller, mistDataSource, mistDataSource);
      yardInfo.release();

      pt::ptime start(options->start_time());
      pt::ptime end(options->end_time());

      sim.Reset(pt::time_period(start, end), options->sim_tick_period(), options->sim_speed_ratio());
      
      options.release();
      
      sim.Start();
   } catch (std::exception& e) {
      cout << "Fatal Exception: " << e.what() << endl;
      return 1;
   } catch (...) {
      cout << "Caught non-std::exception." << endl;
      std::rethrow_exception(std::current_exception());
   }

   return 0;
} 

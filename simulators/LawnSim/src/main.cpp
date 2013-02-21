#include "LawnSimStd.h"

#include "main.h"

#include <fstream>
#include <ostream>

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


void DebugPrintYardInfo(const YardInfo& y);

#ifdef _DEBUG

void DebugPrintYardInfo(const YardInfo& y) 
{
   ofstream dbgFile;
   dbgFile.open("yardExPerlin.csv");

   bnu::matrix<YardCellInfo> cellInfos = y.yard_cells();

   for(unsigned int i = 0; i < y.yard_length(); ++i) {
      for(unsigned int j = 0; j < y.yard_width(); ++j) {
         dbgFile << cellInfos(i,j).rel_height() << ", ";
      }
      dbgFile << endl;
   }

   dbgFile.close();
}


#endif

int main(int argc, char * argv[]) 
{
   try {
      // Speed up debugging output
      ios_base::sync_with_stdio(false);

      std::unique_ptr<SimOptions> options;
      
      try {
         options.reset(new SimOptions(argc, argv));
      } catch (boost::program_options::error& e) {
         cerr << "Error: " << e.what() << endl;
         return 1;
      }

      auto yardInfo = LawnGenerator().Generate(options->geo_locale(), 1000, 1000);
      auto controller  = Mist::Controllers::Controller::GetControllerByName("NullController", Mist::Controllers::ControllerConfig());

      Simulator sim(yardInfo, controller);

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

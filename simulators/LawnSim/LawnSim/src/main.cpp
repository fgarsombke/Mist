#include "LawnSimStd.h"

#include <fstream>
#include <ostream>
#include <iostream>

#include <boost/program_options/errors.hpp>
#include <boost/date_time.hpp>

#include "GeoLocale.h"
#include "Yard.h"
#include "SimOptions.h"
#include "Simulator.h"
#include "LawnGenerator.h"


using namespace Mist::LawnSim;
using namespace std;
namespace dt = boost::date_time;
namespace pt = boost::posix_time;
namespace gt = boost::gregorian;


void DebugPrintYard(const Yard& y) 
{
   ofstream dbgFile;
   dbgFile.open("yardExPerlin.csv");

   for(unsigned int i = 0; i < y.yard_length(); ++i) {
      for(unsigned int j = 0; j < y.yard_width(); ++j) {
         dbgFile << y.GetCellAt(i,j).rel_height() << ", ";
      }
      dbgFile << endl;
   }

   dbgFile.close();
}


int main(int argc, char * argv[]) 
{
   try {
      ios_base::sync_with_stdio(false);


      SimOptions * options = nullptr;
      try {
         options = new SimOptions(argc, argv);
      } catch (boost::program_options::error& e) {
         cerr << "Error: " << e.what() << endl;
         return 1;
      }
   

      LawnGenerator generator;

      unique_ptr<Yard> yard = generator.Generate(options->geo_locale(), 512, 512);


      DebugPrintYard(*yard);

      Simulator sim(*yard);

      pt::ptime start(options->start_time());
      pt::ptime end(options->end_time());

      sim.Reset(start, end, options->sim_tick_period(), options->sim_speed_ratio());

      sim.Run();
   } catch (std::exception& e) {
      cout << "Fatal Exception: " << e.what() << endl;
      return 1;
   }

   return 0;
} 

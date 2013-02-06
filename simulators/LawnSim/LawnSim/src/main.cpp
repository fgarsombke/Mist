#include <fstream>
#include <ostream>
#include <iostream>

#include "LawnSimStd.h"

#include "GeoLocale.h"
#include "Yard.h"
#include "SimOptions.h"
#include "Simulator.h"

#include <boost/program_options/errors.hpp>
#include <boost/date_time.hpp>

using namespace LawnSim;
using namespace std;
namespace dt = boost::date_time;
namespace pt = boost::posix_time;
namespace gt = boost::gregorian;

int main(int argc, char * argv[]) {
   try {
      SimOptions * options = nullptr;
      try {
         options = new SimOptions(argc, argv);
      } catch (boost::program_options::error& e) {
         cerr << "Error: " << e.what() << endl;
         return 1;
      }
   
      const GeoLocale g;
      Yard y(g);

      Simulator sim(y);

      pt::ptime start(gt::date(2013, gt::Jan, 1));
      pt::ptime end(gt::date(2013, gt::Jan, 2));

      sim.Initialize(start, end, options->sim_tick_period(), options->sim_speed_ratio());

      sim.Run();
   } catch (std::exception& e) {
      cout << e.what() << endl;
      return 1;
   }

   return 0;
} 

#pragma once

#include "LawnSimStd.h"

namespace Mist { namespace LawnSim {

enum class SprayPattern {
   UCircle,
};

// Represents a single sprinkler head.
// Sprinkler heads in this simulation all have the same
//    spray pattern.
class Sprinkler {
public:
   // Makes a sprinkler that does not water
   Sprinkler()
      : watering_rate_(0)
   {
   }

   Sprinkler(double wateringRate)
      : watering_rate_(wateringRate)
   {
   }

   // Stored at a rate of mm/s
   //const bm::matrix<double> SprayRates();

private:
   const double watering_rate_;     // In mm/s

};

}}
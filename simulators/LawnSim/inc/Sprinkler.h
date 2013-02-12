#pragma once

#include "LawnSimStd.h"

namespace Mist {
namespace LawnSim {

// Represents a single sprinkler head.
// Sprinkler heads in this simulation all have the same
//    spray pattern.
class Sprinkler {
public:
   Sprinkler(double wateringRate)
      : watering_rate_(wateringRate)
   {
   }


private:
   const double watering_rate_;     // In mm/s

};

}
}
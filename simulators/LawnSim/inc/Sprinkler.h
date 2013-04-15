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

   explicit Sprinkler(double wateringRate)
      : watering_rate_(wateringRate)
   {
   }

   // Stored at a rate of mm/s
   //const bnu::matrix<double> SprayRates();

private:
   const double watering_rate_;     // In mm/s

};

class SprinklerLocation
{
public:
   // Create a sprinkler that does no waterign
   SprinklerLocation()
      : sprinkler_(0.0)
   {
   }

   SprinklerLocation(Sprinkler sprinkler, size_t row, size_t col)
      : sprinkler_(sprinkler), row_(row), col_(col)
   {
   }

private:
   Sprinkler sprinkler_;
   size_t row_;
   size_t col_;
};

}}
#pragma once

#include "LawnSimStd.h"

namespace Mist { namespace LawnSim {

typedef bnu::compressed_matrix<double> SprinklerMask_t;

enum class SprayPattern : int {
   None,
   UCircle,
   Square,
};

// Represents a single sprinkler head.
// Sprinkler heads in this simulation all have the same
//    spray pattern.
class Sprinkler {
public:
   // Makes a sprinkler that does not water
   Sprinkler()
      : watering_rate_(0), pattern_(SprayPattern::None), range_(0)
   {
   }

   Sprinkler(double wateringRate, 
             SprayPattern pattern,
             size_t range)
      : watering_rate_(wateringRate), pattern_(pattern), range_(range)
   {
   }

   size_t range() const { return range_; }
   SprayPattern pattern() const { return pattern_; }
   double watering_rate() const { return watering_rate_; }

private:
   double watering_rate_;     // In mm/s
   SprayPattern pattern_;
   size_t range_;
};

class SprinklerLocation
{
public:
   // Create a sprinkler that does no watering
   SprinklerLocation()
   {
   }

   SprinklerLocation(Sprinkler sprinkler, size_t row, size_t col)
      : sprinkler_(sprinkler), row_(row), col_(col)
   {
   }

   const Sprinkler &sprinkler() const { return sprinkler_; }

   SprinklerMask_t GenerateSprinklerMask(size_t rows, size_t cols) const;

   size_t row() const { return row_; }
   size_t col() const { return col_; }

private:
   Sprinkler sprinkler_;
   size_t row_;
   size_t col_;
};

}}
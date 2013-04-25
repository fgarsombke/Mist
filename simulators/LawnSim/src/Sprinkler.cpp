#include "LawnSimStd.h"
#include "Sprinkler.h"


namespace Mist { namespace LawnSim {

   SprinklerMask_t SprinklerLocation::GenerateSprinklerMask(size_t rows, size_t cols) const{
      // TODO: Implement based on shapes
      SprinklerMask_t mask(rows, cols);

      mask(row_, col_) = 1.0;

      return mask;
   }

}}
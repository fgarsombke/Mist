#include "LawnSimStd.h"
#include "Sprinkler.h"

#include <cmath>
#include <algorithm>


namespace Mist { namespace LawnSim {

   SprinklerMask_t SprinklerLocation::GenerateSprinklerMask(size_t rows, size_t cols) const{
      SprinklerMask_t mask(rows, cols);

      size_t top = std::max(0ULL, row_ - sprinkler_.range());
      size_t bottom = std::min(rows, row_ + sprinkler_.range() + 1);

      size_t left = std::max(0ULL, col_ - sprinkler_.range());
      size_t right = std::min(cols, col_ + sprinkler_.range() + 1);

      switch (sprinkler_.pattern()) {
         case SprayPattern::UCircle: {
            for (size_t i = top; i < bottom; ++i) {
               for (size_t j = left; j < right; ++j) {
                  if (sqrt(pow(i-row_,2) + pow(j-col_,2))) {
                     mask(i,j) = 1.0;
                  }
               }
            }
         }

         case SprayPattern::Square:
         default: {
            for (size_t i = top; i < bottom; ++i) {
               for (size_t j = left; j < right; ++j) {
                  mask(i,j) = 1.0;
               }
            }
          }
     } 

      return mask;
   }

}}
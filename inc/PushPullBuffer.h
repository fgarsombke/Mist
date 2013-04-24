#pragma once

#include "MistStd.h"

namespace Mist {
   template<class data_t>
   class PushPullBuffer
   {
   public:
      PushPullBuffer(size_t rows, size_t cols);

      // Unconditionally copies data
      // Returns true if the data has been pulled since the last push
      bool Push(const bnu::matrix<data_t> &dataSource);

      // Only copies data if new data has been pushed since the last pull
      // Returns true if new data was copied into dataSink
      bool Pull(bnu::matrix<data_t> &dataSink);

   private:
      bnu::matrix<data_t> buffer0_;
      bnu::matrix<data_t> buffer1_;
      
      bool zero_active_;
   };
}
#pragma once

#include "LawnSimStd.h"

#include "YardInfo.h"

namespace Mist {
namespace LawnSim {

typedef void (*FillHeightFunc_t)(bm::matrix<double> &heights);

class LawnGenerator {
public:
   LawnGenerator() {
      
   }

   // Generates a square yard
   std::unique_ptr<YardInfo> Generate(GeoLocale locale, size_t rows, size_t cols) const;

private:
   bm::matrix<YardCellInfo> GenerateCells(size_t rows, 
                                         size_t cols,
                                         FillHeightFunc_t hFunc = FillHeightsDiagonally) const;

   inline static void FillHeightsDiagonally(bm::matrix<double> &heights);
   inline static void FillHeightsPerlin(bm::matrix<double> &heights);

};

}
}
#pragma once

#include "LawnSimStd.h"

#include "YardInfo.h"

namespace Mist {
namespace LawnSim {

typedef void (*FillHeightFunc_t)(bnu::matrix<double> &heights);

class LawnGenerator {
public:
   LawnGenerator() {
      
   }

   // Generates a square yard
   std::unique_ptr<YardInfo> Generate(GeoLocale locale, size_t rows, size_t cols) const;

private:
   bnu::matrix<YardCellInfo> GenerateCells(size_t rows, 
                                         size_t cols,
                                         FillHeightFunc_t hFunc = FillHeightsDiagonally) const;

   static void FillHeightsDiagonally(bnu::matrix<double> &heights);
   static void FillHeightsPerlin(bnu::matrix<double> &heights);

};

}
}
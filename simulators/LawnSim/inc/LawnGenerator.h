#pragma once

#include "LawnSimStd.h"

#include "YardInfo.h"

namespace Mist { namespace LawnSim {

typedef void (*FillHeightFunc_t)(bnu::matrix<double> &heights, std::string param);

class LawnGenerator {
public:
   LawnGenerator() {
      
   }

   // Generates a square yard
   std::unique_ptr<YardInfo> Generate(GeoLocale locale, size_t rows, size_t cols, std::string heightParams) const;

private:
   inline bnu::matrix<YardCellInfo> GenerateCells(size_t rows, 
                                                  size_t cols,
                                                  const SprinklersList_t &sprinklers,
                                                  FillHeightFunc_t hFunc = FillHeightsDiagonally, 
                                                  std::string heightParams = "") const;

   static void FillHeightsDiagonally(bnu::matrix<double> &heights, std::string param);
   static void FillHeightsPerlin(bnu::matrix<double> &heights, std::string param);
   static void FillHeightsFromFile(bnu::matrix<double> &heights, std::string param);
};

}}
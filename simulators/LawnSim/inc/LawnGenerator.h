#pragma once

#include "LawnSimStd.h"

#include "YardInfo.h"

namespace Mist { namespace LawnSim {

typedef bnu::matrix<double> (*FillHeightFunc_t)(std::string param, size_t rows, size_t cols);

class LawnGenerator {
public:
   // Loads a rectangular yard from the file
   static uPtrYardInfo LoadYard(std::string configDir);

   // Generates a square yard
   static uPtrYardInfo Generate(GeoLocale locale, size_t rows, size_t cols, std::string heightParams = "");

private:
   LawnGenerator();
   LawnGenerator(const LawnGenerator& other);
   LawnGenerator& operator=(LawnGenerator other);

   static bnu::matrix<YardCellInfo> GenerateCells(size_t rows, 
                                                  size_t cols,
                                                  const SprinklerList_t &sprinklers,
                                                  std::string heightParams,
                                                  FillHeightFunc_t hFunc = FillHeightsDiagonally);


   static bnu::matrix<double>  FillMatrixFromFile(std::string fileName, size_t rows, size_t cols);
   static SprinklerList_t GenerateDefaultSprinklerList(size_t rows, size_t cols);
   static RainMask_t GenerateDefaultRainMask(size_t rows, size_t cols);

   static bnu::matrix<double> FillHeightsDiagonally(std::string param, size_t rows, size_t cols);
   static bnu::matrix<double> FillHeightsPerlin(std::string param, size_t rows, size_t cols);
};

}}
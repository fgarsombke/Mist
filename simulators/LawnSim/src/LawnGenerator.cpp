#include "LawnSimStd.h"

#include<numeric>
#include <random>
#include <math.h>

#include "LawnGenerator.h"
#include "PerlinNoiseGenerator.h"

using std::unique_ptr;

// TODO: We should probably use something from here:
//    http://lodev.org/cgtutor/randomnoise.html
namespace Mist {
namespace LawnSim {

unique_ptr<YardInfo> LawnGenerator::Generate(GeoLocale locale, size_t rows, size_t cols) const 
{
   if (rows == 0 || cols == 0) {
      throw std::invalid_argument("rows and cols must both be nonzero.");
   }

   // Place 4 sprinklers at each corner
   SprinklersList_t sprinklers = SprinklersList_t(8);
   CellPerZoneList_t cellsPerZone = CellPerZoneList_t(8);

   return unique_ptr<YardInfo>(
      new YardInfo(locale, GenerateCells(rows, cols, sprinklers, FillHeightsDiagonally)
      , sprinklers)
   );
}


inline bnu::matrix<YardCellInfo> LawnGenerator::GenerateCells(size_t rows, 
                                                  size_t cols, 
                                                  const SprinklersList_t &sprinklers,
                                                  FillHeightFunc_t hFunc) const 
{
   bnu::matrix<YardCellInfo> cells(rows, cols, YardCellInfo());
   
   bnu::matrix<double> heights(rows, cols);

   // Generate the heights
   hFunc(heights);

   //TODO URGENT: Compute actual zone!!!!
   for (unsigned int i = 0; i < rows; ++i) {
      for (unsigned int j = 0; j < cols; ++j) {
         cells(i,j) = YardCellInfo(0, heights(i,j), 0);
      }
   }

   return cells;
}

void LawnGenerator::FillHeightsPerlin(bnu::matrix<double> &heights) {
   size_t rows = heights.size1();
   size_t cols = heights.size2();

   Perlin::PerlinModified perGen(10, 1, .01, 5, 0);//time(0));

   for (unsigned int i = 0; i < rows; ++i) {
      for (unsigned int j = 0; j < cols; ++j) {
         heights(i,j) = perGen.GetHeight(j,i);
      }
   }
}

void LawnGenerator::FillHeightsDiagonally(bnu::matrix<double> &heights) {
   size_t rows = heights.size1();
   size_t cols = heights.size2();
   
   typedef std::random_device RNGType;

   RNGType rng;
   //RNGType rng(time(0));

   //size_t featureSize = rows/4;

   double driftparameter = 0;

   double defaultStdev = 0.01;

   std::normal_distribution<double> dist(driftparameter, defaultStdev);

   // Start at a corner and do a random walk to generate a height map
   //
   // Corner (0,0) is always the zero point for relative elevation
   heights(0,0) = 0;

   // Do left and top edges next
   for (size_t j = 1; j < cols; ++j) {
      heights(0,j) = heights(0, j-1) + dist(rng);
   }
   for (size_t i = 1; i < rows; ++i) {
      heights(i,0) = heights(i-1, 0) + dist(rng);
   }

   // Next do the top left half
   for (size_t i = 1; i < rows; ++i) {
      size_t bound = std::min(i, cols);
      for (size_t j = 0; j < bound; ++j) {
         heights(i-j,j+1) = (heights(i-j,j) + heights(i-j-1,j+1))/2 + dist(rng);
      }
   }

   // Finish with the bottom half
   for(size_t i = 2; i < cols; ++i) {
      for (size_t j = 0; j < cols-i; ++ j) {
         heights(rows-1-j, i+j) = (heights(rows-1-j, i+j-1) + heights(rows-2-j, i+j))/2 + dist(rng);
      }
   }
}


}
}
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

unique_ptr<YardInfo> LawnGenerator::Generate(GeoLocale locale, 
                                             size_t rows, 
                                             size_t cols, 
                                             std::string heightParams) const 
{
   if (rows == 0 || cols == 0) {
      throw std::invalid_argument("rows and cols must both be nonzero.");
   }

   // Place 8 sprinklers at each corner
   SprinklersList_t sprinklers = SprinklersList_t(8);
   sprinklers[0] = sprinklers[1] = SprinklerLocation(Sprinkler(1, SprayPattern::Square, 20), 0,0);
   sprinklers[2] = sprinklers[3] = SprinklerLocation(Sprinkler(1, SprayPattern::Square, 20), 0,cols-1);
   sprinklers[4] = sprinklers[5] = SprinklerLocation(Sprinkler(1, SprayPattern::Square, 20), rows-1,0);
   sprinklers[6] = sprinklers[7] = SprinklerLocation(Sprinkler(1, SprayPattern::Square, 20), rows-1,cols-1);

   CellPerZoneList_t cellsPerZone = CellPerZoneList_t(8);

   SprinklerMaskList_t sprinklerMasks( sprinklers.size());
   for (size_t i = 0; i < sprinklers.size(); ++i) {
      sprinklerMasks[i] = sprinklers[i].GenerateSprinklerMask(rows, cols);
   }

   return unique_ptr<YardInfo>(
      new YardInfo(locale, GenerateCells(rows, cols, sprinklers, heightParams, FillHeightsFromFile)
      , sprinklers, sprinklerMasks, RainMask_t(rows, cols, 1.0))
   );
}


inline bnu::matrix<YardCellInfo> LawnGenerator::GenerateCells(size_t rows, 
                                                  size_t cols, 
                                                  const SprinklersList_t &sprinklers,
                                                  std::string heightParams,
                                                  FillHeightFunc_t hFunc) const 
{
   bnu::matrix<YardCellInfo> cells(rows, cols, YardCellInfo());
   
   bnu::matrix<double> heights(rows, cols);

   // Generate the heights
   hFunc(heights, heightParams);

   //TODO URGENT: Compute actual zone!!!!
   for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
         double minDistance = FP_INFINITE;
         size_t minZone = 0;

         // Calculate the zone the slow, painful way
         for (size_t l = 0; l < sprinklers.size(); ++l) {
            double thisDistance = sqrt(pow(i-sprinklers[l].row(),2) + pow(j-sprinklers[l].col(),2));
            if (thisDistance < minDistance) {
               minDistance = thisDistance;
               minZone = l;
            }
         }

         cells(i,j) = YardCellInfo(0, heights(i,j), minZone);
      }
   }

   return cells;
}

void LawnGenerator::FillHeightsFromFile(bnu::matrix<double> &heights, std::string param) {
   std::ifstream heightsFile(param);

   std::string line;
   size_t rowNum = 0;
   size_t colNum;
   while(std::getline(heightsFile, line)) {
      std::stringstream lineStream(line);
      std::string data;

      colNum = 0;
      while(std::getline(lineStream, data, ',')) {
         heights(rowNum, colNum) = std::strtod(data.c_str(), nullptr);
         ++colNum;
      }
      ++rowNum;
   }

   if (!((rowNum == heights.size1()) && (colNum == heights.size2()))) {
      throw new std::logic_error("The file " + param + " did not contain enough lawn cell heights.\n");
   }
}

void LawnGenerator::FillHeightsPerlin(bnu::matrix<double> &heights, std::string param) {
   size_t rows = heights.size1();
   size_t cols = heights.size2();

   Perlin::PerlinModified perGen(10, 1, .01, 5, 0);//time(0));

   for (unsigned int i = 0; i < rows; ++i) {
      for (unsigned int j = 0; j < cols; ++j) {
         heights(i,j) = perGen.GetHeight(j,i);
      }
   }
}

void LawnGenerator::FillHeightsDiagonally(bnu::matrix<double> &heights, std::string param) {
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
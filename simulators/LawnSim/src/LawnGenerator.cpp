#include "LawnSimStd.h"

#include <numeric>
#include <random>
#include <math.h>

#include "boost/optional.hpp"

#include "LawnGenerator.h"
#include "PerlinNoiseGenerator.h"



using std::unique_ptr;

// TODO: We should probably use something from here:
//    http://lodev.org/cgtutor/randomnoise.html
namespace Mist { namespace LawnSim {

namespace LawnGeneratorInternal {
   // Special File Names
   //
   const std::string ConfigFilename = "config.json";
}

unique_ptr<YardInfo> LawnGenerator::Generate(GeoLocale locale, 
                                             size_t rows, 
                                             size_t cols, 
                                             std::string heightParams) 
{
   if (rows == 0 || cols == 0) {
      throw std::invalid_argument("rows and cols must both be nonzero.");
   }

   SprinklerList_t sprinklers = GenDefaultSprinklerList(rows, cols);

   return unique_ptr<YardInfo>(
      new YardInfo(
         fs::path("."),
         locale, 
         GenerateCells(rows, cols, sprinklers, SunlightFractions_t(rows,cols,1.0), heightParams, FillHeightsPerlin), 
         sprinklers, 
         GenDefaultSprinklerMaskList(rows, cols, sprinklers), 
         GenDefaultRainMask(rows, cols))
   );
}


bnu::matrix<YardCellInfo> LawnGenerator::GenerateCells(size_t rows, 
                                                  size_t cols, 
                                                  const SprinklerList_t &sprinklers,
                                                  SunlightFractions_t sunlightFractions,
                                                  std::string heightParams,
                                                  FillHeightFunc_t hFunc)
{
   bnu::matrix<YardCellInfo> cells(rows, cols, YardCellInfo());
   
   // Generate the heights
   bnu::matrix<double> heights = hFunc( heightParams, rows, cols);

   for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
         double minDistance = std::numeric_limits<double>::infinity();
         zone_number_t minZone = 0;

         // Calculate the zone the slow, painful way
         for (size_t l = 0; l < sprinklers.size(); ++l) {
            double thisDistance = sqrt(pow(i-sprinklers[l].row(),2) + pow(j-sprinklers[l].col(),2));
            if (thisDistance < minDistance) {
               minDistance = thisDistance;
               minZone = l;
            }
         }

         cells(i,j) = YardCellInfo(0, heights(i,j), minZone, sunlightFractions(i,j));
      }
   }

   return cells;
}

// Load a yard from the config file located in configDir
uPtrYardInfo LawnGenerator::LoadYard(fs::path configDir) 
{
   // We load the yard from various files in the directory
   using namespace LawnGeneratorInternal;

   // First read in the config information directly
   using namespace boost::property_tree;
   namespace fs = boost::filesystem;

   std::string configFileName((configDir / fs::path(ConfigFilename)).string());

   ptree configTree;
   read_json(configFileName, configTree);

   double latitude = configTree.get<double>("latitude");
   double longitude = configTree.get<double>("longitude");

   size_t rows = configTree.get<size_t>("rows");
   size_t cols = configTree.get<size_t>("cols");

   std::string heightsFileName(configTree.get("heightsFile", ""));
   std::string rainMaskFileName(configTree.get("rainMaskFile", ""));
   std::string sunlightFracsFileName(configTree.get("sunlightFractionsFile", ""));

   auto sprinklersTree = configTree.get_child_optional("sprinklers");

   std::string heightsPath;
   FillHeightFunc_t fillFunc;

   if (heightsFileName.length() > 0) {
      fillFunc = FillMatrixFromFile;
      heightsPath = (configDir / fs::path(heightsFileName)).string();
   } else {
      fillFunc = FillHeightsPerlin;
   }

   SunlightFractions_t sunlightFractions;
   if (sunlightFracsFileName.length() > 0) {
      sunlightFractions = FillMatrixFromFile((configDir / fs::path(sunlightFracsFileName)).string(), rows, cols);
   } else {
      sunlightFractions = SunlightFractions_t(rows,cols,1.0);
   }
   
   RainMask_t rainMask;
   if (rainMaskFileName.length() > 0) {
      rainMask = FillMatrixFromFile((configDir / fs::path(rainMaskFileName)).string(), rows, cols);
   } else {
      rainMask = GenDefaultRainMask(rows, cols);
   }

   SprinklerList_t sprinklers(0);
   SprinklerMaskList_t sprinklerMasks(0);

   if (sprinklersTree.is_initialized()) {
      for (const ptree::value_type &t : sprinklersTree.get()) {
         ptree sprinklerTree = t.second;

         size_t row = sprinklerTree.get<size_t>("row");
         size_t col = sprinklerTree.get<size_t>("col");
         double wateringRate = sprinklerTree.get<double>("wateringRate");
         SprayPattern pattern = (SprayPattern)sprinklerTree.get<int>("sprayPattern");
         size_t range = sprinklerTree.get<size_t>("range");
         std::string maskFileName(configTree.get("maskFile", ""));

         // Generate the sprinkler without specifying the mask
         SprinklerLocation s(Sprinkler(wateringRate, pattern, range), row, col);

         if (maskFileName.length() > 0) {
            sprinklerMasks.push_back(FillMatrixFromFile((configDir / fs::path(maskFileName)).string(), rows, cols));
         } else {
            sprinklerMasks.push_back(s.GenerateSprinklerMask(rows, cols));
         }
      
         sprinklers.push_back(s); 
      }
   }

   if (sprinklers.size() == 0) {
      sprinklers = GenDefaultSprinklerList(rows, cols);
      sprinklerMasks = GenDefaultSprinklerMaskList(rows, cols, sprinklers);
   }
   
   bnu::matrix<YardCellInfo> yardCells =
      GenerateCells(rows, cols, sprinklers, sunlightFractions, heightsPath, fillFunc);

   return unique_ptr<YardInfo>( 
      new YardInfo(configDir,
                   GeoLocale(latitude, longitude), 
                   yardCells, sprinklers, sprinklerMasks, rainMask)
   );
}

// Places 8 sprinklers on the lawn with two at each corner
SprinklerList_t LawnGenerator::GenDefaultSprinklerList(size_t rows, size_t cols) 
{
   // Place 8 sprinklers at each corner
   SprinklerList_t sprinklers = SprinklerList_t(8);
   sprinklers[0] = sprinklers[1] = SprinklerLocation(Sprinkler(1, SprayPattern::Square, 20), 0,0);
   sprinklers[2] = sprinklers[3] = SprinklerLocation(Sprinkler(1, SprayPattern::Square, 20), 0,cols-1);
   sprinklers[4] = sprinklers[5] = SprinklerLocation(Sprinkler(1, SprayPattern::Square, 20), rows-1,0);
   sprinklers[6] = sprinklers[7] = SprinklerLocation(Sprinkler(1, SprayPattern::Square, 20), rows-1,cols-1);

   return sprinklers;
}

// Generates a rain mask that is just a constant matrix with entries 1.0
RainMask_t LawnGenerator::GenDefaultRainMask(size_t rows, size_t cols)
{
   return bnu::matrix<double>(rows, cols, 1.0);
}

SprinklerMaskList_t LawnGenerator::GenDefaultSprinklerMaskList(size_t rows, size_t cols,
                                                                    const SprinklerList_t &sprinklers)
{
   SprinklerMaskList_t sprinklerMasks(sprinklers.size());
   for (size_t i = 0; i < sprinklers.size(); ++i) {
      sprinklerMasks[i] = sprinklers[i].GenerateSprinklerMask(rows, cols);
   }

   return sprinklerMasks;
}

bnu::matrix<double> LawnGenerator::FillMatrixFromFile(std::string fileName, size_t rows, size_t cols) {
   std::ifstream heightsFile(fileName);

   bnu::matrix<double> dataOut(rows, cols);

   std::string line;
   size_t rowNum = 0;
   size_t colNum = 0;
   while(std::getline(heightsFile, line) && (rowNum<dataOut.size1())) {
      std::stringstream lineStream(line);
      std::string data;

      colNum = 0;
      while(std::getline(lineStream, data, ',') && (colNum<dataOut.size2())) {
         dataOut(rowNum, colNum) = std::strtod(data.c_str(), nullptr);
         ++colNum;
      }
      ++rowNum;
   }

   if (!((rowNum == rows) && (colNum == cols))) {
      throw new std::logic_error("The file " + fileName + " did not contain enough data.\n");
   }

   return dataOut;
}

bnu::matrix<double> LawnGenerator::FillHeightsPerlin(std::string param, size_t rows, size_t cols) {
   bnu::matrix<double> heights(rows, cols);

   Perlin::PerlinModified perGen(10, 1, .01, 5, 0);//time(0));

   for (unsigned int i = 0; i < rows; ++i) {
      for (unsigned int j = 0; j < cols; ++j) {
         heights(i,j) = perGen.GetHeight(j,i);
      }
   }

   return heights;
}

bnu::matrix<double> LawnGenerator::FillHeightsDiagonally(std::string param, size_t rows, size_t cols) {
   bnu::matrix<double> heights(rows, cols);
   
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

   return heights;
}


}}
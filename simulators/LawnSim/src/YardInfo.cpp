#include "LawnSimStd.h"

#include "YardInfo.h"

#include <fstream>
#include <ostream>

namespace Mist { namespace LawnSim {

CellPerZoneList_t YardInfo::InitCellsPerZone(const bnu::matrix<YardCellInfo> &yardCells)
{
   CellPerZoneList_t cellsPerZone;

   // Find the number of zones
   size_t numZones = 0;
   for(const YardCellInfo &cell : yardCells.data()) {
      if (cell.zone() > numZones) {
         numZones = cell.zone();
      }
   }

   cellsPerZone.resize(numZones+1);
   // Compute the number of cells in each zone
   for(const YardCellInfo &cell : yardCells.data()) {
      ++cellsPerZone[cell.zone()];
   }

   return cellsPerZone;
}


#ifdef _DEBUG
void YardInfo::DebugPrint(const std::string &fileName) const
{
   using namespace std;

   ofstream dbgFile;

   const std::string &name = fileName.length() == 0? "YardExPerlin.csv": fileName;

   dbgFile.open(name);

   bnu::matrix<YardCellInfo> cellInfos = this->yard_cells();

   for(unsigned int i = 0; i < this->yard_length(); ++i) {
      for(unsigned int j = 0; j < this->yard_width() - 1; ++j) {
         dbgFile << cellInfos(i,j).rel_height() << ", ";
      }
      dbgFile << cellInfos(i,this->yard_width() - 1).rel_height() << endl;
   }

   dbgFile.close();
}

#else
void YardInfo::DebugPrint(const std::string &fileName) const
{
}

#endif

}}
#include "LawnSimStd.h"

#include "Yard.h"

namespace Mist {
namespace LawnSim {


   
Yard::Yard(const YardInfo& yardInfo)
   : cells_(InitCells(yardInfo)), drift_map_(InitDriftMap(cells_))
{
   // Zero everything out


   // Generate a height sorted view of the 


}


   
const bm::matrix<YardCell> Yard::InitCells(const YardInfo& yardInfo)
{
   bm::matrix<YardCell> cells = bm::matrix<YardCell>(yardInfo.yard_length(), yardInfo.yard_width(), YardCell());
   auto cellInfos = yardInfo.yard_cells();

   auto cSt = cells.begin1();
   for (YardCellInfo &cellInfo: cellInfos.data()) {
      *cSt = YardCell(cellInfo);
      ++cSt;
   }

   return cells;
}

const bm::unbounded_array<DriftEntry> Yard::InitDriftMap(const bm::matrix<YardCell> &cells) {
   bm::unbounded_array<DriftEntry> retVal(cells.data().size(), DriftEntry());

   // Fill in the drift map

   // Sort the drift map by height
   auto heightLambda = [&cells](const DriftEntry &lPos, 
                                const DriftEntry &rPos) {
      double diff =  cells.data()[lPos.yard_location()].cell_info().rel_height() -
                     cells.data()[rPos.yard_location()].cell_info().rel_height();
      return (diff < 0? floor(diff): ceil(diff));
   };

   std::sort(retVal.begin(), retVal.end(), heightLambda);  

   return retVal;
}


void Yard::ElapseTime(pt::time_period interval) {
   
   // On every Tick, we need to deliver the right amount of water to each cell

   // Water    
   
   // Possibly rain
   // Shine sunlight
   // Apply heat
   // Apply humidity
   // Blow wind

   // Grow 
}








}
}


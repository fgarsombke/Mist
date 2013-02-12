#include "LawnSimStd.h"

#include "Yard.h"
#include "YardCell.h"


namespace Mist {
namespace LawnSim {

   
Yard::Yard(const YardInfo& yardInfo)
   : cells_(InitCells(yardInfo)) 
{
   // Zero everything out


}


   
bm::matrix<YardCell> Yard::InitCells(const YardInfo& yardInfo)
{
   bm::matrix<YardCell> cells = bm::matrix<YardCell>(yardInfo.yard_length(), yardInfo.yard_width(), YardCell());
   auto cellInfos = yardInfo.yard_cells();

   auto cSt = cells.begin1();
   for (YardCellInfo &cellInfo: cellInfos.begin1()) {
      *cSt = YardCell(cellInfo);
      ++cSt;
   }


   return cells;
}










}
}


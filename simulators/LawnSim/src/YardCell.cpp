#include "LawnSimStd.h"

#include "YardCell.h"

namespace Mist { namespace LawnSim {


YardCell::YardCell(YardCellInfo info, NeighborHeightDiffs_t &heightDiffs) 
   : cell_info_(info), drift_entry_(DriftEntry(heightDiffs))
{
}

YardCell::YardCell()
   : cell_info_(YardCellInfo()), drift_entry_(DriftEntry())
{
}




}}
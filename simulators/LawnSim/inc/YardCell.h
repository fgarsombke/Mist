#pragma once

#include "LawnSimStd.h"

#include "YardCellInfo.h"
#include "DriftEntry.h"

namespace Mist {
namespace LawnSim {

class YardCell {
public:
   YardCell(YardCellInfo info, NeighborHeightDiffs_t &heightDiffs);


   YardCell();


   const YardCellInfo cell_info() const { return cell_info_; }
   const DriftEntry drift_entry() const { return drift_entry_; }
private:
   YardCellInfo cell_info_;

   double current_health_;
   DriftEntry drift_entry_;

   // Water soaks down into the depths
   double water_saturation_;

};

















}
}
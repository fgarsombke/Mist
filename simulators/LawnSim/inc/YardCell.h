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

   void ResetState();




   const YardCellInfo cell_info() const { return cell_info_; }
   const DriftEntry drift_entry() const { return drift_entry_; }
private:
   size_t yard_position_;

   YardCellInfo cell_info_;
   DriftEntry drift_entry_;

   double current_health_;
   
   // The yard cell only gets water soaked into the soil
   double absorbed_water_;

};

















}
}
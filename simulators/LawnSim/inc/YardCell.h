#pragma once

#include "LawnSimStd.h"

#include "YardCellInfo.h"
#include "DriftEntry.h"
#include "ETCalc.h"

namespace Mist { namespace LawnSim {

enum class YardCellType_t {
   Grass,
   Void,
   Isolated,
};

class YardCell {
public:
   static YardCell CreateVoid(double relHeight);
   static YardCell CreateGrass(YardCellInfo info, NeighborHeightDiffs_t &heightDiffs);
   static YardCell CreateIsolated(YardCellInfo info);

   YardCell();

   void UnIsolate(NeighborHeightDiffs_t heightDiffs);

   void ResetState();

   // Increase height by delta
   void ChangeHeight(double delta);

   const YardCellInfo cell_info() const { return cell_info_; }
   YardCellType_t cell_type() const { return cell_type_; }
   const DriftEntry drift_entry() const;
private:
   YardCellInfo cell_info_;
   
   YardCellType_t cell_type_;
   
   DriftEntry drift_entry_;
   
   // The yard cell only gets water soaked into the soil
   double absorbed_water_;
   
   YardCell(YardCellInfo info, YardCellType_t type);

   YardCell(YardCellInfo info, YardCellType_t type, NeighborHeightDiffs_t &heightDiffs);

   // Returns the amount of water lost from the soil
   double Grow(double waterSupplied, const ETCalc::ETCalcParameters &etParams);
};



}}
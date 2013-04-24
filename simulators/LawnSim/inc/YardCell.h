#pragma once

#include "LawnSimStd.h"

#include "YardCellInfo.h"
#include "DriftEntry.h"
#include "ETCalc.h"

namespace Mist { namespace LawnSim {
   
typedef Mist::ETCalc::ET_float_t water_mm_t;

enum class YardCellType_t {
   Grass,
   Void,
   Isolated,
   Dead,
};

class YardCell {
public:
   // Static factory methods
   static YardCell CreateVoid(double relHeight);
   static YardCell CreateIsolated(YardCellInfo info);

   static health_t ComputeHealthMetric(health_t currentHealth, 
                                       water_mm_t waterAvailable, 
                                       double periodLengthDays,
                                       double growthFactor);

   // Default Constructor
   YardCell();

   void UnIsolate(NeighborHeightDiffs_t heightDiffs, zone_number_t zone);

   // Increase height by delta
   void ChangeHeight(double delta);
   void ResetState();

   void Kill() { cell_type_ = YardCellType_t::Dead; }

   const YardCellInfo cell_info() const { return cell_info_; }
   const YardCellType_t cell_type() const { return cell_type_; }

	// Cell K value
	double ET_K() const { return 1.0; }

   const DriftEntry drift_entry() const;
private:
   YardCellInfo cell_info_;
   
   YardCellType_t cell_type_;
   
   DriftEntry drift_entry_;

   zone_number_t zone_;
   
   // The yard cell only gets water soaked into the soil
   double absorbed_water_;
   
   YardCell(YardCellInfo info, YardCellType_t type);

   YardCell(YardCellInfo info, YardCellType_t type, NeighborHeightDiffs_t &heightDiffs);
};



}}
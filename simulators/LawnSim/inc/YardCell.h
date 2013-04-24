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

   void UnIsolate(NeighborHeightDiffs_t heightDiffs);

   // Increase height by delta
   void ChangeHeight(double delta);
   void ResetState();

   void Kill() { cell_type_ = YardCellType_t::Dead; }

   const YardCellType_t cell_type() const { return cell_type_; }
   zone_number_t zone() const { return cell_info_.zone(); }
   double rel_height() const { return cell_info_.rel_height(); }
   double sunlight_fraction() const { return cell_info_.sunlight_fraction(); }


	// Cell K value
	double ET_K() const;
   bool HasHealth() const;

   const DriftEntry drift_entry() const;
private:
   YardCellInfo cell_info_;  
   YardCellType_t cell_type_; 
   DriftEntry drift_entry_;
   
   // The yard cell only gets water soaked into the soil
   double absorbed_water_;
   
   YardCell(YardCellInfo info, YardCellType_t type);

   YardCell(YardCellInfo info, YardCellType_t type, NeighborHeightDiffs_t &heightDiffs);
};



}}
#pragma once

#include "LawnSimStd.h"

#include "YardInfo.h"
#include "YardCell.h"
#include "DriftEntry.h"
#include "WeatherData.h"
#include "LawnCoordinate.h"
#include "Feedback.h"

#include "Controller.h"

namespace Mist { namespace LawnSim {

class Yard {
public:
   explicit Yard(const  YardInfo& yardInfo);

   const bnu::matrix<YardCell> &cells() const { return cells_; }

   void ElapseTime(pt::time_period tickPeriod, 
                  const WeatherData &data, 
                  const std::vector<pt::time_duration> &sprinklerDurations, 
                  bool addFeedback,
                  ZoneFeedback_t &feedbackByZone);

   void ResetState();

   size_t ZoneCount() const { return SprinklersCount(); }
   size_t SprinklersCount() const { return sprinklers_.size(); }
   const GeoLocale locale() const { return locale_; }

   void DebugPrint() const;
   void DebugPrintHeights(std::string fileName) const;

private:
   inline const bnu::matrix<YardCell> InitCells(const YardInfo& yardInfo);
   inline void ComputeFeedback(pt::time_period tickPeriod, ZoneFeedback_t &feedbackByZone) const;
   inline void DoGrow(ETCalc::ETParam_t ET_0, 
                      double growthFactor,
                      size_t startCell, 
                      size_t count);

   // Determines in which zen
   inline size_t ComputeZone(size_t row, size_t) const;

   const fs::path config_dir_;
   const GeoLocale locale_;

   // The sprinklers in the yard, along with their positions in the yard
   const SprinklerList_t sprinklers_;

   // Cells in the yard
   bnu::matrix<YardCell> cells_;

   // Stores the indices of the yardcells in order of decreasing height
   const bnu::unbounded_array<LawnCoordinate> cells_by_height_;
   
   const CellPerZoneList_t cells_per_zone_;

   // The sprinkler "masks" which indicate how much water is to be delivered to each cell
   // The units are mm/s, so each matrix needs to be multiplied by time duration when watering
   // We assume here that any deviation from ideal in the sprinkler spray pattern is time invariant
   const SprinklerMaskList_t sprinkler_masks_;

   // Contains fraction rain amoun modifiers so that rain will be distributed unevenly across the yard
   const RainMask_t rain_mask_;


   // The water on the surface across the yard, which has not yet been absorbed into the soil
   bnu::matrix<water_mm_t> surface_water_;

   // Matrix which holds the current health of each lawn cell
   bnu::matrix<health_t> cell_health_;

   ETCalc::ETCalc et_calc_;



   SprinklerMaskList_t InitSprinklerMasks(const YardInfo &yardInfo);

   

   template<class M>
   static void DebugPrintMatrix(const M &toPrint, const fs::path &fileName);

   static const bnu::unbounded_array<LawnCoordinate> InitHeightMap(YardInfo const &yardInfo);
};


}}


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

typedef bnu::compressed_matrix<double> SprinklerMask_t;
typedef bnu::unbounded_array<SprinklerMask_t> SprinklerMaskList_t;

class Yard {
public:
   explicit Yard(const YardInfo& yardInfo);

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

   const GeoLocale locale_;

   // The sprinklers in the yard, along with their positions in the yard
   const SprinklersList_t sprinklers_;

   // Cells in the yard
   bnu::matrix<YardCell> cells_;

   // Stores the indices of the yardcells in order of decreasing height
   const bnu::unbounded_array<LawnCoordinate> cells_by_height_;
   
   const CellPerZoneList_t cells_per_zone_;

   
   // The water on the surface across the yard, which has not yet been absorbed into the soil
   bnu::matrix<water_mm_t> surface_water_;

   // Matrix which holds the current health of each lawn cell
   bnu::matrix<health_t> cell_health_;

   ETCalc::ETCalc et_calc_;

   // The sprinkler "masks" which indicate how much water is to be delivered to each cell
   // The units are mm/s, so each matrix needs to be multiplied by time duration when watering
   // We assume here that any deviation from ideal in the sprinkler spray pattern is time invariant
   SprinklerMaskList_t sprinkler_masks_;

   // Contains fraction rain amoun modifiers so that rain will be distributed unevenly across the yard
   // TODO: Possibly get a more sophistocated mask
   bnu::scalar_matrix<double> rain_mask_;




   SprinklerMaskList_t InitSprinklerMasks(const YardInfo &yardInfo);

   

   template<class T>
   static void DebugPrintMatrix(const bnu::matrix<T> &toPrint, std::string fileName);

   static const bnu::unbounded_array<LawnCoordinate> InitHeightMap(YardInfo const &yardInfo);
};


}}


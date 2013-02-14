#pragma once

#include "LawnSimStd.h"

#include "YardInfo.h"
#include "YardCell.h"
#include "DriftEntry.h"
#include "WeatherData.h"
#include "GeoLocale.h"

#include "Controller.h"

namespace Mist { namespace LawnSim {

typedef bm::compressed_matrix<double> SprinklerMask_t;
typedef bm::unbounded_array<SprinklerMask_t> SprinklerMaskList_t;



class Yard {
public:
   Yard(const YardInfo& yardInfo);

   const bm::matrix<YardCell> &cells() const { return cells_; }

   void ElapseTime(pt::time_period tickPeriod, const WeatherData &data, std::vector<pt::time_duration> sprinklerDurations);

   void ResetState();

   size_t SprinklersCount() const { return sprinklers_.size(); }
   const GeoLocale locale() const { return locale_; }

private:
   const GeoLocale locale_;

   bm::matrix<YardCell> cells_;

   // The sprinklers in the yard, along with their positions in the yard
   const SprinklersList_t sprinklers_;

   // Stores the indices of the yardcells in order of decreasing height
   const bm::unbounded_array<size_t> cells_by_height_;

   // The sprinkler "masks" which indicate how much water is to be delivered to each cell
   // The units are mm/s, so each matrix needs to be multiplied by time duration when watering
   // We assume here that any deviation from ideal in the sprinkler spray pattern is time invariant
   SprinklerMaskList_t sprinkler_masks_;

   // Contains fraction rain amoun modifiers so that rain will be distributed unevenly across the yard
   // TODO: Possibly get a more sophistocated mask
   bm::scalar_matrix<double> rain_mask_;

   // The water on the surface across the yard, which has not yet been absorbed into the soil
   bm::matrix<double> surface_water_;


   SprinklerMaskList_t InitSprinklerMasks(const YardInfo &yardInfo);

   static const bm::matrix<YardCell> InitCells(const YardInfo& yardInfo);
   static const bm::unbounded_array<size_t> InitHeightMap(const bm::matrix<YardCell> &cells);
};









}
}


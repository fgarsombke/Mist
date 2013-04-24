#pragma once

#include "LawnSimStd.h"
#include "YardCellInfo.h"
#include "Sprinkler.h"

#include <math.h>

namespace Mist { namespace LawnSim {

typedef bnu::unbounded_array<SprinklerLocation> SprinklersList_t;
typedef bnu::unbounded_array<size_t> CellPerZoneList_t;

class YardInfo
{
public:
   const bnu::matrix<YardCellInfo> &yard_cells() const { return yard_cells_; }

   size_t yard_length() const { return yard_cells_.size1(); }
   size_t yard_width() const { return yard_cells_.size2(); }

   const GeoLocale locale() const { return locale_; }

   const SprinklersList_t &sprinklers() const 
   { return sprinklers_; }

   const CellPerZoneList_t &cells_per_zone() const
   { return cells_per_zone_; }

   void DebugPrint(const std::string &fileName) const;

private: 
   friend class LawnGenerator;  
   static CellPerZoneList_t InitCellsPerZone(const bnu::matrix<YardCellInfo> &yardCells);

   const GeoLocale locale_;
   const bnu::matrix<YardCellInfo> yard_cells_;

   // The sprinklers in the yard, along with their positions in the yard
   const SprinklersList_t sprinklers_;
   const CellPerZoneList_t cells_per_zone_;

    YardInfo(const GeoLocale locale, 
             const bnu::matrix<YardCellInfo> yardCells, 
             const SprinklersList_t sprinklers)
      : locale_(locale), yard_cells_(yardCells), sprinklers_(sprinklers), cells_per_zone_(InitCellsPerZone(yard_cells_))
   {
   }

};


}}
#pragma once

#include "LawnSimStd.h"
#include "YardCellInfo.h"
#include "Sprinkler.h"

#include "GeoLocale.h"

namespace Mist { namespace LawnSim {

typedef bnu::unbounded_array<std::pair<Sprinkler, std::pair<size_t,size_t> > > SprinklersList_t;

class YardInfo
{
public:
   const bnu::matrix<YardCellInfo> &yard_cells() const { return yard_cells_; }


   size_t yard_length() const { return yard_cells_.size1(); }
   size_t yard_width() const { return yard_cells_.size2(); }

   const GeoLocale locale() const { return locale_; }

   const SprinklersList_t &sprinklers() const 
   { return sprinklers_; }


   void DebugPrint(const std::string &fileName) const;

private: 
   friend class LawnGenerator;  
   const GeoLocale locale_;
   
   const bnu::matrix<YardCellInfo> yard_cells_;

   // The sprinklers in the yard, along with their positions in the yard
   const SprinklersList_t sprinklers_;

    YardInfo(const GeoLocale locale, 
             const bnu::matrix<YardCellInfo> yardCells, 
             const SprinklersList_t sprinklers)
      : locale_(locale), yard_cells_(yardCells), sprinklers_(sprinklers)
   {
   }
};


}}
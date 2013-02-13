#pragma once

#include "LawnSimStd.h"

#include "YardInfo.h"
#include "YardCell.h"
#include "DriftEntry.h"
#include "WeatherData.h"
#include "GeoLocale.h"

namespace Mist {
namespace LawnSim {

class Yard {
public:
   Yard(const YardInfo& yardInfo);

   const bm::matrix<YardCell> &cells() const { return cells_; }

   void ElapseTime(const WeatherData &data);

   const GeoLocale locale() const { return locale_; }

private:
   bm::matrix<YardCell> cells_;

   // Stores the indices of the yardcells in order decreasing height
   const bm::unbounded_array<size_t> cells_by_height_;
   const GeoLocale locale_;






   
   static const bm::matrix<YardCell> InitCells(const YardInfo& yardInfo);
   static const bm::unbounded_array<size_t> InitHeightMap(const bm::matrix<YardCell> &cells);



};









}
}


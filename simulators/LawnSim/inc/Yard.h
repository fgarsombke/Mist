#pragma once

#include "LawnSimStd.h"

#include "YardInfo.h"
#include "YardCell.h"
#include "DriftEntry.h"

namespace Mist {
namespace LawnSim {

class Yard {
public:
   Yard(const YardInfo& yardInfo);

   const bm::matrix<YardCell> &cells() const { return cells_; }

   void ElapseTime(pt::time_period interval);

private:
   const bm::matrix<YardCell> cells_;

   // Stores the indices of the yardcells in order decreasing height
   const bm::unbounded_array<size_t> cells_by_height_;







   
   static const bm::matrix<YardCell> InitCells(const YardInfo& yardInfo);
   static const bm::unbounded_array<size_t> InitHeightMap(const bm::matrix<YardCell> &cells);



};









}
}


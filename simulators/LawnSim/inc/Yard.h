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


   void ElapseTime(pt::time_period interval);

private:
   const bm::matrix<YardCell> cells_;

   // Stores the indices of the yardcells
   const bm::unbounded_array<DriftEntry> drift_map_;







   
   static const bm::matrix<YardCell> InitCells(const YardInfo& yardInfo);
   static const bm::unbounded_array<DriftEntry> InitDriftMap(const bm::matrix<YardCell> &cells);



};









}
}


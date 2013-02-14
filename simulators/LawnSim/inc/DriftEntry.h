#pragma once

#include "LawnSimStd.h"
#include "NeighborData.h"

namespace Mist { namespace LawnSim {

class DriftEntry : public NeighborData<double> {
public:
   DriftEntry() 
      : NeighborData<double>()
   {
   }

   DriftEntry(NeighborHeightDiffs_t &heightDiffs);
};



















}}

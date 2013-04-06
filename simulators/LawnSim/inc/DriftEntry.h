#pragma once

#include "LawnSimStd.h"
#include "NeighborData.h"

namespace Mist { namespace LawnSim {

class DriftEntry : public NeighborData<double> 
{
public:
   DriftEntry() 
      : NeighborData<double>()
   {
   }

   explicit DriftEntry(NeighborHeightDiffs_t &heightDiffs);
};




std::ostream& operator<<(std::ostream &out, DriftEntry lc);


}}


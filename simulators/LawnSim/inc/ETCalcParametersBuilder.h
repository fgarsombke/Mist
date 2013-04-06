#pragma once

#include "LawnSimStd.h"
#include "ETCalcParameters.h"

namespace Mist { namespace FAO_ET {
   
namespace ETCalcParametersBuilderInternal {
   struct DataEntry
   {
      ET_float_t Data;
      bool Set;
   };
}

using namespace ETCalcParametersBuilderInternal;

class ETCalcParametersBuilder 
{
public:
   explicit ETCalcParametersBuilder(pt::time_period interval)
      :interval_(interval)
   {
   }

   void SetDataValue(ETCalcData type, ET_float_t data) 
   {
      // Assign the value and indicate that it has been assigned
      data_entries_[static_cast<size_t>(type)].Data = data;
      data_entries_[static_cast<size_t>(type)].Set = true;
   }

   ETCalcParameters Build() const;

private:
   pt::time_period interval_;

   DataEntry data_entries_[static_cast<size_t>(ETCalcData::NumDataTypes)];
};




}} 
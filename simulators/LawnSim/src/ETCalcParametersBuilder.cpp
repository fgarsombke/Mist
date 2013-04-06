#include "LawnSimStd.h"
#include "ETCalcParametersBuilder.h"

namespace Mist { namespace FAO_ET {

using namespace ETCalcParametersBuilderInternal;


ETCalcParameters ETCalcParametersBuilder::Build() const
{
   ETCalcParameters retVal(interval_);


   return retVal;
}

}}
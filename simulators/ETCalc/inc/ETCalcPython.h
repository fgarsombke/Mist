#pragma once

#include "MistStd.h"
#include "ETCalc.h"
#include "ETCalcParameters.h"
#include "DataSetOptional.h"


#ifndef _MSC_VER
#define __declspec(x) 
#endif

using namespace Mist;

using namespace ETCalc;

extern "C" {

__declspec( dllexport ) ET_float_t CalculateET_0(
      double latitude,
      double longitude,
      unsigned long long intervalStart,
      unsigned long long intervalEnd,
      BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(TYPED_PARAMS,ET_float_t,ETDATA_PARAMS))
      )
{
   using namespace pt;
   using namespace boost::gregorian;

   Mist::ETCalc::ETCalc etCalc(latitude, longitude);

   ptime start = ptime(date(1970, 1, 1), time_duration(0, 0, 0, time_duration::ticks_per_second() * intervalStart));
   ptime end = ptime(date(1970, 1, 1), time_duration(0, 0, 0, time_duration::ticks_per_second() * intervalEnd));

   ETCalcParameters parameters(pt::time_period(start, end), BOOST_PP_SEQ_ENUM(ETDATA_PARAMS));

   return etCalc.CalculateET_0(parameters);
}
}

#undef PACK_PARAMS
#undef LIST_PARAMS
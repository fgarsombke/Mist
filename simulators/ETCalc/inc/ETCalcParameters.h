#pragma once

#include "MistStd.h"

namespace Mist { namespace ETCalc {

typedef double ET_float_t;

#define ETDATA_PARAMS (MinTemp)(MaxTemp)(AvgTemp)\
                      (MinRH)(MaxRH)\
                      (StartTemp)(EndTemp)\
                      (AvgPressure)\
                      (AvgWindSpeed)\
                      (SunlightFraction)\

#define ETDATA_VALUES ETDATA_PARAMS\
                      (ENUM_COUNT)

#define TYPED_PARAMS(r, data, elem) (data elem)
	
enum class ETCalcData_t
{
	BOOST_PP_SEQ_ENUM(ETDATA_VALUES)
};

enum class ETCalcLengthType
{
   HoursDay,
   HoursNight,
   Days,
   Months,
};

class ETCalcParameters 
	: DataSetOptional<ET_float_t, ETCalcData_t>
{
public:
	ETCalcParameters();
   ETCalcParameters(pt::time_period interval, const WeatherData& data);

   ETCalcParameters(pt::time_period interval,
                    BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(TYPED_PARAMS,ET_float_t,ETDATA_PARAMS)));

   pt::time_period Interval() const { return interval_; }

   ETCalcLengthType LengthType() const { return length_type_; }

#if _DEBUG
   #define CHECK_IS_SET(x, data, elem)  if (!IsSet(data::elem)) { throw std::logic_error("ETCalc Value " BOOST_PP_STRINGIZE_I(elem) " used but not set."); }
#else
   #define CHECK_IS_SET(x, data, elem)
#endif

   // Use this regex s/\(([\w\d_]+?)\)/inline ET_float_t $1() const { return GetValue(ETCalcData_t::$1); }\n/g
#define GET_FUNC(r, data, elem) inline ET_float_t elem() const { \
      CHECK_IS_SET(x, data, elem)   \
      return GetValue(data::elem); \
   }

	BOOST_PP_SEQ_FOR_EACH(GET_FUNC,ETCalcData_t,ETDATA_VALUES)

#undef CHECK_IS_SET
#undef GET_FUNC
#undef DATA_VALUES

	inline void SetSunlightFraction(ET_float_t value) { SetValue(ETCalcData_t::SunlightFraction, value); } 

private:
   void SetLengthType();

   pt::time_period interval_;
   ETCalcLengthType length_type_;
};

typedef const ETCalcParameters &ETParam_t;

}}

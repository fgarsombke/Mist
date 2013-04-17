#pragma once

// Standard Library Headers
#include <string>
#include <stdexcept>

#include <iostream>
#include <algorithm>
#include <array>
#include <forward_list>

#if __linux__ == 1

#include <uuid/uuid.h>

#else 
#ifdef _MSC_VER

#define __attribute__(_) 

#endif
#endif

// Boost Headers
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/preprocessor.hpp>

namespace pt = boost::posix_time;
namespace dt = boost::date_time;
namespace bJP = boost::property_tree::json_parser;
namespace bUUID = boost::uuids;

typedef boost::uuids::uuid uuid;
extern const char* EmptyUUIDString;

namespace Mist {
   typedef boost::uuids::uuid uuid;
   
   const int ZONE_OFFSET = 1;

   typedef size_t product_id_t;
}


#include "GeoLocale.h"
#include "ScheduleSource.h"
#include "WeatherData.h"
#include "WeatherDataSource.h"
#include "Feedback.h"

#if defined(_MSC_VER)
  #define JL_SIZE_T_SPECIFIER    "%Iu"
  #define JL_SSIZE_T_SPECIFIER   "%Id"
  #define JL_PTRDIFF_T_SPECIFIER "%Id"
#elif defined(__GNUC__)
  #define JL_SIZE_T_SPECIFIER    "%zu"
  #define JL_SSIZE_T_SPECIFIER   "%zd"
  #define JL_PTRDIFF_T_SPECIFIER "%zd"
#else
  // TODO figure out which to use.
  #if NUMBITS == 32
    #define JL_SIZE_T_SPECIFIER    something_unsigned
    #define JL_SSIZE_T_SPECIFIER   something_signed
    #define JL_PTRDIFF_T_SPECIFIER something_signed
  #else
    #define JL_SIZE_T_SPECIFIER    something_bigger_unsigned
    #define JL_SSIZE_T_SPECIFIER   something_bigger_signed
    #define JL_PTRDIFF_T_SPECIFIER something-bigger_signed
  #endif
#endif
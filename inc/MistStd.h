#pragma once

// Standard Library Headers
#include <string>
#include <stdexcept>

#include <iostream>
#include <algorithm>
#include <array>
#include <forward_list>
#include <regex>

#if __linux__ == 1


#else 
#ifdef _MSC_VER

#define __attribute__(_) 

#endif
#endif

// Boost Headers
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/preprocessor.hpp>
#include <boost/algorithm/string.hpp>

namespace pt = boost::posix_time;
namespace dt = boost::date_time;
namespace bJP = boost::property_tree::json_parser;

#define _DEBUG_DATA 1
#define _DEBUG_NO_PIPES 0

#ifdef _DEBUG
#define DbgAssertLogic(p, m) if(p) { throw std::logic_error((m)); }

#define DbgPrint(msg) std::cout << (msg) << std::endl;
#else
#define DbgAssertLogic(p,m) 
#define DbgPrint(msg) 
#endif

namespace Mist {  
   const int ZONE_OFFSET = 1;

   typedef size_t product_id_t;
   typedef size_t user_id_t;
}


#include "GeoLocale.h"
#include "ScheduleSource.h"
#include "WeatherDataSource.h"

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
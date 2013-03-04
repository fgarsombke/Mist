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



#endif
#endif

// My Headers
#include "MistStd.h"
#include "GeoLocale.h"

// Boost Headers
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/uuid/string_generator.hpp>

namespace pt = boost::posix_time;
namespace dt = boost::date_time;
namespace bJP = boost::property_tree::json_parser;
using boost::property_tree::ptree;
namespace bUUID = boost::uuids;

typedef boost::uuids::uuid uuid;



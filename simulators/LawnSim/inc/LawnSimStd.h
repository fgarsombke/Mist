#pragma once

// Standard Library Headers
#include <string>
#include <memory>
#include <stdexcept>

#include <iostream>
#include <algorithm>
#include <array>
#include <thread>
#include <mutex>

// Boost Headers
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/program_options/errors.hpp>

namespace pt = boost::posix_time;
namespace dt = boost::date_time;
namespace bnu = boost::numeric::ublas;

#ifdef _DEBUG
//#define _DEBUG_SPRINKLER_DURATIONS
#endif

// My Headers
#include "MistStd.h"
#include "Constants.h"

// -100 is most undergrown, 0 perfect, 100 most overgrown
typedef double health_t;

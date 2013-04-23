#pragma once

// Standard Library Headers
#include <string>
#include <memory>
#include <stdexcept>

#include <math.h>
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

typedef double health_t;


// My Headers
#include "MistStd.h"
#include "Constants.h"

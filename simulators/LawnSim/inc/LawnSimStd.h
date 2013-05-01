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
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/filesystem.hpp>

namespace bnu = boost::numeric::ublas;
namespace fs = boost::filesystem;

#ifdef _DEBUG
//#define _DEBUG_SPRINKLER_DURATIONS
#endif

typedef double health_t;
typedef size_t zone_number_t;


// My Headers
#include "MistStd.h"
#include "Constants.h"

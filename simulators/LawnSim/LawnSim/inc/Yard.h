#pragma once

#include "LawnSimStd.h"
#include "YardCell.h"
#include "GeoLocale.h"

namespace bm = boost::numeric::ublas;

namespace LawnSim {

class Yard 
{
private: 
   bm::matrix<YardCell> yard_cells_;


   const GeoLocale locale_;

public:
   Yard(const GeoLocale locale);


};

}
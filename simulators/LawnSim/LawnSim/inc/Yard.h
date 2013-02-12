#pragma once

#include "LawnSimStd.h"
#include "YardCell.h"
#include "GeoLocale.h"


namespace bm = boost::numeric::ublas;

namespace Mist {
namespace LawnSim {

class Yard 
{
private: 
   friend class LawnGenerator;

   const bm::matrix<YardCell> yard_cells_;

   const GeoLocale locale_;

    Yard(const GeoLocale locale, bm::matrix<YardCell> yardCells);
public:
   const YardCell& GetCellAt(size_t row, size_t col) const;

   size_t yard_length() const { return yard_cells_.size1(); }
   size_t yard_width() const { return yard_cells_.size2(); }
};

}
}
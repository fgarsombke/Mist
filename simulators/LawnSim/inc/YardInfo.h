#pragma once

#include "LawnSimStd.h"
#include "YardCellInfo.h"
#include "Sprinkler.h"

#include "GeoLocale.h"


namespace bm = boost::numeric::ublas;
using namespace std;

namespace Mist {
namespace LawnSim {

class YardInfo
{
public:
   const bm::matrix<YardCellInfo> yard_cells() const { return yard_cells_; }


   size_t yard_length() const { return yard_cells_.size1(); }
   size_t yard_width() const { return yard_cells_.size2(); }

private: 
   friend class LawnGenerator;

   const bm::matrix<YardCellInfo> yard_cells_;

   // The sprinklers in the yard, along with their positions
   const vector<pair<Sprinkler, pair<int,int> > > sprinkler_;

   const GeoLocale locale_;

    YardInfo(const GeoLocale locale, bm::matrix<YardCellInfo> yardCells)
      : locale_(locale), yard_cells_(yardCells)
   {
   }
};




}
}
#pragma once

#include "LawnSimStd.h"


namespace Mist { namespace LawnSim {

struct LawnCoordinate {
public:
   unsigned int Row;
   unsigned int Col;

   LawnCoordinate()
   {
   }

   LawnCoordinate(unsigned int x, unsigned int y)
      : Row(x), Col(y) 
   {
   }
};

std::ostream& operator<<(std::ostream &out, LawnCoordinate lc);

}}


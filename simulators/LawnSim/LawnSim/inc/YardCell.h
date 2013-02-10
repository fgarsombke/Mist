#pragma once

#include "LawnSimStd.h"

namespace LawnSim {

class YardCell
{
public:
   YardCell(double initial_health,
            double rel_height)
            : initial_health_(initial_health),
              rel_height_(rel_height)
   {
   }

   YardCell() 
      : initial_health_(0),
        rel_height_(0)
   {
   }


   double rel_height() const { return rel_height_; }
// ------------------------------------------------------------------------------
private:
   double initial_health_;
   double rel_height_;
};

}
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


   
// ------------------------------------------------------------------------------
private:
   const double initial_health_;
   const double rel_height_;

  YardCell & operator=(YardCell other);
  YardCell(const YardCell &other);

};

}
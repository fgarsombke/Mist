#pragma once

#include "LawnSimStd.h"

namespace Mist { namespace LawnSim {

class YardCellInfo
{
public:
   YardCellInfo(double initial_health,
            double rel_height)
   : initial_health_(initial_health),
      rel_height_(rel_height)
   {
   }

   YardCellInfo() 
      : initial_health_(0),
        rel_height_(0)
   {
   }

   double initial_health() const { return initial_health_; }
   double rel_height() const { return rel_height_; }
// ------------------------------------------------------------------------------
private:
   double initial_health_;

   // Height above zero in millimeters
   double rel_height_;
};

}}
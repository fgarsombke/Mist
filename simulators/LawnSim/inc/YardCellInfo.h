#pragma once

#include "LawnSimStd.h"

namespace Mist { namespace LawnSim {

class YardCellInfo
{
public:
   YardCellInfo(double initial_health, 
                double rel_height, 
                zone_number_t zone,
                double sunlightFraction)
      : initial_health_(initial_health), 
        zone_(zone), 
        rel_height_(rel_height),
        sunlight_fraction_(sunlightFraction)
   {
   }

   YardCellInfo() 
      : initial_health_(0),
        rel_height_(0)
   {
   }

   double sunlight_fraction() const { return sunlight_fraction_; }
   double initial_health() const { return initial_health_; }
   double rel_height() const { return rel_height_; }
   zone_number_t zone() const { return zone_; }
   
// ------------------------------------------------------------------------------
private:
   double initial_health_;
   zone_number_t zone_;   


   // Height above zero in millimeters
   double rel_height_;
   double sunlight_fraction_;
};

}}
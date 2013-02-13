#pragma once

#include "LawnSimStd.h"


namespace Mist { namespace LawnSim {



class DriftEntry {
public:
   DriftEntry() {}

   DriftEntry(size_t yardLocation) 
      : yard_location_(yardLocation)
   {
   }

   const size_t yard_location() const { return yard_location_; }


   double& Right() { return drift_constants_[0]; }
   double& TopRight() { return drift_constants_[1]; }
   double& Top() { return drift_constants_[2]; }
   double& TopLeft() { return drift_constants_[3]; }
   double& Left() { return drift_constants_[4]; }
   double& BottomLeft() { return drift_constants_[5]; }
   double& Bottom() { return drift_constants_[6]; }
   double& BottomRight() { return drift_constants_[7]; }

private:
   size_t yard_location_;

   // Drift constants starting at the right going counter clockwise
   double drift_constants_[8];
};



















}}


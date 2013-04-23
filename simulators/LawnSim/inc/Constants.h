#pragma once
#include "LawnSimStd.h"

namespace Mist { namespace LawnSim {

namespace Constants{

   // Default Values
   // 
   const unsigned int DefaultSimTickPeriod = 600000;
   const unsigned int DefaultSimSpeedMultiplier = 600;

   // Error Messages
   //
   extern const char * SimMultDoesNotDivide;
   
   // -100 is most undergrown, 0 perfect, 100 most overgrown
   const health_t MaxHealth = 100;
   const health_t OverGrownThreshhold = 33;
   const health_t UnderGrownThreshhold = -33;
   
}

}}
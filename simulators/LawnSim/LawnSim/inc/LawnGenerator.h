#pragma once

#include "LawnSimStd.h"

#include "Yard.h"

namespace LawnSim {

class LawnGenerator {
public:
   LawnGenerator() {
      
   }

   // Generates a square yard
   std::unique_ptr<Yard> Generate(GeoLocale locale, unsigned int rows, unsigned int cols) const;

private:

};

}
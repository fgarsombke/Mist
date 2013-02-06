/*
   Contains the information and defines behavior in a single plot of land.
*/

#include "LawnSimStd.h"

#include "Yard.h"

namespace LawnSim {

// Generates a new, random yard
Yard::Yard(GeoLocale locale) 
   : locale_(locale)
{}

}
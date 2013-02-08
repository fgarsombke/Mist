/*
   Contains the information and defines behavior in a single plot of land.
*/

#include "LawnSimStd.h"

#include "Yard.h"

namespace LawnSim {

// Generates a new, random yard
Yard::Yard(GeoLocale locale, bm::matrix<YardCell> yardCells) 
   : locale_(locale), yard_cells_(yardCells)
{

}

const YardCell& Yard::GetCellAt(size_t row, size_t col) const {
   return yard_cells_(row, col);
}


}
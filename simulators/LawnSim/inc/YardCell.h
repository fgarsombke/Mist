#pragma once

#include "LawnSimStd.h"

#include "YardCellInfo.h"


namespace Mist {
namespace LawnSim {

class YardCell {
public:
   YardCell(YardCellInfo info)
      : cell_info_(info) 
   {
   }

   YardCell()
      : cell_info_(YardCellInfo()) 
   {
   }


private:
   YardCellInfo cell_info_;

   double current_health_;



   // Water soaks down into the depths
   double water_saturation_;

};

















}
}
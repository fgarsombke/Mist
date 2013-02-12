#pragma once

#include "LawnSimStd.h"

#include "YardInfo.h"
#include "YardCell.h"


namespace Mist {
namespace LawnSim {


class Yard {
public:
   Yard(const YardInfo& yardInfo);

private:
   const bm::matrix<YardCell> cells_;









   
   bm::matrix<YardCell> InitCells(const YardInfo& yardInfo);




};









}
}


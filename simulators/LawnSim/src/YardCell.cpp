#include "LawnSimStd.h"

#include "YardCell.h"
#include "InvalidOperationException.h"


namespace Mist { namespace LawnSim {

YardCell::YardCell(YardCellInfo info, YardCellType_t type) 
   : cell_info_(info), cell_type_(type)
{
}

YardCell::YardCell(YardCellInfo info, YardCellType_t type, NeighborHeightDiffs_t &heightDiffs)
   : cell_info_(info), cell_type_(type), drift_entry_(DriftEntry(heightDiffs))
{
}

YardCell::YardCell()
{
}

void YardCell::ResetState() 
{
   current_health_ = cell_info_.initial_health();
   absorbed_water_ = 0;
}

// Increase height by delta
void YardCell::ChangeHeight(double delta) {
   if (cell_type_ == YardCellType_t::Void) {
      cell_info_ = YardCellInfo(cell_info_.initial_health(), cell_info_.rel_height() + delta);
   }
}

YardCell YardCell::CreateVoid(double relHeight)
{
   return YardCell(YardCellInfo(FP_NAN, relHeight), YardCellType_t::Void);
}


YardCell YardCell::CreateGrass(YardCellInfo info, NeighborHeightDiffs_t &heightDiffs)
{
   return YardCell(info, YardCellType_t::Grass, heightDiffs);
}

YardCell YardCell::CreateIsolated(YardCellInfo info)
{
   return YardCell(info, YardCellType_t::Isolated);
}

void YardCell::UnIsolate(NeighborHeightDiffs_t heightDiffs) {   
   if (cell_type_ == YardCellType_t::Isolated) {
      drift_entry_ = DriftEntry(heightDiffs);

      cell_type_ = YardCellType_t::Grass;
   } else {
      throw Exceptions::InvalidOperationException("Can only unisolate a YardCell of type Isolated.");
   }
}


// Debug Methods
#ifdef _DEBUG

const DriftEntry YardCell::drift_entry() const { 
   if (cell_type_ == YardCellType_t::Grass) {
      return drift_entry_; 
   } else {
      throw Exceptions::InvalidOperationException("Can only get drift_entries for YardCells of type Grass.");
   }
}


#else
const DriftEntry YardCell::drift_entry() const { 
   return drift_entry_; 
}


#endif

}}
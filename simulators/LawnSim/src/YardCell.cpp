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
   absorbed_water_ = 0;
}

// Increase height by delta
void YardCell::ChangeHeight(double delta) {
   if (cell_type_ == YardCellType_t::Void) {
      cell_info_ = YardCellInfo(cell_info_.initial_health(), 
                                cell_info_.rel_height() + delta, 
                                cell_info_.zone()
      );
   } else {
      throw std::logic_error("Cannot change the height of a non void cell");
   }
}

double sign(double num)
{
   return num >=0?1:-1;
}

health_t YardCell::ComputeHealthMetric(health_t currentHealth, 
                                       water_mm_t waterAvailable,
                                       double periodLengthDays,
                                       double growthFactor)
{
   // We compute the health metric by following a logistic curve.
   // http://en.wikipedia.org/wiki/Logistic_function
   // See also matlab/HealthMetric.m

   using namespace Constants;

   // TODO: Figure out how to do this analytically
   const double K = MaxHealth;
   const double A = -MaxHealth;
   const double B = 1/8.0;
   const double C = 0;

   const double T = 1.0;

   double yp = sign(currentHealth)*B*(K-A)*exp(-B*(currentHealth-C))/pow(1+exp(-B*(currentHealth-C)),2);

   double dHealth = yp*periodLengthDays*waterAvailable*T;

   double newHealth = currentHealth + growthFactor*dHealth;

   if (newHealth < -MaxHealth) {
      return -MaxHealth;
   } else if (newHealth > MaxHealth) {
      return MaxHealth;
   } else {
      return newHealth;
   }
}

bool YardCell::HasHealth() const 
{
   return (cell_type_ == YardCellType_t::Grass) | 
          (cell_type_ == YardCellType_t::Dead);
}

double YardCell::ET_K() const
{
   return 1.0;
}

///////////////////////////////////////////////////////////////////////////
// Static Factory Methods
YardCell YardCell::CreateVoid(double relHeight)
{
   return YardCell(YardCellInfo(FP_NAN, relHeight, 0), YardCellType_t::Void);
}

YardCell YardCell::CreateIsolated(YardCellInfo info)
{
   return YardCell(info, YardCellType_t::Isolated);
}
///////////////////////////////////////////////////////////////////////////


void YardCell::UnIsolate(NeighborHeightDiffs_t heightDiffs) {   
   if (cell_type_ == YardCellType_t::Isolated) {
      drift_entry_ = DriftEntry(heightDiffs);
      cell_type_ = YardCellType_t::Grass;
   } else {
      throw Exceptions::InvalidOperationException("Can only unisolate a YardCell of type Isolated.");
   }
}


///////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////

}}
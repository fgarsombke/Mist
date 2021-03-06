#include "LawnSimStd.h"
#include "DriftEntry.h"

namespace Mist { namespace LawnSim {

// These constants use units of meters

// Max amount of water a patch of soil can lose
// Must be less than 1.0
static const double MaxDriftAmount = 0.995;

// Eccentricity of Sigmoid
static const double M = 1.0;

// Difference in height that will cause flow
static const double K = .015;

// Slope of Sigmoid
static const double L = .08;

// Asmptotic correction constant
// Divide by 8 for 8 subcells
static const double C = -(MaxDriftAmount/(1.0 + (K*M/(K*M+L))));

// Function Offset
static const double Offset = M*K/(M*K + L);

// Relative height is stored in mm

static const double HeightUConv = 1000.0;

static const double invSqrt2 = 1.0/sqrt(2.0);

// Compute Drift paramaters
DriftEntry::DriftEntry(NeighborHeightDiffs_t &heightDiffs) 
{
   // Assigns drift constants based on the relative heights of adjacent cells
   // In the equation below, d~slope of the yard between adjacent cells.
   double totalFlowVolume = 1;

   this->Center() = 1.0;

   double cellFlowConstant;
   double total = 0;

   // Scale corners by distance
   heightDiffs.TopLeft() *= invSqrt2;
   heightDiffs.TopRight() *= invSqrt2;
   heightDiffs.BottomLeft() *= invSqrt2;
   heightDiffs.BottomRight() *= invSqrt2;

   // Compute the sigmoid for each distance
   for (int i = 0; i < 9; ++i) {
      if (heightDiffs.data().data()[i] < 0) {
         double d = HeightUConv*heightDiffs.data().data()[i]; 

         // Compute the sigmoid
         cellFlowConstant = C*(M*(d-K)/(M*abs(d-K)+L) + Offset)/8.0;
      } else {
         cellFlowConstant = 0;
      }

      this->data_mutable().data()[i] = cellFlowConstant;
      total -= cellFlowConstant;
   }

   this->Center() = total;
}

#ifdef _DEBUG

std::ostream& operator<<(std::ostream &out, DriftEntry lc) {
   using namespace std;
   out << endl << "DriftEntry: " << endl << "[" << endl;

   //out.precision(8);
   //out.width(10);

   for (int i = 0; i < 3; ++i) {
      out << "\t";

      out << lc.data()(i, 0) << ", ";
      out << lc.data()(i, 1) << ", ";
      out << lc.data()(i, 2);

      out << endl;
   }

   out << endl << "]" << endl;

   out << resetiosflags(ios::floatfield);

   return out;
}

#endif



}}
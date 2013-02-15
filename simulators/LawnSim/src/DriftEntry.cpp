#pragma once

#include "LawnSimStd.h"
#include "DriftEntry.h"

namespace Mist { namespace LawnSim {

// These constants use units of meters

// Max amount of water a patch of soil can lose
// Must be less than 1.0
static const double MaxDriftAmount = 0.98;

// Eccentricity of Sigmoid
static const double M = 1.0;

// Difference in height that will cause flow
static const double K = .15;

// Slope of Sigmoid
static const double L = .02;

// Asmptotic correction constant
// Divide by 8 for 8 adjacent zones
static const double C = (MaxDriftAmount/(1.0 + (K*M/(K*M+L))))/8.0;

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

   auto diffs = heightDiffs.begin();
   auto driftParams = this->begin();
   SetSelf(1.0);
   driftParams[8] = 1.0;

   double cellFlowConstant;

   for (int i = 1; i < 8; i+=2) {
      // Scale corners by distance
      diffs[i] *= invSqrt2;
   }

   for (int i = 0; i < 8; ++i) {
      if (diffs[i] < 0) {
         double d = HeightUConv*diffs[i]; 

         // Compute the sigmoid
         cellFlowConstant = C*(M*(d-K)/(M*abs(d-K)+L) + Offset);
      } else {
         cellFlowConstant = 0;
      }

      driftParams[i] = cellFlowConstant;
      driftParams[8] += cellFlowConstant;
   }
}









}}
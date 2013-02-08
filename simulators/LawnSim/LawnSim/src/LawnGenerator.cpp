#include "LawnSimStd.h"

#include <boost/math/distributions/normal.hpp>
#include <boost/nondet_random.hpp>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>

#include "LawnGenerator.h"
#include <math.h>

using std::unique_ptr;

namespace LawnSim {

unique_ptr<Yard> LawnGenerator::Generate(GeoLocale locale, unsigned int rows, unsigned int cols) const {
   if (rows == 0 || cols == 0) {
      throw std::invalid_argument("rows and cols must both be nonzero.");
   }

   bm::matrix<YardCell> cells(rows, cols);

   bm::matrix<double> heights(rows, cols);

   typedef boost::mt19937 RNGType;

   RNGType rng((unsigned int)time(0));

   int featureSize = rows/4;

   double driftparameter = 0;

   double defaultStdev = 0.01;

   boost::normal_distribution<double> dist(driftparameter, defaultStdev);

   boost::variate_generator<RNGType, boost::normal_distribution<double> > variates(rng, dist);

   // Start at a corner and do a random walk to generate a height map
   //
   // Corner (0,0) is always the zero point for relative elevation
   heights(0,0) = 0;

   // Do left and top edges next
   for (unsigned int i = 1; i < cols; ++i) {
      heights(0,i) = heights(0, i-1) + variates();
   }

   for (unsigned int i = 1; i < rows; ++i) {
      heights(i,0) = heights(i-1, 0) + variates();
   }

   // Next do the top left half
   for (unsigned int i = 1; i < rows; ++i) {
      unsigned int bound = std::min(i, cols);
      for (unsigned int j = 0; j < bound; ++j) {
         heights(i-j,j+1) = (heights(i-j,j) + heights(i-j-1,j+1))/2 + variates();
      }
   }

   // Finish with the bottom half
   //for(unsigned int i = 2; i < cols; ++i) {
   //   for (unsigned int j = i; j < rows; ++ j) {
   //      heights(j,i)
   //   }
   //}


   return unique_ptr<Yard>(new Yard(locale, cells));
}


}
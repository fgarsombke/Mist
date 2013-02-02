#include <cstdio>
#include <cstdlib>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_cdf.h>

int main(int argc, char * argv[]) {
   
   //gsl_rng * gen = gsl_rng_alloc(gsl_rng_default); 

   
   FILE *pF = fopen("exp.csv", "w");


   double x = 0;
   while(x < 6) {
      fprintf(pF, "%lf,\n", gsl_cdf_exponential_P(x, 1));
      x += .001;
   }

   _fcloseall();   
   return 0;
}
#include "ETCalcPython.h"

using namespace Mist::ETCalc;

BOOST_PYTHON_MODULE(ETCalcCPP)
{
    class_<ETCalcParameters>("ETCalcParameters");
}
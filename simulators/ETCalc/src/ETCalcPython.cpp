#include "ETCalcPython.h"

using namespace Mist::ETCalc;

BOOST_PYTHON_MODULE(ETCalcCPP)
{
    class_<ETCalc>("ETCalc", init<double, double>())
		 .def("CalculateET_o", &ETCalc::CalculateET_o);
}
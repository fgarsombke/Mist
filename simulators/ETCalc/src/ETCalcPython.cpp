#include "ETCalcPython.h"

using namespace Mist::ETCalc;

BOOST_PYTHON_MODULE(libETCalc)
{
    class_<ETCalc>("ETCalc", init<double, double>())
		 .def("CalculateET_o", &ETCalc::CalculateET_o);

	 //class_<DataSetOptional>("DataSetOptional");
}

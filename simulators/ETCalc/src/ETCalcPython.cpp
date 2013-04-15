#include "ETCalcPython.h"

using namespace Mist::ETCalc;

BOOST_PYTHON_MODULE(libETCalc)
{
    class_<ETCalc>("ETCalc", init<double, double>())
		 .def("CalculateET_o", &ETCalc::CalculateET_o);
<<<<<<< HEAD

	 //class_<DataSetOptional>("DataSetOptional");
}
=======
}
>>>>>>> 4a0cccfd0ebb3d371346459aea42eeceefd011b4

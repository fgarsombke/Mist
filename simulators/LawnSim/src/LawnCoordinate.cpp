#include "LawnSimStd.h"
#include "LawnCoordinate.h"

namespace Mist { namespace LawnSim {

#ifdef _DEBUG

std::ostream& operator<<(std::ostream &out, LawnCoordinate lc) {
      out << "(" << lc.Col << ", " << lc.Row << ")";
      return out;
}

#endif

}}
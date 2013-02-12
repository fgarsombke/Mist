#pragma once
#include <boost/optional.hpp>
#include "GeoLocale.h"


namespace Mist {
namespace Controllers {

class ControllerConfig {
public:
   // Configuration that is available to the controller in real life 
   boost::optional<GeoLocale> locale_;


};


}
}
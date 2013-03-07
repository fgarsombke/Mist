#pragma once
#include "ControllersStd.h"

#include "MistStd.h"

namespace Mist { namespace Controllers {

class ControllerConfig {
public:
   // Configuration that is available to the controller in real life 
   boost::optional<GeoLocale> locale_;

   pt::time_duration update_period_;

   std::string data_source_;

   product_id_t id;
};


}}
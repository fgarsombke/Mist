#pragma once
#include "ControllersStd.h"

#include "MistStd.h"

namespace Mist { namespace Controllers {

class ControllerConfig {
public:
   // Configuration that is available to the controller in real life 
   GeoLocale Locale;

   pt::time_duration UpdatePeriod;
   sPtrScheduleSource ScheduleSource;
   product_id_t Id;
};

}}
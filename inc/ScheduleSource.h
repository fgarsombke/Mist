#pragma once
#include "MistStd.h"

#include "MistSchedule.h"

namespace Mist {

class ScheduleSource;
typedef std::unique_ptr<ScheduleSource> uPtrScheduleSource;
typedef std::shared_ptr<ScheduleSource> sPtrScheduleSource;

class ScheduleSource {
public:
   virtual product_id_t AddDevice(user_id_t userID, 
                          GeoLocale locale,
                          pt::ptime initTime,
                          size_t numZones,
                          unsigned int timeout = -1) const = 0;

   virtual MistSchedule GetSchedule(product_id_t id, unsigned int timeout = -1) const = 0;
   virtual ~ScheduleSource() { }
};

}
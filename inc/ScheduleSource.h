#pragma once
#include "MistStd.h"

#include "MistSchedule.h"

namespace Mist {

class ScheduleSource;
typedef std::unique_ptr<ScheduleSource> uPtrScheduleSource;
typedef std::shared_ptr<ScheduleSource> sPtrScheduleSource;

class ScheduleSource {
public:
   virtual MistSchedule GetSchedule(product_id_t id, unsigned int timeout = -1) const = 0;
   virtual ~ScheduleSource() { }
};


}
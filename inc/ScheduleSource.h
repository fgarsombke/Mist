#pragma once
#include "MistStd.h"

#include "HTMLSource.h"

namespace Mist {

class ScheduleSource;
typedef std::unique_ptr<ScheduleSource> uPtrScheduleSource;

class ScheduleSource : HTMLSource {
public:
   typedef std::stringstream schedule_ret_t;

   ScheduleSource(const std::string &host,
              product_id_t id)
      : HTMLSource(host, "8080"), id_(id)
   {
   }

   void GetSchedule(schedule_ret_t &out, unsigned int timeout = -1) const;
private:
   // Reference class only
   ScheduleSource &operator=(const HTMLSource &other);
   ScheduleSource(const HTMLSource &other);

   const product_id_t id_;
};


}
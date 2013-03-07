#include "MistStd.h"

#include "ScheduleSource.h"

namespace Mist {
   static const char* apiStr = "/api/schedule?deviceID=";

   void ScheduleSource::GetSchedule(schedule_ret_t &out, unsigned int timeout) const
   {
      std::ostringstream apiFormatted;
      apiFormatted << apiStr << id_;

      std::vector<std::string> headers;

      int result = GetHtml(apiFormatted.str(), out, headers, timeout);
      if (result != 200) {
         // TODO: Throw exception
         std::ostringstream ss;
         ss << "Error Getting HTML: " << result;
         throw std::logic_error(ss.str());
      }
   }
}
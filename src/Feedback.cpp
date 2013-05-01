#include "MistStd.h"

#include "Feedback.h"
#include "PAL.h"

namespace Mist {

   namespace FeedbackInternal {
      const char* DEVICE_ID_LABEL = "deviceID";
      const char* INTERVAL_END_LABEL = "endTime";
      const char* FEEDBACK_LABEL = "zones";
      const char* FEEDBACK_ITEM_LABEL = "fbItems";
      const char* ZONE_NUM_LABEL = "zoneNumber";
      const char* TIME_LABEL = "time";
      const char* VALUE_LABEL = "value";

      // Fix quotes around numbers and bool
      std::regex exp("\"(null|true|false|[0-9]+(\\.[0-9]+)?)\"");
   }

   const std::string Feedback::PackFeedbackJson(const ZoneFeedback_t &feedbackByZone, 
                                                product_id_t deviceID,
                                                pt::ptime intervalEndTime)
   {
      using namespace FeedbackInternal;
      using boost::property_tree::ptree;

      bool any = false;
      ptree mTree;
      ptree fbTree;
      std::stringstream ss;

      mTree.put<product_id_t>(DEVICE_ID_LABEL, deviceID);

      intervalEndTime.time_of_day().total_seconds();
      mTree.put<time_t>(INTERVAL_END_LABEL, GetEpochTime(intervalEndTime));

      for (size_t z = 0; z < feedbackByZone.size(); ++z) {
         FeedbackList_t zoneFeedback = feedbackByZone[z];
         
         if (!zoneFeedback.empty()) {
            ptree currZoneTree;
            any = true;

            currZoneTree.put(ZONE_NUM_LABEL, z + ZONE_OFFSET);
            
            ptree feedbackChildListTree;
            for(const FeedbackEntry& f: zoneFeedback) {
               ptree feedbackEntryTree;
               feedbackEntryTree.put<time_t>(TIME_LABEL, GetEpochTime(f.Time));
               feedbackEntryTree.put(VALUE_LABEL, static_cast<int>(f.Value));

               feedbackChildListTree.push_back(std::make_pair("", feedbackEntryTree));
            }
            currZoneTree.push_back(std::make_pair(FEEDBACK_ITEM_LABEL, feedbackChildListTree));

            // ptree does array as anonymous children
            fbTree.push_back(std::make_pair("", currZoneTree));
         }
      }

      if (any) {
         mTree.add_child(FEEDBACK_LABEL, fbTree);
      }

      boost::property_tree::json_parser::write_json(ss, mTree);
      return std::regex_replace(ss.str(), exp, std::string("$1"));
   }

}
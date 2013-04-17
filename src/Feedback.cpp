#include "MistStd.h"

#include "Feedback.h"


namespace Mist {

   namespace FeedbackInternal {
      const char* ZONE_NUM_LABEL = "zoneNumber";
      const char* FEEDBACK_LABEL = "feedback";
      const char* TIME_LABEL = "time";
      const char* VALUE_LABEL = "value";
   }

   const std::string Feedback::PackFeedbackJson(const ZoneFeedback_t &feedbackByZone)
   {
      using namespace FeedbackInternal;
      using boost::property_tree::ptree;

      // Remove the trivial case from consideration
      if (feedbackByZone.size() == 0) {
         return "";
      }

      ptree fbTree;
      std::stringstream ss;

      for (size_t z = 0; z < feedbackByZone.size(); ++z) {
         FeedbackList_t zoneFeedback = feedbackByZone[z];
         
         if (!zoneFeedback.empty()) {
            ptree currZoneTree;

            currZoneTree.add(ZONE_NUM_LABEL, z + ZONE_OFFSET);
            
            ptree feedbackChildListTree;
            for(const FeedbackEntry& f: zoneFeedback) {
               ptree feedbackEntryTree;
               feedbackEntryTree.add(TIME_LABEL, f.Time);
               feedbackEntryTree.add(VALUE_LABEL, static_cast<int>(f.Value));

               feedbackChildListTree.push_back(std::make_pair("", feedbackEntryTree));
            }
            currZoneTree.push_back(std::make_pair("", feedbackChildListTree));


            // ptree does array as anonymous children
            fbTree.push_back(std::make_pair("", currZoneTree));
         }
      }

      boost::property_tree::json_parser::write_json(ss, fbTree);
      return ss.str();
   }

}
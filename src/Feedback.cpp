#include "MistStd.h"

#include "Feedback.h"


namespace Mist {

   namespace FeedbackInternal {
      const char* ZONE_NUM_LABEL = "zoneNumber";
      const char* FEEDBACK_LABEL = "feedback";
      const char* TIME_LABEL = "time";
      const char* VALUE_LABEL = "value";
   }

   const std::string Feedback::PackFeedbackJson(const std::vector<FeedbackList_t> feedbackByZone)
   {
      using namespace FeedbackInternal;
      using boost::property_tree::ptree;

      // Remove the trivial case from consideration
      if (feedbackByZone.size() == 0) {
         return "";
      }

      ptree fbTree;
      std::stringstream ss;

      for (size_t z = ZONE_OFFSET; z < feedbackByZone.size() + ZONE_OFFSET; ++z) {
         FeedbackList_t zoneFeedback = feedbackByZone[z];
         
         if (!zoneFeedback.empty()) {
            ptree currZoneTree;

            currZoneTree.put(ZONE_NUM_LABEL, z);
            
            ptree feedbackChildListTree;
            for(const FeedbackEntry& f: zoneFeedback) {
               ptree feedbackEntryTree;
               feedbackEntryTree.add(TIME_LABEL, f.Time);
               feedbackEntryTree.add(VALUE_LABEL, static_cast<int>(f.Value));

               feedbackChildListTree.push_back(std::make_pair("", feedbackEntryTree));
            }
            currZoneTree.add_child("", feedbackChildListTree);


            // ptree does array as anonymous children
            fbTree.add_child("", currZoneTree);
         }
      }

      boost::property_tree::json_parser::write_json(ss, fbTree);
      return ss.str();
   }

}
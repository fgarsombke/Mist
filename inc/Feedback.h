#pragma once

#include "MistStd.h"

namespace Mist {

enum class FeebackValue
{
   Overgrown = -1,
   Okay = 0,
   Undergrown = 1,
};

class FeedbackEntry
{
public:
   FeebackValue Value;
   pt::ptime Time;
};

typedef std::forward_list<FeedbackEntry> FeedbackList_t;
typedef std::vector<FeedbackList_t> ZoneFeedback_t;

class Feedback
{
public:
   static const std::string PackFeedbackJson(const ZoneFeedback_t feedbackByZone);
};

}
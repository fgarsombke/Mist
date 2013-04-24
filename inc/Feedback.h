#pragma once

#include "MistStd.h"

namespace Mist {

enum class FeedbackValue
{
   Undergrown = -1,
   Okay = 0,
   Overgrown = 1,
};

class FeedbackEntry
{
public:
   FeedbackValue Value;
   pt::ptime Time;
};

typedef std::vector<FeedbackEntry> FeedbackList_t;
typedef std::vector<FeedbackList_t> ZoneFeedback_t;

class Feedback
{
public:
   static const std::string PackFeedbackJson(const ZoneFeedback_t &feedbackByZone);
};

}
#pragma once
#include "MistStd.h"

#include "Feedback.h"

namespace Mist {
	class FeedbackSink;
	typedef std::unique_ptr<FeedbackSink> uPtrFeedbackSink;
	typedef std::shared_ptr<FeedbackSink> sPtrFeedbackSink;

	class FeedbackSink {
	public:
		virtual int SubmitFeedback(product_id_t id, 
                                 pt::ptime intervalEnd, 
                                 const std::vector<FeedbackList_t> feedback, 
                                 unsigned int timeout = -1) const = 0;

		virtual ~FeedbackSink() { };
	};
}
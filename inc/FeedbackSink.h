#pragma once
#include "MistStd.h"

namespace Mist {
	class FeedbackSink;
	typedef std::unique_ptr<FeedbackSink> uPtrWeatherDataSource;
	typedef std::shared_ptr<FeedbackSink> sPtrWeatherDataSource;

	class FeedbackSink {
	public:
		virtual WeatherData SubmitFeedback(const std::forward_list<int> feedback, pt::time_period period, unsigned int timeout = -1) = 0;
		virtual ~FeedbackSink() { };
	};
}
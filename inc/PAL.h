#pragma once
#include "MistStd.h"

extern inline unsigned int GetSystemTimeMS();
extern inline int SleepForMS(int time_ms);

extern inline time_t GetEpochTime(pt::ptime time);

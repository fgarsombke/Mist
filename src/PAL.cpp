#include "MistStd.h"

#include "PAL.h"


#ifdef _MSC_VER 

#include <Windows.h>
//#include <mmsystem.h>

// In Milliseconds
inline unsigned int GetSystemTimeMS() 
{
   return GetTickCount(); //timeGetTime();
}

inline int SleepForMS(int time_ms) 
{ 
   return SleepEx(time_ms, false);
}

#else
#include <time.h>

// In Milliseconds
inline unsigned int GetSystemTimeMS() 
{
   struct timespec ts;
   if(clock_gettime(CLOCK_MONOTONIC,&ts) != 0) {
      throw std::logic_error("clock_gettime failed.");//error
   }
   
   return ts.tv_sec*1000 + ts.tv_nsec/1000000;
}

inline int SleepForMS(int time_ms) 
{  
   struct timespec tim; 
   int sleepSec = (time_ms)/1000; 
   int sleepRem = (time_ms)%1000; 
   tim.tv_sec = sleepSec; 
   tim.tv_nsec = sleepRem * 1000000L; 

   return nanosleep(&tim, NULL);
}

#endif

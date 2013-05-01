#include "MistStd.h"

#include "PAL.h"


#ifdef _MSC_VER 

#include <Windows.h>
//#include <mmsystem.h>

// In Milliseconds
extern unsigned int GetSystemTimeMS() 
{
   return GetTickCount(); //timeGetTime();
}

extern int SleepForMS(int time_ms) 
{ 
   return SleepEx(time_ms, false);
}

#else
#include <time.h>

// In Milliseconds
extern unsigned int GetSystemTimeMS() 
{
   struct timespec ts;
   if(clock_gettime(CLOCK_MONOTONIC,&ts) != 0) {
      throw std::logic_error("clock_gettime failed.");//error
   }
   
   return ts.tv_sec*1000 + ts.tv_nsec/1000000;
}

extern int SleepForMS(int time_ms) 
{  
   struct timespec tim; 
   int sleepSec = (time_ms)/1000; 
   int sleepRem = (time_ms)%1000; 
   tim.tv_sec = sleepSec; 
   tim.tv_nsec = sleepRem * 1000000L; 

   return nanosleep(&tim, NULL);
}

#endif

extern time_t GetEpochTime(pt::ptime time)
{
   tm timeTM = pt::to_tm(time);
   time_t tt = mktime(&timeTM);
   return tt;
}
#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include "schedule.h"

extern unsigned long ScheduleSize;
extern schedule_entry_t *Schedule;

void Schedule_Init(void);
int Schedule_Enter(schedule_entry_t *entry_ptr);
void Schedule_Refresh(void);

#endif // __FILE_SYSTEM_H__

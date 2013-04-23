#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/lm3s811.h"

#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "json.h"
#include "schedule.h"
#include "system_clock.h"
#include "wifly.h"
#include "file_system.h" // TODO - MH - better name
#include "zone.h"

#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine){}
#endif

// Stores a network response
char resp[2500];

#define CONST_SEC_PER_MIN  60
#define CONST_MIN_PER_HR 60
#define CONST_SEC_PER_HR (CONST_SEC_PER_MIN * CONST_MIN_PER_HR)
#define CONST_HR_PER_DAY    24
#define CONST_SEC_PER_DAY (CONST_SEC_PER_HR * CONST_HR_PER_DAY)

#define REF_TIME_DAY  21
#define REF_TIME_HOUR 0
#define REF_TIME_MIN  0
#define REF_TIME_TIMESTAMP 1366524000

// only workds for April 2013
#define BEGIN_TIME_DAY  22
#define BEGIN_TIME_HR 19
#define BEGIN_TIME_MIN  15

#define RUN_HR 1
#define RUN_MIN 0
#define RUN_INTERVALS_PER_MIN 2
#define RUN_SEC_PER_INTERVAL (CONST_SEC_PER_MIN / RUN_INTERVALS_PER_MIN)
#define RUN_INTERVALS_TOTAL (((RUN_HR * CONST_MIN_PER_HR) + RUN_MIN) * RUN_INTERVALS_PER_MIN)

#define BEGIN_TIME_TIMESTAMP (REF_TIME_TIMESTAMP + \
                  ((BEGIN_TIME_DAY - REF_TIME_DAY) * CONST_SEC_PER_DAY) +\
                  (BEGIN_TIME_HR * CONST_SEC_PER_HR) +\
                   (BEGIN_TIME_MIN * CONST_SEC_PER_MIN))

#define NUM_ZONES 8

int main(void){
    unsigned long i, current_time;
    schedule_entry_t temp;
	
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_6MHZ);

    // Enable interrupts
    //IntMasterEnable();
    
    Schedule_Init();
    
    current_time = BEGIN_TIME_TIMESTAMP;

    for(i = 0; i < RUN_INTERVALS_TOTAL; i++) {
        temp.zone = i % NUM_ZONES;
        temp.start_time = current_time;
        current_time += RUN_SEC_PER_INTERVAL;
        temp.end_time = current_time;
        temp.not_completed = 0xFFFFFFFF;
        Schedule_Enter(&temp);
    }

    Schedule_Refresh();

    while(1)
    {
    }
}


#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"

#include "zone.h"
#include "file_system.h"

#define MS 5000000 
#define MS_TO_S 1000

unsigned long static SystemClock_Sec = 0;
unsigned long static SystemClock_MSec = 0;

unsigned long SystemClock_Get(void) {
    return SystemClock_Sec;
}

void SystemClock_Set(unsigned long time) {
    SystemClock_Sec = time;
}

void SystemClock_Init(void) {
    SysTickIntDisable();
    SysTickDisable();
    SysTickPeriodSet(MS);
    SysTickEnable();
    SysTickIntEnable();
}


void SysTick_Handler(void){
    int i = 0;
	  SystemClock_MSec = (SystemClock_MSec++) % MS_TO_S;
    if(!SystemClock_MSec) SystemClock_Sec++;
	
	  while(i < ScheduleSize) {
		    if((Schedule[i].start_time < SystemClock_Sec) && (Schedule[i].end_time > SystemClock_Sec)) {
					Zone_Enable(Schedule[i].zone);
				} else {
				  Zone_Disable();
			  }
		}
	  
	  
	  
}


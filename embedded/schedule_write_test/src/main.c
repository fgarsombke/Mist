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

int main(void){
	  int i;
	  schedule_entry_t temp;
	
	  unsigned int errors = 0;
	
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_6MHZ);
    
    // Enable interrupts
    //IntMasterEnable();
    
    Schedule_Init();
	
	  // Schedule is pointed to buffer A
	  for(i = 0; i < ( 0x4000 / sizeof(schedule_entry_t)); i++) {
		   if(Schedule[i].zone != 0xFFFFFFFF) errors++;
	     if(Schedule[i].start_time != 0xFFFFFFFF) errors++;
	     if(Schedule[i].end_time != 0xFFFFFFFF) errors++;
	     if(Schedule[i].not_completed != 0xFFFFFFFF) errors++;
		}
		
		Schedule_Refresh();
		
		// Schedule is pointed to buffer B
		for(i = 0; i < ( 0x4000 / sizeof(schedule_entry_t)); i++) {
		   if(Schedule[i].zone != 0xFFFFFFFF) errors++;
	     if(Schedule[i].start_time != 0xFFFFFFFF) errors++;
	     if(Schedule[i].end_time != 0xFFFFFFFF) errors++;
	     if(Schedule[i].not_completed != 0xFFFFFFFF) errors++;
		}
	
	  temp.zone = 0x55555555;
	  temp.start_time = 0xAAAAAAAA;
	  temp.end_time = 0x55555555;
	  temp.not_completed = 0xAAAAAAAA;
	
		// Load buffer A with test vector
	  for(i = 0; i < 0x4000; i += sizeof(schedule_entry_t)) {
        Schedule_Enter(&temp);		
    }
		
		Schedule_Refresh();
		
		if(ScheduleSize != ( i / sizeof(schedule_entry_t)))
			errors++;
		
		// Check buffer A for valid schedule store
		for(i = 0; i < ( 0x4000 / sizeof(schedule_entry_t)); i++) {
		   if(Schedule[i].zone != 0x55555555) errors++;
	     if(Schedule[i].start_time != 0xAAAAAAAA) errors++;
	     if(Schedule[i].end_time != 0x55555555) errors++;
	     if(Schedule[i].not_completed != 0xAAAAAAAA) errors++;
		}
		
		// Load buffer B with test vector
	  for(i = 0; i < 0x4000; i += sizeof(schedule_entry_t)) {
        Schedule_Enter(&temp);		
    }
		
		if(ScheduleSize != ( i / sizeof(schedule_entry_t)))
			errors++;
		
		Schedule_Refresh();
		
		// Check buffer B for valid schedule store
		for(i = 0; i < ( 0x4000 / sizeof(schedule_entry_t)); i++) {
		   if(Schedule[i].zone != 0x55555555) errors++;
	     if(Schedule[i].start_time != 0xAAAAAAAA) errors++;
	     if(Schedule[i].end_time != 0x55555555) errors++;
	     if(Schedule[i].not_completed != 0xAAAAAAAA) errors++;
		}
				
	  while(1)
		{
    }
}


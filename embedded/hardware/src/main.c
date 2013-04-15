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
	
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_6MHZ);
    
    // Enable interrupts
    IntMasterEnable();
    
    // Enable Systick
    SystemClock_Init();

    // Initializes the WiFly module and UART communication
    WiFly_Init();
	
	  /* TODO - MH - Uncommend after parsing bug is fixed
	  // Initialize flash to store schedule
	  Schedule_Init();
	
		// Initialize zone interface
	  Zone_Init();
    */
	
    // Gets POSIX time from NIST 
    SystemClock_Set(WiFly_Time());
    
    for(;;) {
        WiFly_Open(resp);
        scheduleExtract(resp);
        scheduleParse(resp);
			  /* TODO - MH - Uncomment after parsing bug is fixed
			  for(i = 0; i < schedule_idx; i++) {
				    Schedule_Enter(&schedule[i]);
				}
				// TODO - MH - Disable_Interrupts();
			  SystemClock_Set(WiFly_Time()); // resync clock
				Schedule_Refresh();
				// TODO - MH - Enable_Interrupts();
			  */
    }
}


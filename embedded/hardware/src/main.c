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

#include "schedule.h"
#include "system_clock.h"
#include "wifly.h"

#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine){}
#endif
    
// Stores a network response
//char resp[2500];

int main(void){
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_6MHZ);
    
    // Enable interrupts
    IntMasterEnable();
    
    // Enable Systick
    SystemClock_Init();

    // Initializes the WiFly module and UART communication
    WiFly_Init();
    
    // Gets POSIX time from NIST 
    SystemClock_Set(WiFly_Time());
    
    for(;;) {
        WiFly_Open();
        //scheduleExtract(resp);
        scheduleParse();
    }
}


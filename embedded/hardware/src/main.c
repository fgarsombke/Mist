#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/lm3s811.h"

#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
//#include "drivers/rit128x96x4.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "json.h"
#include "schedule.h"
#include "SystemClock.h"
#include "wifly.h"

#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine){}
#endif
    
char resp[2100];

int main(void){
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_6MHZ);
    
    // Enable interrupts
    IntMasterEnable();
    
    // Enable Systick
    SystemClock_Init();
    //RIT128x96x4Init(1000000);
    //RIT128x96x4Clear();
    WiFly_Init();
    SystemClock_Set(WiFly_Time());
    for(;;) {
        WiFly_Open(resp);
        scheduleExtract(resp);
        JSON_Parse(resp);
    }
}


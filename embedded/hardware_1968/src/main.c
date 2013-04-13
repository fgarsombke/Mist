#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/lm3s1968.h"

#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "schedule.h"
#include "wifly.h"

#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine){}
#endif

int main(void){
    char resp[4096];
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
    RIT128x96x4Init(1000000);
    RIT128x96x4Clear();
    WiFly_Init();
    while(1) {
        RIT128x96x4Clear();
        WiFly_Time();
        WiFly_Open(resp);
        scheduleExtract(resp);
        scheduleParse(resp);
        SysCtlDelay(SysCtlClockGet()*3);
    }
}


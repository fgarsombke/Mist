#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/lm3s1968.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
#include "wifly.h"
#include "rgb_led.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine){}
#endif

int main(void){
    volatile unsigned long time;
    char resp[2048];
    RIT128x96x4Init(1000000);
    RIT128x96x4Clear();
    WiFly_Init();
    time = WiFly_Time();
    while(1) {
        int x = 0;
        WiFly_Open(resp, 2048);
        x++;
    }
}


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
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine){}
#endif

int main(void){
    volatile unsigned long time;
    RIT128x96x4Init(1000000);
    RIT128x96x4Clear();
    WiFly_Init();
    while(1) {
        WiFly_Open();
    }
}


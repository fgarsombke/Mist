#include<>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"

unsigned long SystemClock_Sec = 0;
unsigned long SystemClock_MSec = 0;

unsigned long SystemClock_Get(void)
{
    return SystemClock_Sec;
}

void SystemClock_Set(unsigned long time) {
    SystemClock_Sec = time;
}

void SystemClock_Init(void)
{
    SysTickPeriodSet(5000000);
    SysTickEnable();
}


void SysTick_Handler(void)
{
    if((SystemClock_MSec++ % 1000) == 0) {
        SystemClock_Sec++;
        SystemClock_MSec = 0;
    }
}


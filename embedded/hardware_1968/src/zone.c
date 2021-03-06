#include "zone.h"
#include "inc/lm3s811.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"

typedef enum zone_e {
    ZONE_NONE,
    ZONE_ONE,
    ZONE_TWO,
    ZONE_THREE,
    ZONE_FOUR,
    ZONE_FIVE,
    ZONE_SIX,
    ZONE_SEVEN,
    ZONE_EIGHT,
} zone_t;

static int Current_Zone = ZONE_NONE;

void Zone_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinTypeSSI(GPIO_PORTA_BASE,
            GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5);

    GPIOPadConfigSet(GPIO_PORTA_BASE, 
            GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5,
            GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    SSIDisable(SSI0_BASE);

    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
            SSI_MODE_MASTER, 2000000, 8);

    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_3,
            GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    SSIEnable(SSI0_BASE);
  
    while(SSIBusy(SSI0_BASE)) {}

    SSIDataPut(SSI0_BASE, ZONE_NONE);
}

void Zone_Enable(int zone)
{
    int val;

    while(SSIBusy(SSI0_BASE)) {}

    val = 0x01 << (zone - 1);
    SSIDataPut(SSI0_BASE, val);
}

void Zone_Disable(void)
{
    while(SSIBusy(SSI0_BASE)) {}

    SSIDataPut(SSI0_BASE, ZONE_NONE);
}

int Zone_Current(void)
{
    return Current_Zone;
}

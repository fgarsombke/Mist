
#include "inc/lm3s811.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"

#include "zone.h"

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

static unsigned long Current_Zone = 0;

// Initializes zone control using SSI0
void Zone_Init(void) {
    // Gating configuration
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Alternate function enable
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    SSIDisable(SSI0_BASE);

    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 2000000, 8);

    // Alternate function enable
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    SSIEnable(SSI0_BASE);
  
    while(SSIBusy(SSI0_BASE)); // Blocking Wait

    SSIDataPut(SSI0_BASE, ZONE_NONE);
}

// Enables a zone
void Zone_Enable(unsigned long zone) {
	  if(zone != Current_Zone) {
			Zone_Disable();
			Current_Zone = zone;
			
			while(SSIBusy(SSI0_BASE));

			// Zones numbered starting at 1, subtract to index
			SSIDataPut(SSI0_BASE, (0x01 << (zone - 1)));
		}
}

// Disables a zone
void Zone_Disable(void) {
	  Current_Zone = 0;
	
    while(SSIBusy(SSI0_BASE));
        
    SSIDataPut(SSI0_BASE, ZONE_NONE);
}


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

#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine){}
#endif

int main(void) {
    int i = 0;
  
    RIT128x96x4Init(1000000);
    RIT128x96x4Clear();
  
    WiFly_Init();
    RIT128x96x4StringDraw("TOGGLING LIGHTS", 0, 0, 15);
    while(1) {
      RIT128x96x4StringDraw("                            ", 0, 8, 15);
      RIT128x96x4StringDraw("                            ", 0, 8, 15);
      if(WiFly_Cmd("lites\r", "")) {
        RIT128x96x4StringDraw("SUCCESS", 0, 8, 15);
        if(i^=1) {RIT128x96x4StringDraw("LIGHTS ON ", 0, 16, 15);} 
        else {RIT128x96x4StringDraw("LIGHTS OFF", 0, 16, 15);}
      } 
      else {
        RIT128x96x4StringDraw("FAILURE", 0, 8, 15);
      }
      SysCtlDelay(SysCtlClockGet());
    }
}


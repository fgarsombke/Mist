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

#define NULL 0x0

#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine){}
#endif

int main0(void) {
    int i = 0;
    RIT128x96x4Init(1000000);
    RIT128x96x4Clear();
  
    RGB_Init();
    WiFly_Init();
    RIT128x96x4StringDraw("TOGGLING LIGHTS", 0, 0, 15);
    while(1) {
      RIT128x96x4StringDraw("                            ", 0, 8, 15);
      RIT128x96x4StringDraw("                            ", 0, 8, 15);
      if(WiFly_Send_Cmd("lites\r", NULL)) {
        RIT128x96x4StringDraw("SUCCESS", 0, 8, 15);
        RGB_On(GREEN); RGB_Off(RED);
        if(i^=1) {RIT128x96x4StringDraw("LIGHTS ON ", 0, 16, 15); RGB_On(BLUE);} 
        else {RIT128x96x4StringDraw("LIGHTS OFF", 0, 16, 15); RGB_Off(BLUE);}
      } 
      else {
        RGB_On(RED); RGB_Off(GREEN);
        RIT128x96x4StringDraw("FAILURE", 0, 8, 15);
      }
      SysCtlDelay(SysCtlClockGet());
    }
} 

int main1(void) {
    RIT128x96x4Init(1000000);
    RIT128x96x4Clear();
  
    RGB_Init();
  
    RIT128x96x4StringDraw("RGB TEST", 0, 0, 15);
    while(1) {
      RGB_Set(NONE);
      RIT128x96x4StringDraw("NONE     ", 0, 8, 15);
      SysCtlDelay(SysCtlClockGet()/3);
      RGB_Set(RED);
      RIT128x96x4StringDraw("RED      ", 0, 8, 15);
      SysCtlDelay(SysCtlClockGet()/3);
      RGB_Set(GREEN);
      RIT128x96x4StringDraw("GREEN    ", 0, 8, 15);
      SysCtlDelay(SysCtlClockGet()/3);
      RGB_Set(YELLOW);
      RIT128x96x4StringDraw("YELLOW   ", 0, 8, 15);
      SysCtlDelay(SysCtlClockGet()/3);
      RGB_Set(BLUE);
      RIT128x96x4StringDraw("BLUE     ", 0, 8, 15);
      SysCtlDelay(SysCtlClockGet()/3);
      RGB_Set(PURPLE);
      RIT128x96x4StringDraw("PURPLE   ", 0, 8, 15);
      SysCtlDelay(SysCtlClockGet()/3);
      RGB_Set(TURQUOISE);
      RIT128x96x4StringDraw("TURQUOISE", 0, 8, 15);
      SysCtlDelay(SysCtlClockGet()/3);
      RGB_Set(WHITE);
      RIT128x96x4StringDraw("WHITE    ", 0, 8, 15);
      SysCtlDelay(SysCtlClockGet()/3);
    }
}

int main(void){
    char buf[20];
    unsigned long rtc;
    RIT128x96x4Init(1000000);
    RIT128x96x4Clear();
    WiFly_Init();
    rtc = WiFly_Time();
    snprintf(buf, "%lu", rtc);
    RIT128x96x4StringDraw(buf, 0, 8, 15);
    return 0;
}


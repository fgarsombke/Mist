#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/rit128x96x4.h"
#include "jsmn.h"
#include "string.h"

#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine){}
#endif

volatile char json[9];
// The UART interrupt handler.
void UARTIntHandler(void) {
    unsigned long ulStatus;
    static unsigned short i = 0;
    static unsigned short j = 0;
    static unsigned short json_flag = 0;
    static unsigned short json_idx = 0;

    ulStatus = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, ulStatus);

    while(UARTCharsAvail(UART0_BASE)) {
        char c[2] = {0x0, 0x0};
        c[0] = UARTCharGetNonBlocking(UART0_BASE);
       
        if(c[0] == '{') {
          json_flag = 1;
          json_idx = 0;
        }
     
        if(json_flag == 1) {
          json[json_idx] = c[0];
          json_idx++;
        }
        
        if (c[0] == '}') {
          json_flag = 0;
        }
          
        if(c[0] == '\n') {
          j = (j + 1) % 12; // 12 lines
          i = 0;
        }
        
        RIT128x96x4StringDraw(c, i*8, j*8, 15);
        i = (i + 1) % 16; // 16 characters a line
    }
}

// Prototypes
void UARTSend(const unsigned char *, unsigned long);

int main(void) {
    char s[100];
  
    // setup our clock
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

    // screen init
    RIT128x96x4Init(1000000);
    RIT128x96x4Clear();

    // enable UART0 on PORTA
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // enable Interupts
    IntMasterEnable();

    // set pins to UART
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // configure UART0 9600 8-n-1
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // enable UART0 RX/TX interrupts
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    SysCtlDelay(SysCtlClockGet()/12);
    
    while(1) {
      RIT128x96x4Clear();
      UARTSend((unsigned char *)"close\r", 6);
      SysCtlDelay(SysCtlClockGet()/12);
      UARTSend((unsigned char *)"exit\r", 5);
      SysCtlDelay(SysCtlClockGet()/12);
      UARTSend((unsigned char *)"$$$", 3);
      SysCtlDelay(SysCtlClockGet()/12);
      UARTSend((unsigned char *)"close\r", 6);
      SysCtlDelay(SysCtlClockGet()/12);
      UARTSend((unsigned char *)"open\r", 5);
      SysCtlDelay(SysCtlClockGet());
      
      RIT128x96x4Clear();
      strcpy(s, (const char *)json);
      RIT128x96x4StringDraw(s, 0, 0, 15);
      SysCtlDelay(SysCtlClockGet()*3);
    }
}

// fill the buffer with data to send
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount) {
    while(ulCount--) {
        UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
    }
}


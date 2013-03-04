#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/lm3s811.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "string.h"
#include "zone.h"

#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine){}
#endif

char data[10];
volatile short flag = 0;
// The UART interrupt handler.
void UARTIntHandler(void) {
    unsigned long ulStatus;
//    static unsigned short i = 0;
//    static unsigned short j = 0;
    static unsigned short data_flag = 0;
    static unsigned short data_idx = 0;

    ulStatus = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, ulStatus);

    while(UARTCharsAvail(UART0_BASE)) {
        char c[2] = {0x0, 0x0};
        c[0] = UARTCharGetNonBlocking(UART0_BASE);
       
        if(c[0] == '(') {
          data_flag = 1;
          data_idx = 0;
        }
     
        if(data_flag == 1) {
          data[data_idx] = c[0];
          data_idx++;
        }
        
        if (c[0] == ')') {
          data_flag = 0;
          flag = 1;
        }
          
//         if(c[0] == '\n') {
//           j = (j + 1) % 12; // 12 lines
//           i = 0;
//         }
//         
//         RIT128x96x4StringDraw(c, i*8, j*8, 15);
//         i = (i + 1) % 16; // 16 characters a line
    }
}

// Prototypes
void adhocOff(void);
void UARTSend(const unsigned char *, unsigned long);

int main(void) {
    char s[10];
  
    // set up PD2&3 LEDs
    volatile unsigned long ulLoop;
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOD;
    ulLoop = SYSCTL_RCGC2_R;
    GPIO_PORTD_DIR_R = 0x0D;
    GPIO_PORTD_DEN_R = 0x0D;
  
    // setup our clock
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_6MHZ);

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
    
    adhocOff();
    Zone_Init();
    GPIO_PORTD_DATA_R &= ~(0x0C);
    while(1) {
      UARTSend((unsigned char *)"close\r", 6);
      UARTSend((unsigned char *)"exit\r", 5);
      
      SysCtlDelay(SysCtlClockGet()/12);
      UARTSend((unsigned char *)"$$$", 3);
      SysCtlDelay(SysCtlClockGet()/12);
      UARTSend((unsigned char *)"close\r", 6);
      
      UARTSend((unsigned char *)"open\r", 5);
      
      while(flag == 0){}
        
      strcpy(s, data);
      flag = 0;
      Zone_Disable(); // initialize all zones to zero
      if(!strcmp(s, "(1L, 1)")) {
        GPIO_PORTD_DATA_R |= 0x04;
        GPIO_PORTD_DATA_R &= ~(0x08);
        Zone_Enable(1);
      } else {
        GPIO_PORTD_DATA_R |= 0x08;
        GPIO_PORTD_DATA_R &= ~(0x04);
        Zone_Disable();
      }
    }
}

void adhocOff(void) {
  GPIO_PORTD_DATA_R &= ~(0x01);
}

// fill the buffer with data to send
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount) {
    while(ulCount--) {
        UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
    }
}


#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/lm3s1968.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "fifo.h"
#include "string.h"
#include "wifly.h"

#include "drivers/rit128x96x4.h"

#define FIFOSIZE   64       
#define FIFOSUCCESS 1       
#define FIFOFAIL    0         
AddIndexFifo(Rx, FIFOSIZE, char, FIFOSUCCESS, FIFOFAIL)

// The UART interrupt handler.
void UARTIntHandler(void) {
    unsigned long ulStatus;
    ulStatus = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, ulStatus);

    while(UARTCharsAvail(UART0_BASE)) {
        RxFifo_Put(UARTCharGetNonBlocking(UART0_BASE));
    }
}

void WiFly_Init(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
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
  
    RxFifo_Init();
    SysCtlDelay(SysCtlClockGet()/12);
}

void WiFly_Cmd(char * cmd) {
  static int call_count = 0;
  char response[25];
  short r_idx = 0;
  short status = FIFOSUCCESS;
  
  WiFly_Flush();
  SysCtlDelay(SysCtlClockGet()/12);
  UARTSend("$$$", 3);
  SysCtlDelay(SysCtlClockGet()/12);
  while((status == FIFOSUCCESS) && (response[r_idx] != '\n')) {
    status = RxFifo_Get(&response[r_idx]);
    r_idx++;
  }
  response[r_idx-1] = call_count + 0x30;
  response[r_idx] = 0x0;
  RIT128x96x4StringDraw(response, 0, 8, 15);
  call_count = (call_count + 1) % 10;
  r_idx = 0;
  
  UARTSend((unsigned char *)cmd, strlen(cmd));
  SysCtlDelay(SysCtlClockGet()/12);
  UARTSend("exit\r", 5);
  SysCtlDelay(SysCtlClockGet()/12);  
}

// Flush the buffer
void WiFly_Flush(void) {
  char c;
  while(RxFifo_Get(&c)) {}
}

// fill the buffer with data to send
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount) {
    while(ulCount--) {
        UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
    }
}

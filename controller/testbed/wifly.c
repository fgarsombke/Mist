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
#include "stdlib.h"
#include "wifly.h"

#include "drivers/rit128x96x4.h"
#include "rgb_led.h"

#define TRUE        1
#define FALSE       0
#define SUCCESS     1
#define FAILURE     0

#define MAX_ATTEMPTS 5

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


// Function sends a command to the module and checks
// for an expectend response
int WiFly_Send_Cmd(char * cmd, char * resp) {
  int status;
  int cmd_status = FAILURE;

  WiFly_Flush();
  
  status = WiFly_Send("$$$", "CMD");
  if(status) { 
    cmd_status = WiFly_Send(cmd, resp);
  }
  status = WiFly_Send("exit\r", "EXIT");
  return (status & cmd_status);
}

// Sends a string via UART to the module and will
// timeout after MAX_ATTEMPTS tries
int WiFly_Send(char * send, char * resp) {
  int status = SUCCESS;
  int attempts = 0;
  while(attempts < MAX_ATTEMPTS) {
    SysCtlDelay(SysCtlClockGet()/12);
    UART_Send((unsigned char*)send, strlen(send));
    SysCtlDelay(SysCtlClockGet()/12);
    if(UART_Match(resp)) {
      break;
    } 
    attempts++; 
  }
  
  if(attempts >= (MAX_ATTEMPTS - 1)) {
    status = FAILURE;
  }
  
  return status;
}

// Flush the buffer
void WiFly_Flush(void) {
  char c;
  while(RxFifo_Get(&c)) {}
}

unsigned long WiFly_Time(void) {
  int i, status;
  char time[11];
  status = WiFly_Send("$$$", "CMD");
  if(status) { 
    status = WiFly_Send("show t t", "RTC=");
    if(!status) {
      return 0;
    }
  } else {return 0;}
  
  for(i = 0; i < 10; i++) {
    RxFifo_Get(&time[i]);
  } time[10] = 0x0;
  return strtol(time, NULL, 10);
}

// Searchs for a desired string by reading
// through the recieved characters. This will remove
// items from the buffer.
int UART_Match(char * match) {
  char c; 
  int idx = 0;
  int m_len = strlen(match);
  int fifo_status = FIFOSUCCESS;
  while((m_len != idx) && (fifo_status == FIFOSUCCESS)) {
    fifo_status = RxFifo_Get(&c);
    if(c == match[idx]) {
      idx++;
    } else {
      idx = 0;
    }
  }
  
  if(m_len == idx) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
// fill the buffer with data to send
void UART_Send(const unsigned char *pucBuffer, unsigned long ulCount) {
    while(ulCount--) {
        UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
    }
}

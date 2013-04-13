#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/lm3s811.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "stdint.h"

#include "fifo.h"

// Initalizes UART0 with interrupts
void UART_Init(void) {
        // Initialize the FIFO
        RxFifo_Init();
    
        // Enable UART0 on PORTA
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    
        // Enable interrupts
        IntMasterEnable();
    
        // PA0/PA1 are UART0
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    
        // Configure UART0 for 9600 baud and 8-n-1
        UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
        
        // Enable UART0 RX/TX interrupts
        IntEnable(INT_UART0);
        UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}
  
// Sends "count" characters from "buffer" over UART
void UART_Send(const char *buffer, char count) {
    while(count--) {
        UARTCharPutNonBlocking(UART0_BASE, *buffer++);
    }
}

// Handler runs on RX
void UARTIntHandler(void) {
    unsigned long status;
    
    // Acknowledge the interrupt
    status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, status);

    // Remove data from the UART buffer and put it in a fifo
    while(UARTCharsAvail(UART0_BASE)) {
      RxFifo_Put(UARTCharGetNonBlocking(UART0_BASE));
    }
}

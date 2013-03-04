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

#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine){}
#endif

char BUFFER[100]; 
volatile unsigned short BFLAG = 0;

// The UART interrupt handler.
#define TOGGLE  1
void UARTIntHandler(void) {
    unsigned long ulStatus;
    static unsigned comFl   = 0;
    static unsigned jsonFl  = 0;
    static unsigned short i = 0;

    ulStatus = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, ulStatus);

    while(UARTCharsAvail(UART0_BASE)) {
        char c = UARTCharGetNonBlocking(UART0_BASE); 

        if(jsonFl == 1 && comFl == 0 && BFLAG == 0) {
            BUFFER[i] = c;
            i = i + 1;
        }

        if(c == '*') { 
            comFl ^= TOGGLE;
        } else if(comFl) {
            if(c == 'O') {
                jsonFl = 1;
                i = 0;
            } 
            if(c == 'C') {
                jsonFl = 0;
                BUFFER[i] = 0x0;
                BFLAG = 1;
            }
        }
    }
}

// Prototypes
void bufferClr(void);
void parseJSON(void);
void commandMode(void); 
void UARTSend(const unsigned char *, unsigned long);

int main(void) {
    char t[2] = {0x0,0x0};
    unsigned int i = 0;

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

    bufferClr();
    SysCtlDelay(SysCtlClockGet()/12);

    while(1) {
        UARTSend((unsigned char *)"GET /mist\r\n", 12);
        SysCtlDelay(SysCtlClockGet());
        if(BFLAG == 1) {
            RIT128x96x4Clear();
            t[0] = 0x30 + i;
            RIT128x96x4StringDraw(t, 12,  0, 15);
            i = (i + 1) % 10;
            RIT128x96x4StringDraw(BUFFER, 12,  8, 15);
            parseJSON();
            bufferClr();
            BFLAG = 0;
        }
    }
}

void bufferClr(void) {
    unsigned int i;
    for(i = 0; i < 100; i++) {
        BUFFER[i] = 0;
    }
}

void parseJSON(void) {
    unsigned short i;
    jsmn_parser parser;
    jsmntok_t t[8];
    jsmn_init(&parser);

    jsmn_parse(&parser, BUFFER, t, 10);
    for(i = 0; i < 8; i++) {
        if(t[i].type == JSMN_OBJECT) {
            RIT128x96x4StringDraw("OBJECT", 12,  (i+2)*8, 15);
        } else if(t[i].type == JSMN_ARRAY) {
            RIT128x96x4StringDraw("ARRAY", 12,  (i+2)*8, 15);
        } else if(t[i].type == JSMN_STRING) {
            BUFFER[t[i].end] = 0x0;
            RIT128x96x4StringDraw("STRING:", 12,  (i+2)*8, 15);
            RIT128x96x4StringDraw(&(BUFFER[t[i].start]), 12+6*7,  (i+2)*8, 15);
        } else if(t[i].type == JSMN_PRIMITIVE) {
            RIT128x96x4StringDraw("PRIMITIVE", 12,  (i+2)*8, 15);
        } else {
            RIT128x96x4StringDraw("UNKNOWN", 12,  (i+2)*8, 15);
        }
    }
}

// enter command mode
void commandMode(void) {
    RIT128x96x4Clear();
    SysCtlDelay(SysCtlClockGet()/12); // 250ms delay
    UARTSend((unsigned char *)"$$$", 3);
    SysCtlDelay(SysCtlClockGet()/12); // 250ms delay
}

// fill the buffer with data to send
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount) {
    while(ulCount--) {
        UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
    }
}


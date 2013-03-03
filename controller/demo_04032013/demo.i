#line 1 "demo.c"
#line 1 "..\\inc/hw_ints.h"















































#line 57 "..\\inc/hw_ints.h"






#line 174 "..\\inc/hw_ints.h"




























#line 214 "..\\inc/hw_ints.h"



#line 2 "demo.c"
#line 1 "..\\inc/hw_memmap.h"
















































#line 127 "..\\inc/hw_memmap.h"
                                            
#line 143 "..\\inc/hw_memmap.h"














#line 164 "..\\inc/hw_memmap.h"



#line 3 "demo.c"
#line 1 "..\\inc/hw_types.h"















































typedef unsigned char tBoolean;














#line 78 "..\\inc/hw_types.h"






























#line 114 "..\\inc/hw_types.h"
































































































#line 217 "..\\inc/hw_types.h"

#line 4 "demo.c"
#line 1 "..\\driverlib/debug.h"

































extern void __error__(char *pcFilename, unsigned long ulLine);







#line 52 "..\\driverlib/debug.h"

#line 5 "demo.c"
#line 1 "..\\driverlib/gpio.h"












































#line 53 "..\\driverlib/gpio.h"








































#line 100 "..\\driverlib/gpio.h"





















































#line 160 "..\\driverlib/gpio.h"




#line 172 "..\\driverlib/gpio.h"













































#line 225 "..\\driverlib/gpio.h"




#line 236 "..\\driverlib/gpio.h"





























#line 272 "..\\driverlib/gpio.h"




#line 283 "..\\driverlib/gpio.h"




#line 295 "..\\driverlib/gpio.h"




#line 306 "..\\driverlib/gpio.h"




#line 318 "..\\driverlib/gpio.h"




#line 332 "..\\driverlib/gpio.h"
















































#line 386 "..\\driverlib/gpio.h"













#line 405 "..\\driverlib/gpio.h"




#line 415 "..\\driverlib/gpio.h"




#line 425 "..\\driverlib/gpio.h"










































#line 473 "..\\driverlib/gpio.h"






















































#line 533 "..\\driverlib/gpio.h"






































#line 577 "..\\driverlib/gpio.h"











































































































































extern void GPIODirModeSet(unsigned long ulPort, unsigned char ucPins,
                           unsigned long ulPinIO);
extern unsigned long GPIODirModeGet(unsigned long ulPort, unsigned char ucPin);
extern void GPIOIntTypeSet(unsigned long ulPort, unsigned char ucPins,
                           unsigned long ulIntType);
extern unsigned long GPIOIntTypeGet(unsigned long ulPort, unsigned char ucPin);
extern void GPIOPadConfigSet(unsigned long ulPort, unsigned char ucPins,
                             unsigned long ulStrength,
                             unsigned long ulPadType);
extern void GPIOPadConfigGet(unsigned long ulPort, unsigned char ucPin,
                             unsigned long *pulStrength,
                             unsigned long *pulPadType);
extern void GPIOPinIntEnable(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinIntDisable(unsigned long ulPort, unsigned char ucPins);
extern long GPIOPinIntStatus(unsigned long ulPort, tBoolean bMasked);
extern void GPIOPinIntClear(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPortIntRegister(unsigned long ulPort,
                                void (*pfnIntHandler)(void));
extern void GPIOPortIntUnregister(unsigned long ulPort);
extern long GPIOPinRead(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinWrite(unsigned long ulPort, unsigned char ucPins,
                         unsigned char ucVal);
extern void GPIOPinConfigure(unsigned long ulPinConfig);
extern void GPIOPinTypeADC(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeCAN(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeComparator(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeEPI(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeEthernetLED(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeGPIOInput(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeGPIOOutput(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeGPIOOutputOD(unsigned long ulPort,
                                    unsigned char ucPins);
extern void GPIOPinTypeI2C(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeI2S(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypePWM(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeQEI(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeSSI(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeTimer(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeUART(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeUSBAnalog(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeUSBDigital(unsigned long ulPort, unsigned char ucPins);










#line 6 "demo.c"
#line 1 "..\\driverlib/interrupt.h"



















































extern tBoolean IntMasterEnable(void);
extern tBoolean IntMasterDisable(void);
extern void IntRegister(unsigned long ulInterrupt, void (*pfnHandler)(void));
extern void IntUnregister(unsigned long ulInterrupt);
extern void IntPriorityGroupingSet(unsigned long ulBits);
extern unsigned long IntPriorityGroupingGet(void);
extern void IntPrioritySet(unsigned long ulInterrupt,
                           unsigned char ucPriority);
extern long IntPriorityGet(unsigned long ulInterrupt);
extern void IntEnable(unsigned long ulInterrupt);
extern void IntDisable(unsigned long ulInterrupt);
extern void IntPendSet(unsigned long ulInterrupt);
extern void IntPendClear(unsigned long ulInterrupt);










#line 7 "demo.c"
#line 1 "..\\driverlib/sysctl.h"















































#line 106 "..\\driverlib/sysctl.h"







#line 146 "..\\driverlib/sysctl.h"







#line 164 "..\\driverlib/sysctl.h"
















#line 189 "..\\driverlib/sysctl.h"







#line 202 "..\\driverlib/sysctl.h"

















#line 226 "..\\driverlib/sysctl.h"



















#line 405 "..\\driverlib/sysctl.h"






extern unsigned long SysCtlSRAMSizeGet(void);
extern unsigned long SysCtlFlashSizeGet(void);
extern tBoolean SysCtlPinPresent(unsigned long ulPin);
extern tBoolean SysCtlPeripheralPresent(unsigned long ulPeripheral);
extern void SysCtlPeripheralReset(unsigned long ulPeripheral);
extern void SysCtlPeripheralEnable(unsigned long ulPeripheral);
extern void SysCtlPeripheralDisable(unsigned long ulPeripheral);
extern void SysCtlPeripheralSleepEnable(unsigned long ulPeripheral);
extern void SysCtlPeripheralSleepDisable(unsigned long ulPeripheral);
extern void SysCtlPeripheralDeepSleepEnable(unsigned long ulPeripheral);
extern void SysCtlPeripheralDeepSleepDisable(unsigned long ulPeripheral);
extern void SysCtlPeripheralClockGating(tBoolean bEnable);
extern void SysCtlIntRegister(void (*pfnHandler)(void));
extern void SysCtlIntUnregister(void);
extern void SysCtlIntEnable(unsigned long ulInts);
extern void SysCtlIntDisable(unsigned long ulInts);
extern void SysCtlIntClear(unsigned long ulInts);
extern unsigned long SysCtlIntStatus(tBoolean bMasked);
extern void SysCtlLDOSet(unsigned long ulVoltage);
extern unsigned long SysCtlLDOGet(void);
extern void SysCtlLDOConfigSet(unsigned long ulConfig);
extern void SysCtlReset(void);
extern void SysCtlSleep(void);
extern void SysCtlDeepSleep(void);
extern unsigned long SysCtlResetCauseGet(void);
extern void SysCtlResetCauseClear(unsigned long ulCauses);
extern void SysCtlBrownOutConfigSet(unsigned long ulConfig,
                                    unsigned long ulDelay);
extern void SysCtlDelay(unsigned long ulCount);
extern void SysCtlClockSet(unsigned long ulConfig);
extern unsigned long SysCtlClockGet(void);
extern void SysCtlPWMClockSet(unsigned long ulConfig);
extern unsigned long SysCtlPWMClockGet(void);
extern void SysCtlADCSpeedSet(unsigned long ulSpeed);
extern unsigned long SysCtlADCSpeedGet(void);
extern void SysCtlIOSCVerificationSet(tBoolean bEnable);
extern void SysCtlMOSCVerificationSet(tBoolean bEnable);
extern void SysCtlPLLVerificationSet(tBoolean bEnable);
extern void SysCtlClkVerificationClear(void);
extern void SysCtlGPIOAHBEnable(unsigned long ulGPIOPeripheral);
extern void SysCtlGPIOAHBDisable(unsigned long ulGPIOPeripheral);
extern void SysCtlUSBPLLEnable(void);
extern void SysCtlUSBPLLDisable(void);
extern unsigned long SysCtlI2SMClkSet(unsigned long ulInputClock,
                                      unsigned long ulMClk);










#line 8 "demo.c"
#line 1 "..\\driverlib/uart.h"












































#line 56 "..\\driverlib/uart.h"










#line 80 "..\\driverlib/uart.h"























































































extern void UARTParityModeSet(unsigned long ulBase, unsigned long ulParity);
extern unsigned long UARTParityModeGet(unsigned long ulBase);
extern void UARTFIFOLevelSet(unsigned long ulBase, unsigned long ulTxLevel,
                             unsigned long ulRxLevel);
extern void UARTFIFOLevelGet(unsigned long ulBase, unsigned long *pulTxLevel,
                             unsigned long *pulRxLevel);
extern void UARTConfigSetExpClk(unsigned long ulBase, unsigned long ulUARTClk,
                                unsigned long ulBaud, unsigned long ulConfig);
extern void UARTConfigGetExpClk(unsigned long ulBase, unsigned long ulUARTClk,
                                unsigned long *pulBaud,
                                unsigned long *pulConfig);
extern void UARTEnable(unsigned long ulBase);
extern void UARTDisable(unsigned long ulBase);
extern void UARTFIFOEnable(unsigned long ulBase);
extern void UARTFIFODisable(unsigned long ulBase);
extern void UARTEnableSIR(unsigned long ulBase, tBoolean bLowPower);
extern void UARTDisableSIR(unsigned long ulBase);
extern tBoolean UARTCharsAvail(unsigned long ulBase);
extern tBoolean UARTSpaceAvail(unsigned long ulBase);
extern long UARTCharGetNonBlocking(unsigned long ulBase);
extern long UARTCharGet(unsigned long ulBase);
extern tBoolean UARTCharPutNonBlocking(unsigned long ulBase,
                                       unsigned char ucData);
extern void UARTCharPut(unsigned long ulBase, unsigned char ucData);
extern void UARTBreakCtl(unsigned long ulBase, tBoolean bBreakState);
extern tBoolean UARTBusy(unsigned long ulBase);
extern void UARTIntRegister(unsigned long ulBase, void(*pfnHandler)(void));
extern void UARTIntUnregister(unsigned long ulBase);
extern void UARTIntEnable(unsigned long ulBase, unsigned long ulIntFlags);
extern void UARTIntDisable(unsigned long ulBase, unsigned long ulIntFlags);
extern unsigned long UARTIntStatus(unsigned long ulBase, tBoolean bMasked);
extern void UARTIntClear(unsigned long ulBase, unsigned long ulIntFlags);
extern void UARTDMAEnable(unsigned long ulBase, unsigned long ulDMAFlags);
extern void UARTDMADisable(unsigned long ulBase, unsigned long ulDMAFlags);
extern unsigned long UARTRxErrorGet(unsigned long ulBase);
extern void UARTRxErrorClear(unsigned long ulBase);
extern void UARTSmartCardEnable(unsigned long ulBase);
extern void UARTSmartCardDisable(unsigned long ulBase);
extern void UARTModemControlSet(unsigned long ulBase,
                                unsigned long ulControl);
extern void UARTModemControlClear(unsigned long ulBase,
                                  unsigned long ulControl);
extern unsigned long UARTModemControlGet(unsigned long ulBase);
extern unsigned long UARTModemStatusGet(unsigned long ulBase);
extern void UARTFlowControlSet(unsigned long ulBase, unsigned long ulMode);
extern unsigned long UARTFlowControlGet(unsigned long ulBase);
extern void UARTTxIntModeSet(unsigned long ulBase, unsigned long ulMode);
extern unsigned long UARTTxIntModeGet(unsigned long ulBase);







#line 224 "..\\driverlib/uart.h"
#line 233 "..\\driverlib/uart.h"










#line 9 "demo.c"
#line 1 "..\\drivers/rit128x96x4.h"

































extern void RIT128x96x4Clear(void);
extern void RIT128x96x4StringDraw(const char *pcStr,
                                    unsigned long ulX,
                                    unsigned long ulY,
                                    unsigned char ucLevel);
extern void RIT128x96x4ImageDraw(const unsigned char *pucImage,
                                   unsigned long ulX,
                                   unsigned long ulY,
                                   unsigned long ulWidth,
                                   unsigned long ulHeight);
extern void RIT128x96x4Init(unsigned long ulFrequency);
extern void RIT128x96x4Enable(unsigned long ulFrequency);
extern void RIT128x96x4Disable(void);
extern void RIT128x96x4DisplayOn(void);
extern void RIT128x96x4DisplayOff(void);

#line 10 "demo.c"
#line 1 "jsmn.h"









 
typedef enum {
	JSMN_PRIMITIVE = 0,
	JSMN_OBJECT = 1,
	JSMN_ARRAY = 2,
	JSMN_STRING = 3
} jsmntype_t;

typedef enum {
	 
	JSMN_ERROR_NOMEM = -1,
	 
	JSMN_ERROR_INVAL = -2,
	 
	JSMN_ERROR_PART = -3,
	 
	JSMN_SUCCESS = 0
} jsmnerr_t;






 
typedef struct {
	jsmntype_t type;
	int start;
	int end;
	int size;



} jsmntok_t;




 
typedef struct {
	unsigned int pos;  
	int toknext;  
	int toksuper;  
} jsmn_parser;



 
void jsmn_init(jsmn_parser *parser);




 
jsmnerr_t jsmn_parse(jsmn_parser *parser, const char *js, 
		jsmntok_t *tokens, unsigned int num_tokens);

#line 11 "demo.c"





volatile char json[9];

void UARTIntHandler(void) {
    unsigned long ulStatus;
    static unsigned short i = 0;
    static unsigned short j = 0;
    static unsigned short json_flag = 0;
    static unsigned short json_idx = 0;

    ulStatus = UARTIntStatus(0x4000C000, 1);
    UARTIntClear(0x4000C000, ulStatus);

    while(UARTCharsAvail(0x4000C000)) {
        char c[2] = {0x0, 0x0};
        c[0] = UARTCharGetNonBlocking(0x4000C000);
       
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
          j = (j + 1) % 12; 
          i = 0;
        }
        
        RIT128x96x4StringDraw(c, i*8, j*8, 15);
        i = (i + 1) % 16; 
    }
}


void bufferClr(void);
void parseJSON(void);
void commandMode(void); 
void UARTSend(const unsigned char *, unsigned long);

int main(void) {
    
    SysCtlClockSet(0x07800000 | 0x00003800 | 0x00000000 | 0x00000380);

    
    RIT128x96x4Init(1000000);
    RIT128x96x4Clear();

    
    SysCtlPeripheralEnable(0x10000001);
    SysCtlPeripheralEnable(0x20000001);

    
    IntMasterEnable();

    
    GPIOPinTypeUART(0x40004000, 0x00000001 | 0x00000002);

    
    UARTConfigSetExpClk(0x4000C000, SysCtlClockGet(), 9600, (0x00000060 | 0x00000000 | 0x00000000));

    
    IntEnable(21);
    UARTIntEnable(0x4000C000, 0x010 | 0x040);
    SysCtlDelay(SysCtlClockGet()/12);
    
    RIT128x96x4Clear();
    UARTSend((unsigned char *)"close\r", 6);
    SysCtlDelay(SysCtlClockGet()/12);
    UARTSend((unsigned char *)"exit\r", 5);
    SysCtlDelay(SysCtlClockGet()/12);
    UARTSend((unsigned char *)"$$$", 3);
    SysCtlDelay(SysCtlClockGet()/12);
    UARTSend((unsigned char *)"open\r", 5);
    SysCtlDelay(SysCtlClockGet()/12);
    UARTSend((unsigned char *)"exit\r", 5);
    SysCtlDelay(SysCtlClockGet()/12);
    
    while(1) {
    }
}


void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount) {
    while(ulCount--) {
        UARTCharPutNonBlocking(0x4000C000, *pucBuffer++);
    }
}


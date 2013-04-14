#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "inc/lm3s811.h"

//#include "drivers/rit128x96x4.h"

#include "stdlib.h"
#include "string.h"

#include "fifo.h"
#include "uart.h"
#include "wifly.h"

#define NO_RSP  NULL
const char* CMD_CMD = "$$$";
const char* CMD_RSP = "CMD";
const char* EXIT_CMD = "exit\r";
const char* EXIT_RSP = "EXIT";
const char* TIME_CMD = "show t t\r";
const char* TIME_RSP = "RTC=";
const char* OPEN_CMD = "open\r";

#define ADHOC_IO 0x01
#define RESET_IO 0x10

// Delays 250ms based on the clock speed
void delay250ms(void){ SysCtlDelay(SysCtlClockGet()/12); }

void WiFly_Adhoc_On(void) {
  GPIO_PORTD_DATA_R |= ADHOC_IO;
}

void WiFly_Adhoc_Off(void) {
  GPIO_PORTD_DATA_R &= ~(ADHOC_IO);
}

// Initializes the WiFly module
void WiFly_Init(void) {
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
    delay = SYSCTL_RCGC2_R;
    
    GPIO_PORTD_DIR_R |= (ADHOC_IO | RESET_IO);
    GPIO_PORTD_DEN_R |= (ADHOC_IO | RESET_IO);
    
    UART_Init();
    delay250ms();
    WiFly_Adhoc_Off();
    WiFly_Reset();
    WiFly_Send_Cmd("time/r", NO_RSP);
}

// Searchs for a desired string by reading
// through the recieved characters. This will remove
// items from the buffer.
tBoolean WiFly_Match(const char * match) {
  char ch; 
  long idx = 0;
  long len = strlen(match);
  tBoolean status = true;
  while((len != idx) && status) {
    status = RxFifo_Get(&ch);
    idx = (ch == match[idx]) ? (idx+1) : 0;
  }
  
  if(len == idx) status = true;
  return status;
}

void WiFly_Reset(void) {
  GPIO_PORTD_DATA_R &= ~(RESET_IO);
  delay250ms();
  GPIO_PORTD_DATA_R |= RESET_IO;
}

// Sends a string to the WiFly module and will timeout after MAX_ATTEMPTS
#define MAX_ATTEMPTS 5
tBoolean WiFly_Send(const char * send, const char * resp) {
  tBoolean status = true;
  unsigned char attempts = 0;
    
  while(attempts < MAX_ATTEMPTS) {
    delay250ms();
    UART_Send((unsigned char*)send, strlen(send));
    delay250ms();
      
    if(!resp) break; // Case where no response is required
    
    if(WiFly_Match(resp)) break;
    
    attempts++; 
  }
  
  // return failure
  if(attempts >= (MAX_ATTEMPTS - 1)) status = false; 
  return status;
}

// Sends a command and searches for a response
tBoolean WiFly_Send_Cmd(const char * cmd, const char * resp) {
  tBoolean cmd_mode_status = false;
  tBoolean cmd_status = false;

  RxFifo_Flush(); // Flush the buffer
  
  cmd_mode_status = WiFly_Send(CMD_CMD, CMD_RSP); 
    
  if(cmd_mode_status) { 
    cmd_status = WiFly_Send(cmd, resp); 
  }
  
  cmd_mode_status = WiFly_Send(EXIT_CMD, EXIT_RSP);
  
  return (cmd_mode_status & cmd_status);
}

// Gets posix time from NIST servers
// TODO: Use the NIST DNS for reliability
unsigned long WiFly_Time(void){
  tBoolean status = false;
  unsigned char i = 0;
  unsigned long time = 0;
  char time_str[11]; // A string formatted long can only be 10 characters
    
  RxFifo_Flush();
    
  status = WiFly_Send(CMD_CMD, CMD_RSP); 
  if(status) { 
    status = WiFly_Send(TIME_CMD, TIME_RSP); // Get time
    if(status) {
          for(i = 0; i < 10; i++) RxFifo_Get(&time_str[i]);
          time_str[10] = 0x0;
          //RIT128x96x4StringDraw(time_str, 0, 0, 15);
          time = strtoul(time_str, NULL, 0); // Convert to unsigned long
    }
  }
  
  WiFly_Send(EXIT_CMD, EXIT_RSP); 
  
  return time;
}

// Opens a network connection to the server
tBoolean WiFly_Open(char *resp) {
  tBoolean status = false;
  char* resp_pt = resp;    
    
  RxFifo_Flush(); // Flush the FIFO
    
  status = WiFly_Send(CMD_CMD, CMD_RSP);
  if(status){ 
    status = WiFly_Send(OPEN_CMD, NO_RSP);
    if(status){
      SysCtlDelay(SysCtlClockGet()*3); // Blocking wait
      while(RxFifo_Get(resp_pt)) resp_pt++;
      *resp_pt = NULL;
    }
  }
  
  WiFly_Send(EXIT_CMD, EXIT_RSP); 
  
  return status;
}


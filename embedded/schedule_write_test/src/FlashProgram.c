// FlashProgram.c
// Runs on LM3S811
// Provide functions that initialize the flash memory, write
// 32-bit data to flash, write an array of 32-bit data to flash,
// and erase a 1 KB block.
// Daniel Valvano
// June 30, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011

 Copyright 2011 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// NOTE: To verify that this program works correctly, first open the "Flash"
//  menu and select "Configure Flash Tools..."
// Then select the "Utilities" sub-tab
// Then click the "Setting" button near the pull-down menu that says
//  "Stellaris ICDI"
// Finally, click the button that says "Mass Erase (erase entire Flash)"
// Click both "OK" buttons
//
// This process guarantees that the flash memory at 0x0000.8000 is clear
//  (set to all 1's) in case a previously loaded program left data there.
// If for some reason the flash memory at 0x0000.8000 is not clear before
//  running this program, change the addresses to somewhere that is clear.
//  This program is also stored into flash, and you do not want to write
//  garbage over it!


#define FLASH_FMA_R             (*((volatile unsigned long *)0x400FD000))
#define FLASH_FMD_R             (*((volatile unsigned long *)0x400FD004))
#define FLASH_FMC_R             (*((volatile unsigned long *)0x400FD008))
#define FLASH_USECRL_R          (*((volatile unsigned long *)0x400FE140))
#define FLASH_FMC_WRKEY         0xA4420000  // FLASH write key
#define FLASH_FMC_MERASE        0x00000004  // Mass Erase Flash Memory
#define FLASH_FMC_ERASE         0x00000002  // Erase a Page of Flash Memory
#define FLASH_FMC_WRITE         0x00000001  // Write a Word into Flash Memory
#define FLASH_USECRL_M          0x000000FF  // Microsecond Reload Value mask
#define ERROR                   1           // Value returned if failure
#define NOERROR                 0           // Value returned if success

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
#define FLASH_FMA_OFFSET_MAX    0x0000FFFF  // Address Offset max

// Check if address offset is valid for write operation
// Writing addresses must be 4-byte aligned and within range
static int WriteAddrValid(unsigned long addr){
  // check if address offset works for writing
  // must be 4-byte aligned
  return (((addr % 4) == 0) && (addr <= FLASH_FMA_OFFSET_MAX));
}
// Check if address offset is valid for erase operation
// Erasing addresses must be 1 KB aligned and within range
static int EraseAddrValid(unsigned long addr){
  // check if address offset works for erasing
  // must be 1 KB aligned
  return (((addr % 1024) == 0) && (addr <= FLASH_FMA_OFFSET_MAX));
}

// Initialize flash memory timer
// This function MUST be run before trying to write or erase the flash and
// any time that the system clock frequency changes
void Flash_Init(unsigned char systemClockFreqMHz){
  FLASH_USECRL_R = (systemClockFreqMHz-1)&FLASH_USECRL_M;
}
// Write 32-bit data to flash
// Return NOERROR if successful
int Flash_Write(unsigned long addr, unsigned long data){
  if(WriteAddrValid(addr)){
    DisableInterrupts();                            // may be optional step
                                                    // wait for hardware idle
    while(FLASH_FMC_R&(FLASH_FMC_WRITE|FLASH_FMC_ERASE|FLASH_FMC_MERASE)){
                 // to do later: return ERROR if this takes too long
                 // remember to re-enable interrupts
    };
    FLASH_FMD_R = data;
    FLASH_FMA_R = addr;
    FLASH_FMC_R = (FLASH_FMC_WRKEY|FLASH_FMC_WRITE);// start writing
    while(FLASH_FMC_R&FLASH_FMC_WRITE){
                 // to do later: return ERROR if this takes too long
                 // remember to re-enable interrupts
    };           // wait for completion (~3 to 4 usec)
    EnableInterrupts();
    return NOERROR;
  }
  return ERROR;
}
// Write an array of 32-bit data to flash
// Return number of successful writes; return value == count if completely successful
int Flash_ProgramArray(unsigned long *source, unsigned long addr, unsigned short count){
  unsigned short successfulWrites = 0;
  while((successfulWrites < count) && (Flash_Write(addr + 4*successfulWrites, source[successfulWrites]) == NOERROR)){
    successfulWrites = successfulWrites + 1;
  }
  return successfulWrites;
}
// Erase 1 KB block of flash
// Return NOERROR if successful
int Flash_Erase(unsigned long addr) {
  if(EraseAddrValid(addr)){
    DisableInterrupts();                            // may be optional step
                                                    // wait for hardware idle
    while(FLASH_FMC_R&(FLASH_FMC_WRITE|FLASH_FMC_ERASE|FLASH_FMC_MERASE)){
                 // to do later: return ERROR if this takes too long
                 // remember to re-enable interrupts
    };
    FLASH_FMA_R = addr;
    FLASH_FMC_R = (FLASH_FMC_WRKEY|FLASH_FMC_ERASE);// start erasing 1 KB block
    while(FLASH_FMC_R&FLASH_FMC_ERASE){
                 // to do later: return ERROR if this takes too long
                 // remember to re-enable interrupts
    };           // wait for completion (~3 to 4 usec)
    EnableInterrupts();
    return NOERROR;
  }
  return ERROR;
}

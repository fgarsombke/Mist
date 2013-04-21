
#include "file_system.h"
#include "flash_prog.h"
     
#define SCHEDULE_REGISTRY_SIZE   256
#define SCHEDULE_REGISTRY_ADDR   0x00007C00
#define EMPTY_SCHEDULE_ENTRY     0xFFFFFFFF
#define SCHEDULE_BUFFER_A_ADDR   0x00008000
#define SCHEDULE_BUFFER_B_ADDR   0x0000C000

#define CLOCK_FREQ_MHZ          6           // 6 MHz clock frequency


/* TODO - MH - Discuss - too much space 
typedef union {
    unsigned long words[64][256]; // broken up into sectors...
    schedule_entry_t entries[341];
} schedule_t;
*/

unsigned long *ScheduleRegistry; 
unsigned long ScheduleRegistrySize;

unsigned long *VALID_SCHEDULE;
unsigned long *INVALID_SCHEDULE;

unsigned long ScheduleSize, TempScheduleSize;


schedule_entry_t *Schedule, *TempSchedule;

// TODO schedule_entry_t Schedule_Get_Entry(...)

void Schedule_Init(void)
{
    Flash_Init(CLOCK_FREQ_MHZ);

    ScheduleRegistry = (unsigned long *) SCHEDULE_REGISTRY_ADDR;
	  ScheduleRegistrySize = 0;

    // look for current schedule (if one exists!)
    while((ScheduleRegistry[ScheduleRegistrySize] != EMPTY_SCHEDULE_ENTRY) && (ScheduleRegistrySize < 256)) {
        ScheduleRegistrySize++;
    }

    // check if first time loading a schedule
    if((ScheduleRegistrySize % SCHEDULE_REGISTRY_SIZE) == 0) { 
        Flash_Erase(SCHEDULE_REGISTRY_ADDR);
        Flash_Erase(SCHEDULE_BUFFER_A_ADDR);
        Flash_Write(SCHEDULE_REGISTRY_ADDR, SCHEDULE_BUFFER_A_ADDR);
        Schedule = (schedule_entry_t *) SCHEDULE_BUFFER_A_ADDR;
			  TempSchedule = (schedule_entry_t *) SCHEDULE_BUFFER_B_ADDR;
        ScheduleSize = 0;
			  TempScheduleSize = 0;
			  ScheduleRegistrySize = 1;
    } else {
        Schedule = (schedule_entry_t *) ScheduleRegistry[ScheduleRegistrySize-1];
        ScheduleSize = TempScheduleSize;
			  TempScheduleSize = 0;
        if(Schedule == (schedule_entry_t *) SCHEDULE_BUFFER_A_ADDR)  {
            TempSchedule = (schedule_entry_t *) SCHEDULE_BUFFER_B_ADDR;
        } 
        else {  
            TempSchedule = (schedule_entry_t *) SCHEDULE_BUFFER_A_ADDR;
        }
    }
}

int Schedule_Enter(schedule_entry_t *entry_ptr)
{
    unsigned long next_entry_addr = (unsigned long) &TempSchedule[TempScheduleSize];
    // make sure sure schedule buffer is not full
    // TODO make -1 an error enum?

    // TODO make SCHEDULE_SIZE
    if(next_entry_addr > ((unsigned long) TempSchedule + 0x4000)) 
        return -1;

    // erase flash sector before starting to write schedule there
    if(next_entry_addr % 0x400 == 0) // TODO FLASH_SECTOR_SIZE
        Flash_Erase(next_entry_addr);

    //entry_ptr->not_completed = 0xFFFFFFFF; // TODO
    Flash_ProgramArray((unsigned long *) entry_ptr, next_entry_addr, 4);

    TempScheduleSize++;

    return 0; // TODO SUCCESS
}

void Schedule_Refresh(void) 
{
    if(ScheduleRegistrySize == 256) {
        Flash_Erase(SCHEDULE_REGISTRY_ADDR);
			  ScheduleRegistrySize = 0;
    }
		
		Flash_Write((unsigned long) &ScheduleRegistry[ScheduleRegistrySize], (unsigned long) TempSchedule);
		TempSchedule = Schedule;
	  Schedule = (schedule_entry_t *) ScheduleRegistry[ScheduleRegistrySize];
		ScheduleRegistrySize++;
	  
		ScheduleSize = TempScheduleSize;
	  TempScheduleSize = 0;
}

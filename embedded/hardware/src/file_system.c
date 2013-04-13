
#define SCHEDULE_SIZE 16384

#define SCHEDULE_REGISTRY_SIZE   256
#define SCHEDULE_REGISTRY_ADDR   0x00008000
#define EMPTY_SCHEDULE_ENTRY     0xFFFFFFFF
#define SCHEDULE_BUFFER_A_ADDR   0x000084000
#define SCHEDULE_BUFFER_B_ADDR   0x0000C400

#define CLOCK_FREQ_MHZ          6           // 6 MHz clock frequency

void Flash_Init(unsigned char systemClockFreqMHz);
int Flash_Write(unsigned long addr, unsigned long data);
int Flash_ProgramArray(unsigned long *source, unsigned long addr, unsigned short count);
int Flash_Erase(unsigned long addr);


typedef struct {
    unsigned long zone;
    unsigned long start_time;
    unsigned long end_time;
    unsigned long not_completed;
} schedule_entry_t;

typedef union {
    unsigned long words[64][256]; // broken up into sectors...
    schedule_entry_t entries[341];
} schedule_t;

unsigned long *Schedule_Registry; 

unsigned long *VALID_SCHEDULE;
unsigned long *INVALID_SCHEDULE;
unsigned long ScheduleIndex = 0;

unsigned long *Schedule, *TempSchedule, *NextEmptyScheduleEntry;

// TODO schedule_entry_t Schedule_Get_Entry(...)

void Schedule_Init(void)
{
    unsigned long i = 0;

    Flash_Init(CLOCK_FREQ_MHZ);

    Schedule_Registry = (unsigned long *) SCHEDULE_REGISTRY_ADDR;

    // look for current schedule (if one exists!)
    while((Schedule_Registry[i] == EMPTY_SCHEDULE_ENTRY) && (i < 256)) {
        i++;
    }

    // check if first time loading a schedule
    if(i == SCHEDULE_REGISTRY_SIZE    ) { 
        i = 0;
        Flash_Erase(SCHEDULE_REGISTRY_ADDR);
        Flash_Erase(SCHEDULE_BUFFER_A_ADDR);
        Flash_Write(SCHEDULE_REGISTRY_ADDR, SCHEDULE_BUFFER_A_ADDR);
        Schedule = (unsigned long *) SCHEDULE_BUFFER_A_ADDR;
        ScheduleIndex = 0;
    } else {
        Schedule = (unsigned long *) Schedule_Registry[i];
        ScheduleIndex = 0;
        if(Schedule == (unsigned long*) SCHEDULE_BUFFER_A_ADDR)  {
            TempSchedule = (unsigned long *) SCHEDULE_BUFFER_B_ADDR;
            NextEmptyScheduleEntry = TempSchedule;
        } 
        else {  
            TempSchedule = (unsigned long *) SCHEDULE_BUFFER_A_ADDR;
            NextEmptyScheduleEntry = TempSchedule;
        }
    }
}

int Schedule_Enter(schedule_entry_t *entry_ptr)
{
    unsigned long next_entry_addr = (unsigned long) NextEmptyScheduleEntry;
    // make sure sure schedule buffer is not full
    // TODO make -1 an error enum?

    // TODO make SCHEDULE_SIZE
    if(next_entry_addr > ((unsigned long) TempSchedule + 0x4000)) 
        return -1;

    // erase flash sector before starting to write schedule there
    if(next_entry_addr % 0x400 == 0) // TODO FLASH_SECTOR_SIZE
        Flash_Erase(next_entry_addr);

    entry_ptr->not_completed = 0xFFFFFFFF; // TODO
    Flash_ProgramArray((unsigned long *) entry_ptr, next_entry_addr, 4);

    NextEmptyScheduleEntry = NextEmptyScheduleEntry + 4; 

    return 0; // TODO SUCCESS
}

void Schedule_Refresh(void) 
{
    if(Schedule == (unsigned long *) SCHEDULE_BUFFER_A_ADDR)  {
        Schedule = (unsigned long *) SCHEDULE_BUFFER_B_ADDR;
        ScheduleIndex = 0;
        TempSchedule = (unsigned long *) SCHEDULE_BUFFER_A_ADDR;
        NextEmptyScheduleEntry = TempSchedule;
    } 
    else {  
        Schedule = (unsigned long *) SCHEDULE_BUFFER_A_ADDR;
        ScheduleIndex = 0;
        TempSchedule = (unsigned long *) SCHEDULE_BUFFER_B_ADDR;
        NextEmptyScheduleEntry = TempSchedule;

    }
}

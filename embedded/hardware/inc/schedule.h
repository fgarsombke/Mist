typedef struct {
    unsigned long zone;
    unsigned long start_time;
    unsigned long end_time;
    unsigned long not_completed;
} schedule_entry_t;

extern schedule_entry_t day_schedule[40];

void scheduleParse(char*);
void scheduleExtract(char*);



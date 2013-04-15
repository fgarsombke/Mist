#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__


typedef struct {
    unsigned long zone;
    unsigned long start_time;
    unsigned long end_time;
    unsigned long not_completed;
} schedule_entry_t;

// Global variable for storying one days worth of a schedule

extern schedule_entry_t schedule[];
//extern size_t schedule_idx;
extern unsigned long schedule_idx;

// Extracts the JSON from HTTP data and returns it as a NULL terminated string.
// Does not create a new string because of RAM constraints.
void scheduleExtract(char*);

// Sorts a schedule
void scheduleSort(void);

// Parses a JSON schedule into the global variable
// schedule and sorts it
void scheduleParse(char *);

#endif // __SCHEDULE_H__



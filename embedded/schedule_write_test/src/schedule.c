#include "inc/hw_types.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "fifo.h"
#include "json.h"
#include "schedule.h"
#include "wifly.h"

// for sorting
typedef int (*compfn)(const void*, const void*);

// Constants used for parsing
const char* JSON_START = "\r\n\r\n";
const char* JSON_END = "*CLOS*";

// Global variable for storing a week of schedules
#define SCHED_SIZE 40
schedule_entry_t schedule[SCHED_SIZE];
// size_t schedule_idx = 0;
unsigned long schedule_idx = 0;

// Extracts the JSON from HTTP data and returns it as a NULL terminated string.
// Does not create a new string because of RAM constraints.
void scheduleExtract(char *http_data) {
    char *json_start = strstr(http_data, JSON_START);
    char *json_end   = strstr(http_data, JSON_END);
    
    while((*json_start == '\r') || (*json_start == '\n')) json_start++; // Consume until JSON
    *json_end = NULL;

    http_data = strcpy(http_data, json_start);   
}
// Parses a JSON schedule into the global variable
// schedule and sorts it
void scheduleParse(char *json) {
    JSON_Parse(json);
}

// schedule_entry_t
int compare(schedule_entry_t* elem1, schedule_entry_t* elem2){
   if (elem1->start_time < elem2->start_time)
      return -1;

   else if (elem1->start_time > elem2->start_time)
      return 1;

   else
      return 0;
}

// Sorts the schedule
void scheduleSort(void) {
   qsort((void *) &schedule,    // Beginning address of array
   SCHED_SIZE,                  // Number of elements in array
   sizeof(schedule_entry_t),    // Size of each element
   (compfn)compare );           // Pointer to compare function
   
   
}


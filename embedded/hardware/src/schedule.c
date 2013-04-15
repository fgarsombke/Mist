#include "inc/hw_types.h"

//#include "drivers/rit128x96x4.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "schedule.h"
#include "wifly.h"

const char* ZONE_LABEL = "z";
const char* ZONE_NUM_LABEL = "zoneNumber";
const char* TIMES_ARRAY_LABEL = "times";
const char* START_LABEL = "startTime";
const char* END_LABEL = "endTime";
const char* JSON_START = "\r\n\r\n";
const char* JSON_END = "*CLOS*";

schedule_entry_t day_schedule[40];


// Extracts the JSON from HTTP data and returns it as a NULL terminated string.
// Does not create a new string because of RAM constraints.
void scheduleExtract(char *http_data) {
    char *json_start = strstr(http_data, JSON_START);
    char *json_end   = strstr(http_data, JSON_END);
    
    while((*json_start == '\r') || (*json_start == '\n')) json_start++; // Consume until JSON
    *json_end = NULL;

    http_data = strcpy(http_data, json_start); // move to the front of the string
}

// Extracts the zones and times from a JSON formatted string
void scheduleParse(char *json){
}

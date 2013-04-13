#include "inc/hw_types.h"

//#include "drivers/rit128x96x4.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "cJSON.h"
#include "wifly.h"

const char* ZONE_LABEL = "z";
const char* ZONE_NUM_LABEL = "zoneNumber";
const char* TIMES_ARRAY_LABEL = "times";
const char* START_LABEL = "startTime";
const char* END_LABEL = "endTime";
const char* JSON_START = "\r\n\r\n";
const char* JSON_END = "*CLOS*";


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
    cJSON *schedule = cJSON_Parse(json);
    
    if (!schedule){/*error*/}
    
    else{
        char debug[34]; 
        int array_idx, time_idx;
        
        cJSON *zone_array = cJSON_GetObjectItem(schedule, ZONE_LABEL);
       
        for(array_idx = 0; array_idx < cJSON_GetArraySize(zone_array); array_idx++) {
            cJSON *zone = cJSON_GetArrayItem(zone_array, array_idx);
            int zone_num = cJSON_GetObjectItem(zone, ZONE_NUM_LABEL)->valueint;
            
            cJSON *timeArray = cJSON_GetObjectItem(zone, TIMES_ARRAY_LABEL);
            for(time_idx = 0; time_idx < cJSON_GetArraySize(timeArray); time_idx++) {
                cJSON *time = cJSON_GetArrayItem(timeArray, time_idx);
                int start_time = cJSON_GetObjectItem(time, START_LABEL)->valueint;
                int end_time = cJSON_GetObjectItem(time, END_LABEL)->valueint;
                
                sprintf(debug, "Zone %d = {%d, %d}\n", zone_num, start_time, end_time);
                //RIT128x96x4StringDraw(debug, 0, (debug_idx++)*8, 15);
            }
        }
        
        cJSON_Delete(schedule);
    }
}

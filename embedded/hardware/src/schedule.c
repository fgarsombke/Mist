#include "inc/hw_types.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "ctype.h"
#include "fifo.h"
#include "schedule.h"
#include "wifly.h"

// Global variable for storing a week of schedules
#define SCHED_SIZE 40
schedule_entry_t schedule[SCHED_SIZE];
size_t schedule_idx = 0;

// Constants used for parsing
const char* HTTP_JSON_START = "\r\n\r\n";
const char* HTTP_JSON_END = "*CLOS*";
const char* ZONE_KEY = "zoneNumber";
const char* START_KEY = "startTime";
const char* END_KEY = "endTime";
enum key_t { ZONE, START, END };
const char* const KEYS[] = {"zoneNumber", "startTime", "endTime"};

// Local functions for parsing
long getNextNum(unsigned long*);
long getNextString(char*ng);
long nextDigit(void);
long nextAlpha(void);

// Extracts the JSON from HTTP data and returns it as a NULL terminated string.
// Does not create a new string because of RAM constraints.
void scheduleExtract(char *http_data) {
    char *json_start = strstr(http_data, HTTP_JSON_START);
    char *json_end   = strstr(http_data, HTTP_JSON_END);
    
    while((*json_start == '\r') || (*json_start == '\n')) json_start++; // Consume until JSON
    *json_end = NULL;

    http_data = strcpy(http_data, json_start);   
}
// Parses a JSON schedule into the global variable
// schedule and sorts it
void scheduleParse(void) {
    char key[11];
    enum key_t i;
    unsigned long value, current_zone;
    while(RxFifo_Size() > 0) {
        if(!getNextString(key))
            break;
        for(i = ZONE; i < (sizeof(KEYS)/sizeof(char*)); i++) {
            if(!strcmp(key, KEYS[i])) {
                if(!getNextNum(&value)) 
                    break;
                switch(i) {
                    case ZONE:
                        current_zone = value;
                        break;
                    case START:
                        schedule[schedule_idx].zone = current_zone;
                        schedule[schedule_idx].start_time = value;
                        break;
                    case END:
                        schedule[schedule_idx].end_time = value;
                        schedule_idx++;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

// get the next number
long getNextNum(unsigned long* num) {
    long status = 0;
    char num_s[11], ch;
    size_t i = 0; 
    
    if(nextDigit()) {
        while(RxFifo_Get(&ch)) { 
            if(isdigit(ch)) {
                num_s[i++] = ch;
            }
            else {
                num_s[i] = '\0'; 
                *num = strtoul(num_s, NULL, 10);
                status = 1;
                break;
            }
        }
    }
    return status; 
}

// get the next key
long getNextString(char* string) {
    long status;
    char ch; 
    if(nextAlpha()) {
        while(RxFifo_Get(&ch)) {
            if(isalpha(ch)) 
                *string++ = ch;
            else {
                *string = '\0';
                status = 1; 
                break;
            }
        }
    }
    return status;
}

// advance the fifo to the next digit
long nextDigit(void) {
    long status = 0;
    char ch; 
    while(RxFifo_Peek(&ch)) {
        if(isdigit(ch)) {
            status = 1;
            break;
        }
        RxFifo_Get(&ch);
    }
    return status; 
}

// advance the fifo to the next alphabetic letter
long nextAlpha(void) {
    long status = 0;
    char ch; 
    while(RxFifo_Peek(&ch)) {
        if(isalpha(ch)) {
            status = 1;
            break;
        }
        RxFifo_Get(&ch);
    }
    return status;
}

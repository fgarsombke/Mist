#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "jsmn.h"
#include "json.h"
#include "schedule.h"
#include "string.h"

// Tokens must be a power of 2
#define JSON_TOKENS 256

// Constants for parsing
const char* ROOT_KEY = "z";
const char* ZONE_KEY = "zoneNumber";
const char* START_KEY = "startTime";
const char* END_KEY = "endTime";

// Private functions for parsing
bool json_token_streq(char*, jsmntok_t*, const char*);
char* json_token_tostr(char*, jsmntok_t*);
unsigned long json_token_toul(char*, jsmntok_t*);


// This function parses and stores a JSON schedule
// PERFORMS NO ERROR CHECKING OR HANDLING
void JSON_Parse(char* json) {
    // Initialization variables
    jsmn_parser parser;
    jsmntok_t tokens[JSON_TOKENS];
    
    
    // State machine variables
    typedef enum { START, ZONE, START_TIME, END_TIME, SKIP} parse_state;
    parse_state state;
    unsigned long current_zone = 0;
    size_t i , j;
    
    // Initialize the JSMN parser
    // Note that the tokens return indices
    jsmn_init(&parser);
    jsmn_parse(&parser, json, tokens, JSON_TOKENS);

    // JSMN State Machine by example of 
    // https://github.com/alisdair/jsmn-example
    state = START;
    for (i = 0, j = 1; j > 0; i++, j--) {
        jsmntok_t *t = &tokens[i];
        
        // Continously redefines tokens left to be parsed
        if (t->type == JSMN_ARRAY || t->type == JSMN_OBJECT)
            j += t->size;

        switch (state) {
            // Start
            case START:
                state = SKIP;
            
                break;

            // Skip over tokens we don't care about
            case SKIP:
             
                state = SKIP;

                // Search for objects of importance
                if (t->type == JSMN_STRING) {
                    if(json_token_streq(json, t, ZONE_KEY))
                        state = ZONE;
                    if(json_token_streq(json, t, START_KEY))
                        state = START_TIME;
                    if(json_token_streq(json, t, END_KEY))
                        state = END_TIME;
                }    
                
                break;

            // Parses a zone and stores it for use later
            // Each call sets a new zone
            case ZONE:
                
                state = SKIP;
            
                current_zone = json_token_toul(json, t);
            
                break;

            // Saves the start time and zone
            case START_TIME:

                state = SKIP;
            
                schedule[schedule_idx].zone = current_zone;
                schedule[schedule_idx].start_time = json_token_toul(json, t);

                break;

            // Saves the end time moves to the next watering time
            case END_TIME:
                
                state = SKIP;
            
                schedule[schedule_idx].end_time = json_token_toul(json, t);
                schedule_idx++;
            
                break;
            default:
                break;
        }
    }
}

// strncmp for JSMN tokens
bool json_token_streq(char *js, jsmntok_t *t, const char *s) {
    return (strncmp(js + t->start, s, t->end - t->start) == 0 && strlen(s) == (size_t) (t->end - t->start));
}

// Converts a JSMN token to a string
char * json_token_tostr(char *js, jsmntok_t *t) {
    js[t->end] = '\0';
    return js + t->start;
}

// Converts a JSMN token to an unsigned long
unsigned long json_token_toul(char *js, jsmntok_t *t) {
    js[t->end] = '\0';
    return strtoul((js + t->start), NULL, 0);
}

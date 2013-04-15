#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "jsmn.h"
#include "schedule.h"
#include "string.h"

#define JSON_TOKENS 256

const char* ROOT_KEY = "z";
const char* ZONE_KEY = "zoneNumber";
const char* TIME_KEY = "times";
const char* START_KEY = "startTime";
const char* END_KEY = "endTime";

bool json_token_streq(char*, jsmntok_t*, const char*);
char * json_token_tostr(char*, jsmntok_t*);
unsigned long json_token_toul(char*, jsmntok_t*);

void JSON_Parse(char* json) {
    // Initialization variables
    jsmn_parser parser;
    jsmntok_t tokens[JSON_TOKENS];
    
    
    // State machine variables
    typedef enum { START, ZONE, START_TIME, END_TIME, SKIP, STOP } parse_state;
    parse_state state;
    unsigned long current_zone = 0;
    size_t object_tokens = 0, i , j, idx = 0;
    
    jsmn_init(&parser);
    jsmn_parse(&parser, json, tokens, JSON_TOKENS);

    // State Machine
    // https://github.com/alisdair/jsmn-example
    state = START;
    for (i = 0, j = 1; j > 0; i++, j--) {
        jsmntok_t *t = &tokens[i];

        if (t->type == JSMN_ARRAY || t->type == JSMN_OBJECT)
            j += t->size;

        switch (state)
        {
            case START:
                state = SKIP;
                object_tokens = t->size;

                //if (object_tokens == 0)
                //    state = STOP;
                break;

            case SKIP:
                object_tokens += (t->size-1);
                state = SKIP;

                if (t->type == JSMN_STRING) {
                    if(json_token_streq(json, t, ZONE_KEY))
                        state = ZONE;
                    if(json_token_streq(json, t, START_KEY))
                        state = START_TIME;
                    if(json_token_streq(json, t, END_KEY))
                        state = END_TIME;
                }    
                
                //if (object_tokens == 0)
                //    state = STOP;
                break;

            case ZONE:
                object_tokens--;
                state = SKIP;
                current_zone = json_token_toul(json, t);

                //if (object_tokens == 0)
                //    state = STOP;
                break;

            case START_TIME:
                object_tokens--;
                day_schedule[idx].zone = current_zone;
                day_schedule[idx].start_time = json_token_toul(json, t);
                state = SKIP;

                //if (object_tokens == 0)
                //    state = STOP;
                break;

            case END_TIME:
                object_tokens--;
                day_schedule[idx].end_time = json_token_toul(json, t);
                idx++;
                state = SKIP;

                //if (object_tokens == 0)
                //    state = STOP;
                 break;

            case STOP:
                break;

            default:
                break;
        }
    }
}

bool json_token_streq(char *js, jsmntok_t *t, const char *s) {
    return (strncmp(js + t->start, s, t->end - t->start) == 0 && strlen(s) == (size_t) (t->end - t->start));
}

char * json_token_tostr(char *js, jsmntok_t *t) {
    js[t->end] = '\0';
    return js + t->start;
}

unsigned long json_token_toul(char *js, jsmntok_t *t) {
    js[t->end] = '\0';
    return strtoul(js + t->start, NULL, 0);
}

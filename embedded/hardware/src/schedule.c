#include "wifly.h"
#include "stdio.h"
#include "cJSON.h"

void doit(char *json)
{
    cJSON *sched = cJSON_Parse(json);
    if (!sched) {/*error*/}
    else
    {
        char buffer[20];
        int i, j;
        cJSON *zoneArray = cJSON_GetObjectItem(sched, "z");
        for(i = 0; i < cJSON_GetArraySize(zoneArray); i++) {
            cJSON *zone = cJSON_GetArrayItem(zoneArray, i);
            int zoneNum = cJSON_GetObjectItem(zone, "zoneNumber")->valueint;
            cJSON *timeArray = cJSON_GetObjectItem(zone, "times");

            for(j = 0; j < cJSON_GetArraySize(timeArray); j++) {
                cJSON *time = cJSON_GetArrayItem(timeArray, j);
                int start = cJSON_GetObjectItem(time, "startTime")->valueint;
                int end = cJSON_GetObjectItem(time, "endTime")->valueint;
                sprintf(buffer, "Zone %d = {%d, %d}\n", zoneNum, start, end);
            }
        }
        cJSON_Delete(sched);
    }
}

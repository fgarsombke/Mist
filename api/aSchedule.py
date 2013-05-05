import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from datetime import datetime
import time
from db import DBConfig
import collections
import json
from jsonencode import MyEncoder

class IrrigationEvent:
    def __init__(self, deviceID, zoneNumber, startTime, duration, date):
        self.deviceID = deviceID
        self.zoneNumber = zoneNumber
        self.startTime = startTime
        self.duration = duration
        self.date = date

def queueIrrigation(event):
    startTime = event.date + " " + event.startTime
    timestamp = datetime.now()
    conf = DBConfig.DBConfig()
    results = conf.executeLastRowID("INSERT INTO queuedIrrigations (deviceID, zoneNumber, startTime, duration, created) VALUES (%s, %s, %s, %s, %s)", (event.deviceID, event.zoneNumber, startTime, event.duration, timestamp))
    return results

def getScheduleForDevice(deviceID):
    conf = DBConfig.DBConfig()
    results = conf.executeFetchAll("SELECT * FROM queuedIrrigations WHERE (queuedIrrigations.deviceID = (%s)) AND vectorID=(SELECT max(vectorID) FROM queuedIrrigations) ORDER BY zoneNumber ASC", (deviceID))
    return results

class aSchedule:
    #GET API FOR SCHEDULE
    #Return all queued irrigations for a given device:.
    #Expected parameter: deviceID (integer)
    def GET(self):
        schedule_data = web.input()

        if schedule_data.deviceID:
            schedules = getScheduleForDevice(schedule_data.deviceID) #SQL
            schedule_list = []
            currzone_list = []
            currDict = collections.OrderedDict()
            currDict['zoneNumber'] = 1
            currentZone = 1

            #iterate through all irrigation events, they are sorted by zone number
            for row in schedules:
                if row[1] != currentZone: #we are on new zone
                    #create zone dictionary from current list of events for this zone
                    currDict['times'] = currzone_list
                    #add the dictionary to the list of zone dictionarys
                    schedule_list.append(currDict)
                    #reset the current zone list
                    currzone_list = []
                    currDict = collections.OrderedDict()
                    #increment the curent zone
                    currDict['zoneNumber'] = row[1]
                    currentZone = row[1]

                d = collections.OrderedDict()
                if row[2]:
                    d['startTime'] = int(time.mktime(row[2].timetuple()))
                if row[2] and row[3]:
                    d['endTime'] = int(time.mktime(row[2].timetuple()) + (row[3]*60))
                currzone_list.append(d)

            #GRAB the last one
            currDict['times'] = currzone_list
            #add the dictionary to the list of zone dictionarys
            schedule_list.append(currDict)

            #DONE, turn output into a one key dictionary
            output = collections.OrderedDict()
            output['z'] = schedule_list
            j = json.dumps(output, cls=MyEncoder)
            j = "".join(str(j).split())
            print j
            return j
        else:
            return 0

    #POST API - queue an irrigation event
    #parameters deviceID, zoneNumber, startTime, duration
    def POST(self):
        schedule_data = web.input() 

        if schedule_data:
            event = IrrigationEvent(schedule_data.deviceID, schedule_data.zoneNumber, schedule_data.startTime, schedule_data.duration, schedule_data.date)
            success = queueIrrigation(event)
            return success

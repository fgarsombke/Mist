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
    def __init__(self, productID, zoneNumber, startTime, duration, date):
        self.productID = productID
        self.zoneNumber = zoneNumber
        self.startTime = startTime
        self.duration = duration
        self.date = date

def queueIrrigation(event):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()    
    cursor = db.cursor()
    startTime = event.date + " " + event.startTime
    timestamp = datetime.now() 
    cursor.execute("INSERT INTO queuedIrrigations (productID, zoneNumber, startTime, duration, created) VALUES (%s, %s, %s, %s, %s)", (event.productID, event.zoneNumber, startTime, event.duration, timestamp))
    results = cursor.lastrowid
    db.commit()
    return results

def getScheduleForDevice(productID):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()    
    cursor = db.cursor()
    cursor.execute("SELECT * FROM queuedIrrigations WHERE (queuedIrrigations.productID = (%s)) ORDER by zoneNumber ASC", (productID))
    results = cursor.fetchall()
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
                    d['startTime'] = time.mktime(row[2].timetuple())
                if row[2] and row[3]:
                    d['endTime'] = time.mktime(row[2].timetuple()) + (row[3]*60)
                currzone_list.append(d)

            #GRAB the last one
            currDict['times'] = currzone_list
            #add the dictionary to the list of zone dictionarys
            schedule_list.append(currDict)

            #DONE, turn output into a one key dictionary
            output = collections.OrderedDict()
            output['z'] = schedule_list
            j = json.dumps(output, cls=MyEncoder)
            return j
        else:
            return 0

    #POST API - queue an irrigation event
    #parameters productID, zoneNumber, startTime, duration
    def POST(self):
        schedule_data = web.input() 

        if schedule_data:
            event = IrrigationEvent(schedule_Data.productID, schedule_data.zoneNumber, schedule_data.startTime, schedule_data.date, schedule_data.duration)
            success = queueIrrigation(event)
            return success

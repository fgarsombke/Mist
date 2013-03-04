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

def queueIrrigation(productID, zoneNumber, startTime, duration, date):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()    
    cursor = db.cursor()
    startTime = date + " " + startTime #NEED TO FIX THIS FORMATTING
    timestamp = datetime.now() 
    cursor.execute("INSERT INTO queuedIrrigations (productID, zoneNumber, startTime, duration, created) VALUES (%s, %s, %s, %s, %s)", (productID, zoneNumber, startTime, duration, timestamp))
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
    #Expected parameter: productID (integer)
    def GET(self):
        schedule_data = web.input()

        if schedule_data.deviceID:
            schedules = getScheduleForDevice(schedule_data.deviceID) #SQL
            schedule_list = []
            currzone_list = []
            currDict = collections.OrderedDict()
            currentZone = 1

            #iterate through all irrigation events, they are sorted by zone number
            for row in schedules:
                if row[1] != currentZone: #we are on next zone
                    #create zone dictionary from current list of events for this zone
                    currDict['i'] = currzone_list
                    currDict['zoneNumber'] = row[1]
                    #add the dictionary to the list of zone dictionarys
                    schedule_list.append(currDict)
                    #reset the current zone list
                    currzone_list = []
                    currDict = collections.OrderedDict()
                    #increment the curent zone
                    currentZone = row[1]

                d = collections.OrderedDict()
                if row[2]:
                    d['startTime'] = time.mktime(row[2].timetuple())
                if row[2] and row[3]:
                    d['endTime'] = time.mktime(row[2].timetuple()) + (row[3]*60)
                currzone_list.append(d)
           
           #GRAB the last one
            currDict['i'] = currzone_list
            currDict['zoneNumber'] = currentZone
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
            productID = schedule_data.productID
            zoneNumber = schedule_data.zoneNumber
            startTime = schedule_data.startTime
            date = schedule_data.date
            duration = schedule_data.duration
            success = queueIrrigation(productID, zoneNumber, startTime, duration, date) 
            return success

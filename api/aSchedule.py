import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from datetime import datetime
import time
sys.path.append('../db')
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
    cursor.execute("SELECT * FROM queuedIrrigations WHERE (queuedIrrigations.productID = (%s))", (productID))
    results = cursor.fetchall()
    return results  

class aSchedule:
    #GET API FOR SCHEDULE
    #Return all queued irrigations for a given device:.
    #Expected parameter: productID (integer)
    def GET(self):
        schedule_data = web.input()
        
        if schedule_data.deviceID:
            schedules = getScheduleForDevice(schedule_data.deviceID) # SQL
            objects_list = []
            for row in schedules:
                d = collections.OrderedDict()
                d['productID'] = row[0]
                d['zoneNumber'] = row[1]
                d['startTime'] = row[2]
                d['duration'] = row[3]
                d['created'] = row[4]
                objects_list.append(d)
            j = json.dumps(objects_list, cls=MyEncoder)
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

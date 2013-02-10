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
import urllib2
import urllib
import json


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

class schedule:
    def GET(self):
        schedule_data = web.input()
        render = web.template.render('templates/', base='layout')
        #get user session
        deviceID = 0

        #if user submitted deviceID via a form...
        if schedule_data:
            if schedule_data.deviceID:
                deviceID = schedule_data.deviceID

        #if user logged in (session deviceID exists) - get schedule
        session = web.config._session
        if session.deviceID:
            deviceID = session.deviceID
        #use deviceID to get schedule from api/aSchedule.
        if deviceID != 0:
            query_args = {'deviceID':deviceID}
            encoded_args = urllib.urlencode(query_args)
            url = "http://0.0.0.0:8080/api/schedule?" + encoded_args
            schedule = ""
            try:
                schedule = json.loads(urllib2.urlopen(url).read())
            except urllib2.URLError, e:
                print e
            return render.schedulelist(schedule)
        #otherwise go back to form...
        else:
            return render.schedule()

    def POST(self):
        schedule_data = web.input() 
        render = web.template.render('templates/', base='layout')
        #check if user logged in
        #if so use his deviceID to post another schedule...
        if schedule_data:   
            productID = schedule_data.deviceID
            zoneNumber = schedule_data.zoneNumber
            startTime = schedule_data.startTime
            date = schedule_data.date
            duration = schedule_data.duration
            queueIrrigation(productID, zoneNumber, startTime, duration, date) 
            return render.schedule()

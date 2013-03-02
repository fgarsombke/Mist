import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from datetime import datetime
import time
from db import DBConfig
import urllib2
import urllib
import json


class addSchedule:
    def GET(self):
        render = web.template.render('templates/', base='layout')
        return render.addSchedule()

    def POST(self):
        schedule_data = web.input() 
        render = web.template.render('templates/', base='layout')

        if schedule_data:
            productID = schedule_data.deviceID
            zoneNumber = schedule_data.zoneNumber
            startTime = schedule_data.startTime
            date = schedule_data.date
            duration = schedule_data.duration

        #Construct POST API Request
        query_args = {'productID':productID, 'zoneNumber':zoneNumber, 'startTime':startTime, 'date':date, 'duration':duration}
        encoded_args = urllib.urlencode(query_args)
        url = "http://0.0.0.0:8080/api/schedule?"
        req = urllib2.Request(url, encoded_args)
        devices = ""

        #Execute API request
        try:
            schedule = json.loads(urllib2.urlopen(req).read())
        except urllib2.URLError, e:
            print e
        #Execute POST API Rquest
        return render.addSchedule()

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
        print deviceID
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
        render = web.template.render('templates/', base='layout')
        return render.schedule()

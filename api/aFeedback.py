import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from db import DBConfig
import json
import collections
from jsonencode import MyEncoder
from watercalc import learn

def storeFeedback(deviceID, zoneNumber, created, value):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("INSERT INTO feedback (deviceID, zoneNumber, created, value) VALUES (%s, %s, FROM_UNIXTIME(%s), %s)", (deviceID, zoneNumber, created, value))
    db.commit()
    return 0

class aFeedback:
    def GET(self):
        feedback_data = web.input()
        return 0

    def POST(self):
        data = web.data()
        data = eval(data)
        sim = web.input()
        print data['deviceID']
        if sim.simulator:  #then this is coming from the simulator
            if 'feedback' in data:
                for item in data["feedback"]:
                    print item["zoneNumber"]
                    print item["time"]
                    print item["value"]
                    storeFeedback(data['deviceID'], item["zoneNumber"], item["time"], item["value"])
            else:#first call ever from simulator
                print "empty feedback"
            learn.doLearning(data['deviceID'], 1, data['endTime'])
        else:#this is coming from the iPhone app
            deviceID = str(data['deviceID'])
            zoneNumber = str(data['zoneNumber'])
            created = str(data['created'])
            value = str(data['value'])
            storeFeedback(deviceID, zoneNumber, created, value)

